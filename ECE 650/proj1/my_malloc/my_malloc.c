#include "my_malloc.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//remove a block
void remove_from_list(block_t * ptr, block_t** cur_head) {
    if (ptr->prev == NULL) { //change head
        *cur_head = ptr->next;
    } 
    else {
        ptr->prev->next = ptr->next;
    }
    if (ptr->next) { 
        ptr->next->prev = ptr->prev;
    }
    ptr->next = NULL;
    ptr->prev = NULL;
}

void add_into_list(block_t *ptr, block_t** cur_head) {
    block_t* curr = *cur_head;
    while (curr) {
        if (ptr < curr) {
            ptr->prev = curr->prev;
            ptr->next = curr;
            curr->prev = ptr;
            curr = ptr;
            return;
        }
        curr = curr->next;;
    }
    ptr->next = curr;
    curr = ptr;
}

//split function, returning ptr to splitted free block
block_t* split(block_t* ptr, size_t size) {
    block_t* temp = (block_t*)((unsigned long)ptr + sizeof(block_t) + size);
    temp->size = ptr->size - size - sizeof(block_t);
    ptr->size = size;
    return temp;
}

//after freeing, we need to merge continuous free blocks
void merge(block_t* ptr) {
    unsigned cur_block_addr = (unsigned long)ptr;

    if(ptr->next == NULL) return;
    unsigned next_block_addr = (unsigned long)ptr->next;

    //check merge with next
    if(cur_block_addr + (unsigned long)sizeof(block_t) + (unsigned long)ptr->size == next_block_addr) {
        ptr->size += ptr->next->size + sizeof(block_t);
        remove_from_list(ptr->next, &head);
    }
}

//find best fit block
block_t* find_best_fit(size_t size, block_t** cur_head)
{
    size_t best_size = ULONG_MAX;
    block_t* cur = *cur_head;
    block_t* best_ptr = NULL;
    while(cur) {
        if(cur->size == size) {
            return cur;
        }
        else if (cur->size > size) {
            if(cur->size < best_size) {
                best_size = cur->size;
                best_ptr = cur;
            }
        }
        cur = cur->next;
    }
    return best_ptr;
}

block_t* create_new_block(size_t needed_size)
{
    //block_t *new_block = sbrk(0);
    block_t* new_block = sbrk(needed_size);
    if(!new_block) {
        printf("sbrk failed\n");
        return NULL;
    }
    new_block->size = needed_size - sizeof(block_t);
    new_block->next = NULL;
    return new_block;
}

//Thread Safe malloc/free: locking version 
void *bf_malloc(size_t size){
    if(size <= 0) {
        return NULL;
    }
    assert(pthread_mutex_lock(&lock)==0);
    size_t needed_size = size + sizeof(block_t);
    block_t* new_block = NULL;
    block_t* best_ptr = find_best_fit(size, &head);
    
    if(!best_ptr) { //we cant find a fit, ask for more
        new_block = create_new_block(needed_size);
        add_into_list(new_block, &head);
        assert(pthread_mutex_unlock(&lock)==0);
        return (void*)((unsigned long)new_block + sizeof(block_t));
    }
    else{ //we find the best choice
        remove_from_list(best_ptr, &head);
        if(best_ptr->size > needed_size) {
            block_t* temp = split(best_ptr, size);
            add_into_list(temp, &head);
        }
        void* block_addr = (void*)((unsigned long)best_ptr + sizeof(block_t));
        assert(pthread_mutex_unlock(&lock)==0);
        return block_addr;
    }
}

void bf_free(void *ptr){
    assert(pthread_mutex_lock(&lock)==0);
    if(!ptr) {
        printf("Error: freeing a NULL pointer\n");
    }
    block_t* cur_header = (block_t*)((unsigned long)ptr - sizeof(block_t));
    add_into_list(cur_header, &head);
    merge(cur_header);
    assert(pthread_mutex_unlock(&lock)==0);
}

//Thread Safe malloc/free: non-locking version 
//void *ts_malloc_nolock(size_t size);
//void ts_free_nolock(void *ptr);

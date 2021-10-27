#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <assert.h>

//basic linked list node
typedef struct block block_t;
struct block
{
    block_t* next;
    block_t* prev;
    size_t size;
};

//free list head
static block_t* head = NULL;
__thread block_t* head_tls = NULL;


//Thread Safe malloc/free: locking version 
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

//Thread Safe malloc/free: non-locking version 
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

//other functions
void remove_from_list(block_t * ptr, block_t** cur_head);
void add_into_list(block_t *ptr, block_t** cur_head);
block_t* split(block_t* ptr, size_t size);
void merge(block_t* ptr);
block_t* find_best_fit(size_t size, block_t** cur_head);
block_t* create_new_block(size_t needed_size);


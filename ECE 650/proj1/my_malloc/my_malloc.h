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


static block_t* head = NULL; //free list head
static unsigned long segment_size = 0;
static unsigned long used_size = 0;
static unsigned long number_of_blocks = 0;

//first found
void *ff_malloc(size_t size); 

void ff_free(void *ptr);



//best fit
void *bf_malloc(size_t size); 

void bf_free(void *ptr);


//other functions
void remove_from_list(block_t * ptr, block_t** cur_head);
void add_into_list(block_t *ptr, block_t** cur_head);
block_t* split(block_t* ptr, size_t size);
void merge(block_t* ptr);
unsigned long get_data_segment_size(){
    return 0;
}
unsigned long get_data_segment_free_space_size(){
    return 0;
}


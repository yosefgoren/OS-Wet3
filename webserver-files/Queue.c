#include "Queue.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

//#define ASSERT(cond, msg) if(!cond){printf("%s\n", msg); assert(cond);}
#define ASSERT(cond, msg) assert(cond);
#define CHECK_NULL(param) ASSERT(param != NULL, "unexpected null parameter")
#define EXPECTED(cond) ASSERT(cond, "an inner variable was found to have an unexpected value.");

typedef struct node_t{
    //the 'prior' field in the node 'n', is a node representing the item that came before 'n',
    //so 'n->prior' should be dequeued before 'n'.
    struct node_t* prior;
    //the 'next' field in the node 'n', is a node representing the item the came after 'n',
    //'n->next' should be dequed after 'n'.
    struct node_t* next;
    QueueItemType data;
}* node;

struct Queue_t{
    node oldest;
    node newest;
    int size;
    int max_size; 
};

/**
 * the size of the 'should_drop' array should be the size of the queue, for each index it mark whether 
 * the item in that same index should be dropped from the queue (in relation to the initial state of 'queue').
 */
static void dropAtIndices(Queue* queue, bool* should_drop){
    CHECK_NULL(queue);
    CHECK_NULL(should_drop);

    node current = queue->oldest;
    for(int i = 0; i < queue->size; ++i){
        if(should_drop[i]){
            if(current == queue->oldest){
                queue->oldest = current->next;
            } else {
                current->prior->next = current->next;
            }
            if(current == queue->newest){
                queue->newest = current->prior;
            } else {
                current->next->prior = current->prior;
            }

            free(current);
        }
        current = current->next;
    }
}

/**
 * fills the array with boolean values s.t. 'num_to_mark' of them are true, and the rest are false.
 * the indices of the true values are chosen at random, with even distribution.
 */
static void markRandomIndices(bool* array, int array_size, int num_to_mark){
    CHECK_NULL(array);
    for(int i = 0; i < array_size; ++i)
        array[i] = false;

    while(num_to_mark > 0){
        int rand_index = random()%array_size;
        if(array[rand_index])
            continue;
        array[rand_index] = true;
        --num_to_mark;
    }
}

void dropRandQuarter(Queue* queue){
    CHECK_NULL(queue);
    if(emptyQ(queue))
        return;
    bool should_drop[queue->size];
    markRandomIndices(should_drop, queue->size, queue->size/4);
    dropAtIndices(queue, should_drop);
}

Queue* initQ(int max_size){
    ASSERT(max_size >= 0, "max_size parameter must be non-negaitve.");
    Queue* res = (Queue*)malloc(sizeof(Queue));
    res->oldest = NULL;
    res->newest = NULL;
    res->size = 0;
    res->max_size = max_size;
    return res;
}

void enqueueQ(Queue* queue, QueueItemType item){
    CHECK_NULL(queue);
    ASSERT(queue->size < queue->max_size, "an attempt to enqueue into a full Queue was made.");

    node new_newest = (node)malloc(sizeof(struct node_t));
    node prior_newest = queue->newest;

    new_newest->prior = prior_newest;
    new_newest->next = NULL; 
    new_newest->data = item;
    if(prior_newest != NULL)
        prior_newest->next = new_newest;
    
    if(queue->oldest == NULL)
        queue->oldest = new_newest;

    queue->newest = new_newest;
    ++queue->size;
}

QueueItemType dequeueQ(Queue* queue){
    CHECK_NULL(queue);
    ASSERT(queue->size > 0, "an attempt to dequeue from an empty Queue was made.");
    
    node old_oldest = queue->oldest;
    EXPECTED(old_oldest != NULL);
    EXPECTED(old_oldest->prior == NULL);
    node new_oldest = old_oldest->next;
    QueueItemType res = old_oldest->data;
    free(old_oldest);
    
    if(new_oldest != NULL)
        new_oldest->prior = NULL;

    queue->oldest = new_oldest;
    if(old_oldest == queue->newest)
        queue->newest = NULL;
    --queue->size;
    
    if(queue->size == 0){
        queue->newest = queue->oldest = NULL;
    }

    return res;
}

bool emptyQ(Queue* queue){
    CHECK_NULL(queue);
    return queue->size == 0;
}

bool fullQ(Queue* queue){
    CHECK_NULL(queue);
    return queue->size == queue->max_size;
}
void destroyQ(Queue* queue){
    CHECK_NULL(queue);
    while(!emptyQ(queue))
        dequeueQ(queue);
    free(queue);
}

void printQ(Queue* queue){
    node current = queue->newest;
    while(current != NULL){
        printf("%d ", current->data);
        current = current->prior;
    }
    printf("\n");
}

#include "Queue.h"
#include "assert.h"

static const int null = 0;
#define ASSERT(cond, msg) if(!cond){printf("%s", msg); assert(cond);}
#define CHECK_NULL(param) ASSERT(param != null, "unexpected null parameter")
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

Queue* initQ(int max_size){
    ASSERT(max_size >= 0, "max_size parameter must be non-negaitve.");
    Queue* res = (Queue*)malloc(sizeof(Queue));
    res->oldest = null;
    res->newest = null;
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
    new_newest->next = null; 
    new_newest->data = item;
    if(prior_newest != null)
        prior_newest->next = new_newest;
    
    ++queue->size;
}

QueueItemType dequeueQ(Queue* queue){
    CHECK_NULL(queue);
    ASSERT(queue->size > 0, "an attempt to dequeue from an empty Queue was made.");
    
    node old_oldest = queue->oldest;
    EXPECTED(old_oldest != null);
    EXPECTED(old_oldest->prior == null);
    node new_oldest = old_oldest->next;
    QueueItemType res = old_oldest->data;
    free(old_oldest);
    
    queue->oldest = new_oldest;
    if(old_oldest == queue->newest)
        queue->newest = null;
    --queue->size;
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
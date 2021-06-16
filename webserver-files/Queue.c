#include "Queue.h"
#define null 0

typedef struct node_t{
    //the 'prior' field in the node 'n', is a node representing the item that came before 'n',
    //so 'n->prior' should be dequeued before 'n'.
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
    Queue* res = (Queue*)malloc(sizeof(Queue));
    res->oldest = null;
    res->newest = null;
    res->size = 0;
    res->max_size = max_size;
    return res;
}

void enqueueQ(Queue* queue, QueueItemType item){
    node new_oldest = (node)malloc(sizeof(struct node_t));
    node prior_oldest = queue->oldest;
}

QueueItemType dequeueQ(Queue* queue);
bool emptyQ(Queue* queue);
bool fullQ(Queue* queue);
void destroyQ(Queue* queue);
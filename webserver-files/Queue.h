#ifndef QUEUE
#define QUEUE
#include "stdbool.h"

typedef struct Queue_t Queue;
typedef int QueueItemType;

Queue* initQ(int max_size);
void enqueueQ(Queue* queue, QueueItemType item);
QueueItemType dequeueQ(Queue* queue);
bool emptyQ(Queue* queue);
bool fullQ(Queue* queue);
void destroyQ(Queue* queue);

#endif

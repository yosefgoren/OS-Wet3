#ifndef QUEUE
#define QUEUE
#include "stdbool.h"

#define QUEUE_NO_MAX_SIZE -1

typedef struct Queue_t Queue;
typedef int QueueItemType;

Queue* initQ(int max_size);
void enqueueQ(Queue* queue, QueueItemType item);
QueueItemType dequeueQ(Queue* queue, QueueItemType item);
bool emptyQ(Queue* queue);
bool fullQ(Queue* queue);
void destroyQ(Queue* queue);

#endif

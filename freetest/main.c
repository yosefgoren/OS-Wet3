#include <stdio.h>
#include <pthread.h>
#include "webserver-files/Queue.h"

Queue* shared_queue;
mutex_t m;
cond_t empty_cond;
cond_t full_cond;


void* doOnSharedQueue(void* operation_func){

}

void consumeTasks()
{
    int task;

    while (1) {
        
        mutex_lock(&m);
        while (emptyQ(shared_queue)) {
            cond_wait(&empty_cond, &m);
        }

        task = dequeueQ(shared_queue); //critical code
        cond_signal(&full_cond);

        mutex_unlock(&m);

        //do stuff with task:
        //...
    }
}

//producer method
void produceTasks()
{
    int task;
    while (scanf("%d", &task) != EOF) {
        
        mutex_lock(&m);
        while (fullQ(shared_queue)) {
            cond_wait(&full_cond, &m);
        }

        enqueueQ(shared_queue, task); //critical code
        cond_signal(&empty_cond);
        
        mutex_unlock(&m);
    }
}

int main(int argc, char *argv[]) {
    
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*argc);
    shared_queue = initQ(argv[1]);

    produceTasks();

    for (int i = 0; i < argc; ++i) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    return 0;
}

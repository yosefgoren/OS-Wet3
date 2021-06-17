#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../webserver-files/Queue.h"

Queue* shared_queue;
pthread_mutex_t m;
pthread_cond_t empty_cond;
pthread_cond_t full_cond;

void* handleTask(int task) {
    sleep(4);
    printf("thread num: %ld, got task: %d.\n",pthread_self()%10, task);
}

void* consumeTasks(void* arg)
{
    int task;

    while (1) {
        pthread_mutex_lock(&m);
        while (emptyQ(shared_queue)) {
            pthread_cond_wait(&empty_cond, &m);
        }

        task = dequeueQ(shared_queue); //critical code
        pthread_cond_signal(&full_cond);
        
        pthread_mutex_unlock(&m);

        handleTask(task);
    }
    return NULL;
}

//producer method
void produceTasks()
{
    int task;
    while (scanf("%d", &task) != EOF) {
        
        pthread_mutex_lock(&m);
        while (fullQ(shared_queue)) {
            pthread_cond_wait(&full_cond, &m);
        }

        enqueueQ(shared_queue, task); //critical code
        pthread_cond_signal(&empty_cond);
        
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[]) {
    //check if arguments are v
    if(argc != 3 && argc != 1){
        printf("there should be either 3 or 1 parameters:\n\
            first-the program name\n\
            second-number of threads.\n\
            third-size of queue.\n\
        (0 parameters to get default values)\n");
        exit(1);
    }
    int num_threads = 2, queue_size = 2;
    if(argc == 3){
        num_threads = atoi(argv[1]);
        queue_size = atoi(argv[2]); 
    }

    //create the threads array:
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
    //initialize the queue:
    shared_queue = initQ(queue_size);

    //start all of the threads:
    for(int i = 0; i < num_threads; ++i){
        pthread_create(threads+i, NULL, consumeTasks, NULL);
    }

    //start producing tasks:
    produceTasks();

    //close all of the threads:
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    //free the threads array:
    free(threads);
    //destroy the queue:
    destroyQ(shared_queue);
    return 0;
}

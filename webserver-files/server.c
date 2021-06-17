#include "segel.h"
#include "request.h"
#include <pthread.h>
#include "Queue.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>


// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

#define NUM_THREADS 2
#define QUEUE_SIZE 2

typedef enum OverloadPolicy_t{
    BLOCK,
    DROP_TAIL,
    DROP_HEAD,
    DROP_RAND
} OverloadPolicy;

Queue* shared_queue;
pthread_mutex_t m;
pthread_cond_t empty_cond;
pthread_cond_t full_cond;

// HW3: Parse the new arguments too
void getargs(int *port, int* num_threads, int* queue_size, OverloadPolicy* policy,int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s [portnum] [threads] [queue_size] [schedalg]\n", argv[0]);
	exit(1);
    }
    //get port:
    *port = atoi(argv[1]);

    //get the number of threads:
    *num_threads = atoi(argv[2]);

    //get the max size of the queue:
    *queue_size = atoi(argv[3]);

    //get policy:
    char* policy_str = argv[4];
    if(strcmp(policy_str, "block") == 0)
        *policy = BLOCK;
    else if(strcmp(policy_str, "dt") == 0)
        *policy = DROP_TAIL;
    else if(strcmp(policy_str, "dh") == 0)
        *policy = DROP_HEAD;
    else if(strcmp(policy_str, "random") == 0)
        *policy = DROP_RAND;
}

void* consumeRequests(void* null_arg)
{
    int connfd;

    while (1) {
        pthread_mutex_lock(&m);
        while (emptyQ(shared_queue)) {
            pthread_cond_wait(&empty_cond, &m);
        }

        connfd = dequeueQ(shared_queue); //critical code
        pthread_cond_signal(&full_cond);
        
        pthread_mutex_unlock(&m);

		requestHandle(connfd);
		Close(connfd);
    }
    return NULL;
}

void produceRequests(int port, OverloadPolicy policy)
{
	int listenfd, connfd, clientlen;
    struct sockaddr_in clientaddr;
    
    listenfd = Open_listenfd(port);

    while (true) {	
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

		pthread_mutex_lock(&m);

        if(policy == DROP_TAIL && fullQ(shared_queue)){
            pthread_mutex_unlock(&m);
            Close(connfd);
            break;
        }

        if(fullQ(shared_queue)){
            switch (policy)
            {
            case BLOCK:
                break;
            case DROP_TAIL:
                //this case should have been handled prior to this section.
                assert(false);
                break;
            case DROP_HEAD:
                //the 'top' request is ignored:
                Close(dequeueQ(shared_queue));
                break;
            case DROP_RAND:
                //a random 25% of the items in the queue are dropped:
                dropRandQuarter(shared_queue);
                break;
            }
        }

        while(fullQ(shared_queue))
            pthread_cond_wait(&full_cond, &m);

		enqueueQ(shared_queue, connfd); //critical code!  (>_<)
        pthread_cond_signal(&empty_cond);
        
        pthread_mutex_unlock(&m);
	}
}

int main(int argc, char *argv[])
{    
	int port, num_threads, queue_size;
    OverloadPolicy policy;
	getargs(&port, &num_threads, &queue_size, &policy, argc, argv);
    
	//create the threads array:
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
    //initialize the queue:
    shared_queue = initQ(queue_size);
    
    //start all of the threads:
    for(int i = 0; i < num_threads; ++i){
        pthread_create(threads+i, NULL, consumeRequests, NULL);
    }

    produceRequests(port, policy);

    //release all resources:
	for (int i = 0; i < num_threads; ++i) {
		pthread_join(threads[i], NULL);
    }
    //free the threads array:
    free(threads);
    //destroy the queue:
    destroyQ(shared_queue);
    return 0;
}


    


 

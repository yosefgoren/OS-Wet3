#include "segel.h"
#include "request.h"
#include <pthread.h>
#include "Queue.h"
#include <stdbool.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//
#define NUM_THREADS 4
#define QUEUE_SIZE 4

Queue* shared_queue;
pthread_mutex_t m;
pthread_cond_t empty_cond;
pthread_cond_t full_cond;

// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
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

void produceRequests(int argc, char** argv)
{
	int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    
	getargs(&port, argc, argv);
    listenfd = Open_listenfd(port);

    while (true) {	
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		
		pthread_mutex_lock(&m);
		while (fullQ(shared_queue)) {
            pthread_cond_wait(&full_cond, &m);
        }
        
		enqueueQ(shared_queue, connfd); //critical code!  (>_<)
        pthread_cond_signal(&empty_cond);
        
        pthread_mutex_unlock(&m);
	}
}

int main(int argc, char *argv[])
{    
	int num_threads = NUM_THREADS, queue_size = QUEUE_SIZE;

	//create the threads array:
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
    //initialize the queue:
    shared_queue = initQ(queue_size);

    //start all of the threads:
    for(int i = 0; i < num_threads; ++i){
        pthread_create(threads+i, NULL, consumeRequests, NULL);
    }

	produceRequests(argc, argv);

	for (int i = 0; i < num_threads; ++i) {
		pthread_join(threads[i], NULL);
    }
    //free the threads array:
    free(threads);
    //destroy the queue:
    destroyQ(shared_queue);
    return 0;
}


    


 

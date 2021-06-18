#include "segel.h"
#include "request.h"
#include <pthread.h>
#include "Queue.h"
#include <string.h>
#include <assert.h>


// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


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

void printReqFd(void* location){
    request* req_ptr = (request*)location;
    printf("%d ", req_ptr->connfd);
}

#define SHOWQ(self) DB(printf(self)); DB(printf(": queue status: ")); \
    DB(doEachQ(shared_queue, printReqFd)); DB(printf("\n"));

request makeRequest(int connfd, double arrival){
    request res;
    res.connfd = connfd;
    res.arrival = arrival;
    return res;
}

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

void* consumeRequests(void* arg_tid)
{
    ThreadData thread_data;
    thread_data.id = (intptr_t)arg_tid;
    thread_data.num_http_handled = 0;
    thread_data.num_dynamic_handled = 0;
    thread_data.num_static_handled = 0;

    while (1) {
        pthread_mutex_lock(&m);
        while (emptyQ(shared_queue)) {
            pthread_cond_wait(&empty_cond, &m);
        }

        SHOWQ("consumeRequests: about to dequeue");
        request req = dequeueQ(shared_queue); //critical code
        SHOWQ("consumeRequests: finished dequeuing");
        req.dispatch = Gettimeofday();
        req.dispatch -= req.arrival;

        pthread_cond_signal(&full_cond);
        
        pthread_mutex_unlock(&m);

		++thread_data.num_http_handled;
        //we might have to move the row about up, depending on if we are expected to return the number
        // of requests handled including the one currently being handled or without including the current one. 
		requestHandle(req, &thread_data);
        Close(req.connfd);
    }
    return NULL;
}

void closeRequestAt(void* location){
    request* req_ptr = (request*)location;
    DB(printf("closeRequestAt: closing request with connfd: %d.\n", req_ptr->connfd));
    Close(req_ptr->connfd);
    DB(printf("closeRequestAt: finished closing request.\n"));
}

void produceRequests(int port, OverloadPolicy policy)
{
	int listenfd, connfd, clientlen;
    struct sockaddr_in clientaddr;
    
    listenfd = Open_listenfd(port);

    while (true) {	
		clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		double arrival_time = Gettimeofday();

		pthread_mutex_lock(&m);
        
        if(policy == DROP_TAIL && fullQ(shared_queue)){
            pthread_mutex_unlock(&m);
            Close(connfd);
            continue;
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
                Close(dequeueQ(shared_queue).connfd);
                break;
            case DROP_RAND:
                //a random 25% of the items in the queue are dropped:
                SHOWQ("produceRequests: about to execute rand drop");
                dropRandQuarter(shared_queue, closeRequestAt);
                SHOWQ("produceRequests: finished executing rand drop");
                break;
            }
        }

        while(fullQ(shared_queue))
            pthread_cond_wait(&full_cond, &m);

        SHOWQ("produceRequests: about to add into queue")
		enqueueQ(shared_queue, makeRequest(connfd, arrival_time)); //critical code!  (>_<)
        SHOWQ("produceRequests: finished adding into queue")
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
        pthread_create(threads+i, NULL, consumeRequests, (void*)(intptr_t)i);
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


    


 

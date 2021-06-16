#include "segel.h"
#include "request.h"
#include <pthread.h>
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

IntQueue* todo_fds;
pthread_t* threads;

// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
}

void* slaveAway(void*){
	
		
		
	return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, argc, argv);
    
    todo_fds = intQueueInit();
	
		
    // 
    // HW3: Create some threads...
    //
	threads = (pthread_t*)malloc(sizeof(pthread_t)*NUM_THREADS);
	for(int i = 0; i < NUM_THREADS; ++i){
		pthread_create(threads+i, NULL, slaveAway, NULL);
	}

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	todo_fds->append(connfd);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	
	//OLD IMPL: 
	//	requestHandle(connfd);
	//	Close(connfd);
    }

}


    


 

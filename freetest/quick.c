#include <stdio.h>
#include <stdlib.h>
#include "../webserver-files/Queue.h"
#include "assert.h"

request mk(int connfd){
	request res;
	res.connfd = connfd;
	return res;
}

void test1(){
	Queue* q = initQ(3);
	assert(emptyQ(q));
	
	enqueueQ(q, mk(5));
	enqueueQ(q, mk(1));
	assert(!emptyQ(q));
	assert((dequeueQ(q).connfd == 5));
	assert(!fullQ(q));
	
	enqueueQ(q, mk(4));
	enqueueQ(q,mk(3));
	assert(fullQ(q));

	assert((dequeueQ(q).connfd == 1));
	assert((dequeueQ(q).connfd == 4));
	
	assert(!emptyQ(q));
	assert(!fullQ(q));
	
	assert((dequeueQ(q).connfd == 3));
	assert(emptyQ(q));

	destroyQ(q);
	printf("test1 success!\n");
}

void test2(){
	Queue* q = initQ(4);
	assert(emptyQ(q));
	enqueueQ(q, mk(200));
	enqueueQ(q, mk(100));
	enqueueQ(q, mk(50));
	enqueueQ(q, mk(25));
	assert(fullQ(q));
	assert((dequeueQ(q).connfd == 200));
	assert((dequeueQ(q).connfd == 100));
	assert((dequeueQ(q).connfd == 50));
	assert((dequeueQ(q).connfd == 25));
	assert(emptyQ(q));

	enqueueQ(q, mk(10));
	enqueueQ(q, mk(20));
	assert((dequeueQ(q).connfd == 10));
	enqueueQ(q, mk(30));
	enqueueQ(q, mk(40));
	assert((dequeueQ(q).connfd == 20));
	destroyQ(q);
	
	printf("test2 success!\n");
}

#define DOPRINTS 0
#define PRINTQ(queue) if(DOPRINTS) printQ(queue);
void test3(){
	srand(30);
	Queue* q = initQ(4);
	assert(emptyQ(q));
	enqueueQ(q, mk(200));
	enqueueQ(q, mk(100));
	enqueueQ(q, mk(50));
	enqueueQ(q, mk(25));
	PRINTQ(q);
	dropRandQuarter(q);
	PRINTQ(q);
	enqueueQ(q, mk(1));
	assert(fullQ(q));
	destroyQ(q);

	Queue* p = initQ(3);
	enqueueQ(p, mk(2));
	PRINTQ(p);
	dropRandQuarter(p);
	PRINTQ(p);
	assert(emptyQ(p));
	enqueueQ(p, mk(1));
	enqueueQ(p, mk(2));
	enqueueQ(p, mk(3));
	PRINTQ(p);
	dropRandQuarter(p);
	PRINTQ(p);
	enqueueQ(p, mk(4));
	assert(fullQ(q));
	printf("test3 success!\n");
}

int main(){
	test1();
	test2();
	test3();

	return 0;
}



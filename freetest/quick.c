#include <stdio.h>
#include <stdlib.h>
#include "../webserver-files/Queue.h"
#include "assert.h"

void test1(){
	Queue* q = initQ(3);
	assert(emptyQ(q));
	
	enqueueQ(q, 5);
	enqueueQ(q, 1);
	assert(!emptyQ(q));
	assert(dequeueQ(q) == 5);
	assert(!fullQ(q));
	
	enqueueQ(q, 4);
	enqueueQ(q,3);
	assert(fullQ(q));

	assert(dequeueQ(q) == 1);
	assert(dequeueQ(q) == 4);
	
	assert(!emptyQ(q));
	assert(!fullQ(q));
	
	assert(dequeueQ(q) == 3);
	assert(emptyQ(q));

	destroyQ(q);
	printf("test1 success!\n");
}

void test2(){
	static const int numq = 10;
	Queue* q = initQ(4);
	assert(emptyQ(q));
	enqueueQ(q, 200);
	enqueueQ(q, 100);
	enqueueQ(q, 50);
	enqueueQ(q, 25);
	assert(fullQ(q));
	assert(dequeueQ(q) == 200);
	assert(dequeueQ(q) == 100);
	assert(dequeueQ(q) == 50);
	assert(dequeueQ(q) == 25);
	assert(emptyQ(q));

	enqueueQ(q, 10);
	enqueueQ(q, 20);
	assert(dequeueQ(q) == 10);
	enqueueQ(q, 30);
	enqueueQ(q, 40);
	assert(dequeueQ(q) == 20);
	destroyQ(q);
	
	printf("test2 success!\n");
}

int main(){
	test1();
	test2();

	return 0;
}



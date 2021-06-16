#include <stdio.h>
#include <stdlib.h>
#include "../webserver-files/Queue.h"
#include "assert.h"

int main(){
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
	return 0;
}



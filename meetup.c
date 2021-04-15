//Julia Putko V00889506
/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

 
static resource_t codeword;
pthread_cond_t BarrierQueue;
pthread_mutex_t m;
int count;		//number of BFs in each group
int generation; 
int size; 		//size of groups
int order;

void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

    init_resource(&codeword, "codeword");
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&BarrierQueue, NULL);

    order = mf; 	//MEET_FIRST(1) or MEET_LAST (0)
    size = n;		//size of groups is given by n 

    count = 0;		//initialize count to 0
    generation = 0;	//initialize generation to 0
	
   
}


void join_meetup(char *value, int len) {
   // printf("NOTHING IMPLEMENTED YET FOR join_meetup\n");
	
    pthread_mutex_lock(&m);
    count++; 		//increment count 

    if ((order == MEET_FIRST) && (count ==1)){	//if MEET_FIRST, allow first BF to write   
    	write_resource(&codeword, value, len);
    }
    if ((order == MEET_LAST) && (count == size)){	//if MEET_LAST, allow last BF to write 
    	write_resource(&codeword, value, len);
    }

    if (count < size){				// if number of BF is not yet the same as the group size 
    	int my_generation = generation; 	//set mygen equal to gen, so that genis not overwritten 
	while (my_generation == generation){	//while they are equal, wait 
		pthread_cond_wait(&BarrierQueue, &m);
	}
	read_resource(&codeword, value, len);	//next BF can read
       //when the number of BFs that have read equals the group size, we can release the wait
    }
    else{			//count == size, so enough BFs have now arrived  
    	count = 0;		//reset the count back to 0 (for the next batch)
	generation++;			//increment generation
	read_resource(&codeword, value, len);	//read 
	pthread_cond_broadcast(&BarrierQueue);	//realease wait 
    }
    pthread_mutex_unlock(&m);		//unlock mutex 

}

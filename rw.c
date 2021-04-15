//Julia Putko V00889506
/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"


/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

resource_t data;
int readers;
int writers;
 
sem_t mutex;
sem_t write;


void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
	init_resource(&data, "data");
	sem_init(&write, 0, 1);
	sem_init(&mutex, 0, 1);	
}
//from littlebookofsemaphores.pdf
void rw_read(char *value, int len) {
//	printf("NOTHING IMPLEMENTED YET FOR rw_read\n");
	while(1){
		sem_wait(&mutex);   	//readers is initially 0,
		readers++; 		 //after increment, readers is 1
		if (readers == 1){  	//if value of readers is 1 wait on write 
    			sem_wait(&write);
		}
    						//now, since the read block is halted, the write block will be executed
    		sem_post(&mutex);  			//perform read
   						 //critical section 
    		read_resource(&data, value, len);
    		sem_wait(&mutex);
    		readers--;
    		if (readers == 0){
       			sem_post(&write);
    		}	
    		sem_post(&mutex);
	}	
}
//from littlebookofsemaphores.pdf
void rw_write(char *value, int len) {
	while(1){
    
    		sem_wait(&write); 
		write_resource(&data, value, len);
		sem_post(&write); 
	}
}

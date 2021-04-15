### Julia Putko V00889506 
# CSC360 Assignment2


## Readers/Writers using Pthreads

* create a buffer to hold label for resource
* initialize semaphores 

### rw_read

* perform a wait on sem_t mutex
* readers is initially 0
* after increment of readers, readers becomes 1
* this will make sure that when there is a reader, there may not be any writers 
	* wait on sem_t write 
* release wait on mutex because there are no readers 
* read value from data 
* decerement readers (goes from 1 to 0)
* now if readers is 0 (indicating that there are no more readers in the CS)
	* release wait on write
* release the wait on mutex 

### rw_write

* wait on write
* write value to data 
* no other write can enter into the CS while there is already one writer
* release wait on write
* other writers must wait until the wait is released for them to access the CS

## Meetup Using Threads
 
* initialize variables and resource_t codeword, mutex, and conditon variable 

### join_meetup

* if the meet order is MEET_FIRST, then the BobaFett (BF) receiving write access will be the first BF to arrive, hence count has to be equal to 1 
* if the meet order is MEET_LAST, then the BF receiving write access will be the last BF to arrive, hence count has to be equal to size (the size of the groups)
	* when these conditions are satisfied then this thread is able to write the codeword 
* when count is less than the size a thread, not all the BF of the group have arrived yet and the BF is given access to read the codeword. 
* my_generation is set to generation so that each thread has it's own copy of generation 
* if my_generation is equal to generation, then we know that we are still operating within the current group of BFs and whatever thread that is trying to come in will wait 
* the BFs in the current generation can read the codeword
* the else statement indicates that the count  == to the size of the group, so we know that the amount of BFs that have arrived is now enough to allow the next generation of Threads to come through
* therefore, we reset the count back to 0, increment generation, allow the last BF to read, and release the wait on the next set of threads
* finally, we unlock our initial lock on the mutex 

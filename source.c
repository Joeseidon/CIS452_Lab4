/*******************************************************************************
This mini-programming assignment simulates the thread execution manifested by a
fileserver process. It's an exercise of creating and handling threads, and
communicating results to some controller-thread.

@author   - Joseph Cutino, Brendon Murthum
@version  - Winter 2018

Requirements:
  - When terminated, your program should cleanup as appropriate and shutdown
    gracefully.
  - The Dispatch-Thread should:
    - Input a string from a user (simulating the name of a file to access).
    - Spawn a worker thread and communicate to it the filename entered.
    - Immediately repeat the input/spawn sequence (i.e. prepate to accept a new
      file request).
  - Each Worker-Thread should:
    - Obtain the simulated filename from the dispatcher.
    - Sleep for a certain amount of time, simulating the time spent performing a
      file access:
      - With 80% probability, sleep for 1 second. This simulates the scenario
        that the Worker thread has found the desired file in the disk cache and
        serves it up quickly.
      - With 20% probability, sleep for 7-10 seconds (randomly). This simulates
        the scenario that the worker thread has not found the requested file in
        the disk cache and must wait while it is read in from the hard drive.
      - Wake up, print a diagnostic message that includes the name of the file
        accessed, terminate.
  - In addition to being correct, your program should be efficient and should
    execute in parallel. Remember that threads share data -- as a rule, all
    multi-threaded programs should be carefully scrutinized for potential race
    conditions or "data clobbering".

Extra-Credit:
  - Upon termination, report the average file access time.
    - Recall the classroom discussion of concurrent access to a data object. To
      correctly accumulate the individual thread time values, you will need to
      ensure that the summation operation is exclusive (i.e. one at a time), so
      that executing threads do not overwrite each other.
    - Hint: this process is called "mutual exclusion" and the object needed to
      implement it is called a "mutex".
*******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_THREADS 10
#define MAX_FILE_CHARS 256

// SIGINT handler

/* Global Variables Accessible to Threads... */

int worker_count = 0;     // Worker threads created.
double time_average=0.0;      // Average execution time.
double total_time = 0.0;    // Total accumulated time by threads.
int time_additions = 0;
int time_lock = 0;            // Mutex: prevent threads from overwriting total_time.
pthread_t threads[MAX_THREADS];
int thread_open[] = {0,0,0,0,0,0,0,0,0,0};
int main_running = 1;

/* Initializing signal-handler functions. */

void closeSignalHandler(int);
void signalOneHandler(int);
void mainCloseSignalHandler(int);
void *lookToFile(void*);

int main() {
    // Assign Signal Handler
    //signal (SIGUSR1, signalOneHandler);
	signal (SIGINT, mainCloseSignalHandler);
	

    // Seed the random function for all future threads.
    srand(time(NULL));

    // Initiate a filename string.
    //int max_file_chars = 100;
    char filename[MAX_FILE_CHARS];
	
	pthread_attr_t attr;
	// set thread detachstate attribute to DETACHED 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    // This should loop (always accepting new files and making new threads).
    while (main_running) {
        sleep(0.5);
        printf("Enter a filename: ");
        
        // Waits for user input of filename OR "CTRL-C".
        fgets(filename, MAX_FILE_CHARS, stdin);

        //pthread_t fileSearcherThread;  // Thread ID Holder.
        int status;                    // Captures any error code.

        // Create and start a thread executing the "lookToFile()" function.
        if ((status = pthread_create(&threads[worker_count], &attr, lookToFile, filename)) != 0) {
            fprintf(stderr, "Thread Create Error %d: %s\n",
            status, strerror(status));
            exit(1);
        }
        else {
            // Success in creating thread. Increment.
            thread_open[worker_count]=1;
			worker_count++;
			
        }
    }
	
	return 0;
}

// Worker thread function here
void *lookToFile(void *arg)
{
    // Initiate a filename string for this thread's stack.
    char newFilename[MAX_FILE_CHARS];
    //newFilename = arg;
	strcpy(newFilename,(char*)arg);

    // Detach the child thread to handle processor. Parent will not join().
    /*int didDetach;
    didDetach = pthread_detach(pthread_self());
    if (didDetach != 0) {
        printf("Thread Did Not Detach\n");
    }*/

    // Sleep a semi-random amount of time.
    int probabilityRand = (rand() % 10) + 1; // Generate random number 1-10.
    int randTime;
    if (probabilityRand < 9) {
        randTime = (rand() % 4) + 7;         // Gen. random number 7-10.
    } else {
        randTime = 1;                        // Gen. "random number" of 1.
    }
    //signal (SIGINT, closeSignalHandler);     // Closes the process "mid-search".
    sleep(randTime);

    printf("\nFound File: %s", newFilename);

    // Loop as thread waits in line to add the timer-value to total global time.
        // Add "randTime" to some "globalTime"
		
	while(time_lock)
		/*Wait until this thread has access to time data*/;
	time_lock = 1; //lock to prevent modification by other threads
	total_time += randTime;
	time_additions++;
	time_average = (total_time/(time_additions*1.0));
	time_lock = 0; //unlock for other threads to use
	

    // Exit this thread without returning an argument. Parent is not waiting.
    //return arg;
	int i;
	for(i=0;i<MAX_THREADS;i++){
		if(pthread_equal(pthread_self(),threads[i])){
			thread_open[i]=0;
			break;
		}
	}
	
	//pthread_exit(0);
	return arg;
}

// Dispatch-thread, smooth-exit on user's "CTRL-C"
void mainCloseSignalHandler (int sigNum) {
	
	printf ("\nTotal Requests Serviced: %d\n", time_additions);
	printf ("Total Access Time: %f\n", total_time);
	printf ("Average Access Time: %f\n", time_average);
	
    printf ("Closing Active Child Threads...\n");
	int error;
	int i;
	for(i=0; i<worker_count; i++){
		if(thread_open[i]){
			error = pthread_cancel(threads[i]);
			if(error != ESRCH){
				printf("Closing Thread %d\n",i);
			}
		}
	}
	
    printf ("\nMain thread closing...\n");
	
	main_running = 0; //This will cancel while loop in main, causing exit
	
	//exit(0); //required to exit blocking user input command 
	
	return;
}




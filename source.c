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

// SIGINT handler

/* Global Variables Accessible to Threads... */

int file_count;       // Files "opened".
double time_average;  // Average execution time.
double total_time;    // Total accumulated time by threads.
int execution_time;   // Used by threads to calculate stop - start time
int time_lock;	      //mutex: prevent threads from overrighting execution_time

void main(){
    // Assign Signal Handler
    signal (SIGUSR1, signalOneHandler);

    // Seed the random function for all future threads.
    srand(time(NULL));

    // Initiate a filename string.
    int max_file_chars = 100;
    char filename[max_file_chars];

	// This should loop (always accepting new files and making new threads).
	while (true) {
	    printf("Enter a filename:");
	    signal (SIGINT, mainCloseSignalHandler);
	    gets(filename);  // Waits for user input of filename OR "CTRL-C".
	    printf("\n");	    
	    
	    pthread_t fileSearcherThread;  // Thread ID Holder.
	    int status;                    // Captures any error code.
	    
	    // Create and start a thread executing the "lookToFile()" function.
	    if ((status = pthread_create(&thread1, NULL, 
	                                 do_greeting, filename)) != 0) {
	        fprintf(stderr, "Thread Create Error %d: %s\n",
		    status, strerror(status));
	        exit(1);
	    }
	}
	
	// This should never be called.
	return 0;
}

//worker thread function here
void *lookToFile(void *arg)
{
    // Detach the child thread to handle processor. Parent will not join().
    int didDetach;
    didDetach = pthread_detach (pthread_self());
    if (didDetach == 0) {
        printf("  Thread Detached!\n");
    }

    // Sleep a semi-random amount of time.
    int probabilityRand = (rand() % 10) + 1; // Generate random number 1-10.
    int randTime;
    if (probabilityRand < 9) {
        randTime = (rand() % 4) + 7;         // Gen. random number 7-10.
    } else {
        randTime = 1;                        // Gen. "random number" of 1.
    }
    signal (SIGINT, closeSignalHandler);     // Closes the process "mid-search".
    sleep(randTime);
    
    printf("  Found File: \"%s\"\n", (char)arg );
    
    // Loop as thread waits in line to add the timer-value to total global time.
        // Add "randTime" to some "globalTime"
    
    // Exit this thread without returning an argument. Parent is not waiting.
    pthread_exit();
}

// SIGNINT handler here
void signalOneHandler(int sigNum){
    printf("Received a SIGUSR1 signal\n");
    fflush(stdout);

    // Re-esstablish Signal Handler
    signal (SIGUSR1, signalOneHandler);
    return;
}

// Worker-threads, smooth-exit on user's "CTRL-C"
void closeSignalHandler (int sigNum) {
    
    printf ("Close-Signal received!\n");
    pthread_exit();
}

// Dispatch-thread, smooth-exit on user's "CTRL-C"
void mainCloseSignalHandler (int sigNum) {
    printf ("Main thread closing...\n");
    
    // Print stats
      // Total number of file requests received/serviced
      // EXTRA CREDIT: report average file access time
	  // HINT: use a mutex

	  // Perform cleanup and shutdown gracefully
    
    // Have the main thread close itself.
    exit(1);
}




/*
*LAB 4
*
*
*
*/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//SIGINT handler

void main(){

	//estabilsh SIGINT handler 


	//This should be a continuous loop (always accepting new files and making new threads)

		//user input : simulating the name of a file to access

		//spawn a worker thread (pass it the filename entered by the user)

	//end loop (continue until ctrl-c entered by user)

	
	//print stats
		//total number of file requests received/serviced

		//EXTRA CREDIT: report average file access time
			//HINT: use a mutex

	//perform cleanup and shutdown gracefully

	return 0;
}

//worker thread function here

	//obtain the simulated filename (cast void * input)
	
	//sleep for random time (simulate file access)
		//use probability specified 

	//print a diagnostic message that includes the file name

	//terminate


//SIGNIN handler here



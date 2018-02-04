# CIS452 - Operating Systems - Lab 4

This mini-programming assignment simulates the thread execution manifested by a
fileserver process. It's an exercise of creating and handling threads, and
communicating results to some controller-thread.

**Authors**   - Joseph Cutino, Brendon Murthum
**Version**  - Winter 2018

**Requirements:**
  - When terminated, your program should cleanup as appropriate and shutdown     gracefully.
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
    
**Extra-Credit:**
  - Upon termination, report the average file access time.
    - Recall the classroom discussion of concurrent access to a data object. To 
      correctly accumulate the individual thread time values, you will need to 
      ensure that the summation operation is exclusive (i.e. one at a time), so 
      that executing threads do not overwrite each other.
    - Hint: this process is called "mutual exclusion" and the object needed to 
      implement it is called a "mutex".

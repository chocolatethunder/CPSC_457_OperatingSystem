/**********************************************
Last Name:  Tomar
First Name:  Saurabh
Student ID: --
Course: CPSC 457
Tutorial Section: T02
Assignment: 3
Question: 7

File name: scheduler.cpp

This program simulates two scheduler algorithms - round robin with quantum slice
and shortest job first. Run the program for usage options.

PLEASE REMOVE ANY UNNECESSARY WHITE SPACE AND WHITE SPACE LINES 
AT THE END OF THE CONFIG FILE. 

**********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <array>

using namespace std;

// id - arrival - burst - wait 

int runrr(vector<array<int,4>> procs, int quantum) {
 
 deque<int> procqueue; 
 int timestep = 0;
 int runslice = quantum;
 int currpid = -1; // -1 means no jobs waiting 
 int nextpid = -1;
 int jobs = procs.size();

 printf("Time \t");
 for (int j =0; j < procs.size(); j++) {
  printf("P%d \t",j);
 }
 printf("\n-------------------------------------------\n"); 

 while(jobs > 0) {
  
  printf("%2d\t",timestep);

  for (int i=0; i < procs.size(); i++) {
   
   if (procs[i][1] == timestep) {
     procqueue.push_back(procs[i][0]);
   }
   
   // if job queue is not empty
   if (!procqueue.empty()) { 
     // no process running
     if (currpid == -1) {
       currpid = procqueue.front();
       procqueue.pop_front();
       procs[currpid][2]--;	// decrement burst
       runslice--;		// decrement timeslice
       printf(".\t");
     } else {
     // some process running
       // if current process
       if (currpid == procs[i][0]) {
         runslice--;
         if (procs[currpid][2] > 1) {
          printf(".\t");
          procs[currpid][2]--;
         } else if (procs[currpid][2] == 1) {
          // this is the last burst queue the next job
          jobs--;
          printf(".\t");
          procs[currpid][2]--;
          // if there is a next job waiting load it
          if (!procqueue.empty()) {
            nextpid = procqueue.front();
            procqueue.pop_front();
          } else {
 	    // reset
            currpid = -1;
          }
          runslice = quantum;
         }
       } else if (timestep >= procs[i][1] &&
                  procs[i][2] > 0 ) {
         printf("+\t");
         procs[i][3]++;
       // process waiting
       } else {
       // no process waiting
         printf("\t");
       }
     }

   } else {
   // nothing in job queue but there maybe a process running
      // if there is a job running
      if (currpid != -1) {

       // if current process
       if (currpid == procs[i][0]) {
         runslice--;
         if (procs[currpid][2] > 1) {
          printf(".\t");
          procs[currpid][2]--;
         } else if (procs[currpid][2] == 1) {
          // this is the last burst queue the next job
          printf(".\t");
          jobs--;
          procs[currpid][2]--;
          // if there is a next job waiting load it
          if (!procqueue.empty()) {
            nextpid = procqueue.front();
            procqueue.pop_front();
          } else {
            // reset
            currpid = -1;
          }
          runslice = quantum;
         }
       } else if (timestep >= procs[i][1] &&
                  procs[i][2] > 0 ) {
         printf("+\t");
         procs[i][3]++;
       // process waiting
       } else {
       // no process waiting
         printf("\t");
       }

      } else {
       printf("\t");
      }
   }
   
  } // for loop
  
  if (nextpid != -1) {
    currpid = nextpid;
    nextpid = -1;
  }
   
  // check time slice  
  if (runslice < 1) {
    // is there still something to do in current process?
    if (procs[currpid][2] > 0) {
      // requeue the current proc
      procqueue.push_back(procs[currpid][0]);
    } else {
      // job is finished
      jobs--;
      //printf("-%d",jobs);
    }
    // load the next job in queue
    currpid = procqueue.front();
    procqueue.pop_front();
    runslice = quantum;
  }

  // move to next line
  printf("\n");
 
  // move to next time unit
  timestep++;

 } // while loop 
  
 printf("-------------------------------------------\n"); 
 float waittime;
 for (int m = 0; m < procs.size(); m++) {
   printf("P%d waited %d sec.\n",m,procs[m][3]);
   waittime += procs[m][3];
 }
 
 printf("Average waiting time = %.3f sec.\n", waittime/procs.size());
 
 return 0;
}

int runsjf(vector<array<int,4>> procs) {
 
 deque<int> procqueue; 
 int timestep = 0;
 int currpid = -1; // -1 means no jobs waiting 
 int jobs = procs.size();
 int shortestjob = 100;

 printf("Time \t");
 for (int j =0; j < procs.size(); j++) {
  printf("P%d \t",j);
 }
 printf("\n-------------------------------------------\n"); 

 while(jobs > 0) {
  
  printf("%2d\t",timestep);

  for (int i=0; i < procs.size(); i++) {
   
   if (procs[i][1] == timestep) {
     if (procs[i][2] < shortestjob) {
      procqueue.push_front(procs[i][0]);
      shortestjob = procs[i][2];
     } else {
      procqueue.push_back(procs[i][0]);
     }
   }
   
   // if job queue is not empty
   if (!procqueue.empty()) { 
     // no process running
     if (currpid == -1) {
       currpid = procqueue.front();
       procqueue.pop_front();
       procs[currpid][2]--;	// decrement burst
       printf(".\t");
     } else {
     // some process running
       // if current process
       if (currpid == procs[i][0]) {
         if (procs[currpid][2] > 1) {
          printf(".\t");
          procs[currpid][2]--;
         } else if (procs[currpid][2] == 1) {
          // this is the last burst queue the next job
          jobs--;
          printf(".\t");
          procs[currpid][2]--;
          // if there is a next job waiting load it
          if (!procqueue.empty()) {
            currpid = procqueue.front();
            procqueue.pop_front();
          } else {
 	    // reset
            currpid = -1;
          }
         }
       } else if (timestep >= procs[i][1] &&
                  procs[i][2] > 0 ) {
         printf("+\t");
         procs[i][3]++;
       // process waiting
       } else {
       // no process waiting
         printf("\t");
       }
     }

   } else {
   // nothing in job queue but there maybe a process running
      // if there is a job running
      if (currpid != -1) {

       // if current process
       if (currpid == procs[i][0]) {
         if (procs[currpid][2] > 1) {
          printf(".\t");
          procs[currpid][2]--;
         } else if (procs[currpid][2] == 1) {
          // this is the last burst queue the next job
          printf(".\t");
          jobs--;
          procs[currpid][2]--;
          // if there is a next job waiting load it
          if (!procqueue.empty()) {
            currpid = procqueue.front();
            procqueue.pop_front();
          } else {
            // reset
            currpid = -1;
          }
         }
       } else if (timestep >= procs[i][1] &&
                  procs[i][2] > 0 ) {
         printf("+\t");
         procs[i][3]++;
       // process waiting
       } else {
       // no process waiting
         printf("\t");
       }

      } else {
       printf("\t");
      }
   }
   
  } // for loop
  
  // move to next line
  printf("\n");
 
  // move to next time unit
  timestep++;

 } // while loop 
  
 printf("-------------------------------------------\n"); 
 float waittime;
 for (int m = 0; m < procs.size(); m++) {
   printf("P%d waited %d sec.\n",m,procs[m][3]);
   waittime += procs[m][3];
 }
 
 printf("Average waiting time = %.3f sec.\n", waittime/procs.size());
 
 return 0;
}


int main( int argc, char ** argv)
{
 
 /// parse command line arguments
 if(argc != 3 && argc != 4) {
  printf("\nUsage: scheduler [filename] [algorithm] [time quantum]\n");
  printf("algorithm \t [RR|rr] Round Robin \t [sjf|SJF] Shortest Job First\n");
  printf("time quantum \t [number] Time quantum for RR algorithm in seconds.\n\n");
  exit(-1);
 }
 
 // Capture arguments
 int quantum = 0;
 FILE *fp = fopen(argv[1],"r");
 string algo(argv[2]); 
 if (argc == 4) quantum = atoi(argv[3]);
 
 // Error check to make sure time quantum is there when RR is selected
 if (!algo.compare("RR") || !algo.compare("rr")) {
  if (argc != 4 || quantum < 1) {
   printf("\nRound Robin algorithm requires a time quantum in seconds.\n");
   printf("\nUsage: scheduler [filename] [algorithm] [time quantum]\n");
   printf("algorithm \t [RR|rr] Round Robin \t [sjf|SJF] Shortest Job First\n");
   printf("time quantum \t [number] Time quantum for RR algorithm in seconds.\n\n");
   exit(-1);
  }
 } else if (!algo.compare("SJF") || !algo.compare("sjf")) {
  if (argc != 3) {
   printf("\nShortest Job First algorithm does not require a time quantum.\n");
   printf("\nUsage: scheduler [filename] [algorithm] [time quantum]\n");
   printf("algorithm \t [RR|rr] Round Robin \t [sjf|SJF] Shortest Job First\n");
   printf("time quantum \t [number] Time quantum for RR algorithm in seconds.\n\n");
   exit(-1);
  }
 } else {
   printf("\nInvalid algorithm type detected.\n");
   printf("\nUsage: scheduler [filename] [algorithm] [time quantum]\n");
   printf("algorithm \t [RR|rr] Round Robin \t [sjf|SJF] Shortest Job First\n");
   printf("time quantum \t [number] Time quantum for RR algorithm in seconds.\n\n");
   exit(-1);
 }
 
 // all good
 
 // id - arrival - burst - wait
 
 char buf[128];
 int arrival,burst;
 int i = 0;
 vector<array<int,4>> procs;  
 
 if(fp != NULL) {
  while(fgets(buf, sizeof(buf), fp)) {
   if (buf[0] != '\0' && buf[0] != '\n') {
    //printf("%s",buf);
    sscanf(buf, "%d %d", &arrival, &burst);
    if (0 <= arrival && arrival <= 100 && 1 <= burst && burst <= 100) {
     array<int, 4> proc = {i,arrival,burst,0};
     procs.push_back(proc);
     // debug
     // printf ("Pushing: %d %d %d %d\n", proc[0], proc[1], proc[2], proc[3]);
    } else {
     printf("Invalid data detected. Quiting program.\n");
     exit(-1);
    }
    i++;
   }
  }
  fclose(fp);

  // run the algos
  if (!algo.compare("RR") || !algo.compare("rr")) runrr(procs,quantum);
  if (!algo.compare("SJF") || !algo.compare("sjf")) runsjf(procs);  

 } else {
   printf("Uh oh! No file found.\n");
   exit(-1);
 } 

 return 0;
}

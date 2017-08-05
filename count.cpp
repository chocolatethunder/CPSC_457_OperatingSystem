/**********************************************
Last Name:  Tomar
First Name:  Saurabh
Student ID: --
Course: CPSC 457
Tutorial Section: T02
Assignment: 3
Question: 5

File name: countPrimes.cpp

This program uses multithreading to compute whether a given number 
ingested from a list in a text file or command line is prime or not.
The number threads needed to compute the sum is specified by the user.

Default number of threads is 1.

**********************************************/

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <signal.h>

using namespace std;

// Global vars
int nonPrimeFound = 0;

// Arguments to pass to threads
struct thread_args {
 int tid;
 int numthreads;
 int64_t num;
};

// Mutex to control the thread found lock
pthread_mutex_t lockrun;

// This function processes each chunk of a large
// prime divided by the isPrime function
void *splitScan(void * arguments)
{

  struct thread_args *args = (struct thread_args *)arguments;
  
  int64_t i = 5; 
  int64_t n = args->num;
  int64_t max = sqrt(n);

  // Search multiplier
  int64_t mult = args->tid;
  int threads = args->numthreads;  
  
  // start position of each thread as a product of 6
  int64_t startpos = (mult*6); // 6 12 18 24 
  // advancing amount for each thread based on number of threads
  int64_t next = (6 * (int64_t)threads); // 6 * 4 = 24
  
  // initial start point
  i += startpos;  

  while( i <= max && nonPrimeFound != 1) {
   
   if (n % i == 0 || n % (i+2) == 0) {
     // Send a signal to the other threads to stop them
     pthread_mutex_lock(&lockrun);
     nonPrimeFound = 1;
     pthread_mutex_unlock(&lockrun);
     pthread_exit(0);
   }
   
   // Otherwise continue testing
   i += next; // 30 36 42 48
  }
  
  // Thread clean up? What thread clean up. YOLO. 
  pthread_exit(0);

}

// Primality test, if n is prime, return 1, else return 0
// Divide and conquer
int isPrime(int64_t n, int nThreads)
{
 
 if( n <= 1) return 0; // small numbers are not primes
 if( n <= 3) return 1; // 2 and 3 are prime
 if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and
 int64_t i = 5;
  
 // Multithreading in da house
 
 if (n <= 100) {
  
  // Don't multithread anything below 10 
  int64_t max = sqrt(n); 
  while( i <= max) { 
    if (n % i == 0 || n % (i+2) == 0) return 0; 
    i += 6; 
  } 
  return 1; 
 
 } else {
  
  // Reset the found flag
  nonPrimeFound = 0;
  
  int status;
  pthread_t threads[nThreads];
  thread_args * thread_args_ptr = new thread_args[nThreads];
  
  // Create threads and pass the chunk start and stop to them
  for (int j = 0; j < nThreads; j++) { 
    
    thread_args_ptr[j].tid = j;
    thread_args_ptr[j].numthreads = nThreads;
    thread_args_ptr[j].num = n; 
    
    status = pthread_create(&threads[j], NULL, splitScan, &thread_args_ptr[j]);
    
    if(status != 0) {
       printf("Oops, pthread_create returned error code %d\n", status);
       exit(-1);
    } 
  
  }
  
  // Join all the threads so that main waits for them to finish
  int m;
  for (m=0; m < nThreads; m++) {
    pthread_join(threads[m], NULL);
  }
  
  // One of the threads found that some chunk was divisible 
  // by something other than 1 and it an flagged it
  if (nonPrimeFound == 0) {
    return 1;
  }

 }
 return 0;
}

int main( int argc, char ** argv)
{
 /// parse command line arguments
 int nThreads = 1;
 if( argc != 1 && argc != 2) {
  printf("Usage: count [nThreads]\n");
  exit(-1);
 }
 if( argc == 2) nThreads = atoi( argv[1]);
 /// handle invalid arguments
 if( nThreads < 1 || nThreads > 256) {
  printf("Bad arguments. 1 <= nThreads <= 256!\n");
  exit(-1);
 }
 
 /// count the primes
 printf("Counting primes using %d thread%s.\n", nThreads, nThreads == 1 ? "" : "s");
 
 int64_t count = 0;
 while( 1) {
  int64_t num;
  if( 1 != scanf("%ld", & num)) break;
  if(isPrime(num, nThreads)) count ++;
 }
 
/// report results
 printf("Found %ld primes.\n", count);
 return 0;
}

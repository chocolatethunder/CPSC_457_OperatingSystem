/**********************************************
Last Name:  Tomar
First Name:  Saurabh
Student ID: --
Course: CPSC 457
Tutorial Section: T02
Assignment: 2
Question: 8

File name: sum.cpp

This program uses multithreading to compute the sum of integers 
ingested from a list in a text file. The number threads needed 
to compute the sum is specified by the user. 

**********************************************/

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#define MAX_INTS 1000000

int num_threads, chunk_size;
int sum = 0;

pthread_mutex_t locksum;

// Parameter struct to pass to the thread_add
struct thread_args {
  int tid;
  int *data;
}; 

void *thread_add(void *arguments) {
  
  struct thread_args *args = (struct thread_args *)arguments;
  int t_id = args->tid;
  int *p = new int[chunk_size];
  p = args->data;
  int localsum = 0;
  
  // Add the local sum
  for (int i = 0; i < chunk_size; i++) {
    localsum += p[i];
  }
  
  // Print the local results
  printf("Thread %d: %d\n", t_id, localsum);
  
  // Lock and add to the global sum
  pthread_mutex_lock(&locksum);
  sum += localsum;
  pthread_mutex_unlock(&locksum);
  
  // Clean up
  fill_n(p,chunk_size,0);
  
  pthread_exit(0);

}

int main (int argc, char **argv) {
  // handle command line arguments
  if(argc != 3) {
    fprintf(stderr, "Usage: ./sum [file of int array] [number of threads].\n");
    exit(-1);
  }

  if (atoi(argv[2]) <= 0) { 
    fprintf(stderr, "Usage: ./sum [file of int array] [number of threads].\n");
    exit(-1);
  }

  num_threads = atoi(argv[2]);

  // Collect numbers from the data file
  vector<int> num_store;

  ifstream file(argv[1]);
  
  if(file) {
    int number;
    while( file >> number ) {
      num_store.push_back(number);
    }
  }
  
  // Calculate per thread workload 
  chunk_size = ((num_store.size() + num_threads - 1) / num_threads);  
  
  // Debug  
  // printf("Total:%d \t N_T: %d \t C_S: %d\n", num_store.size(), num_threads, chunk_size);
   
  // Create a workload array to be passed to threads
  int workload[num_threads][chunk_size];
  
  int j,k;
  int l = 0;

  for (j=0; j < num_threads; j++) {
     for (k=0; k < chunk_size; k++) {
       if (l < num_store.size()) {
          // Load the data 
          workload[j][k] = num_store[l];
       } else {
          // Load empty data if reached
          // the end of number storage
          workload[j][k] = 0;
       }
       l++;  
     }
  }  
  
  // Start thread publishing
  int i = 0;
  int status;
  
  pthread_t threads[num_threads];
  
  thread_args * thread_args_ptr = new thread_args[num_threads];  

  for (i = 0; i < num_threads; i++) {

    thread_args_ptr[i].tid = i;
    thread_args_ptr[i].data = workload[i];

    status = pthread_create(&threads[i], NULL, thread_add, &thread_args_ptr[i]);
    if(status != 0) {
       printf("Oops, pthread_create returned error code %d\n", status);
       exit(-1);
    }
  }
  
  // Join all the threads so that main waits for them to finish
  int m;
  for (m=0; m < num_threads; m++) {
    pthread_join(threads[m], NULL);
  }
  
  // Debug
  /*
  for (std::vector<int>::size_type i = 0; i != num.size(); i++) {
    cout << num[i] << endl;
  }
  */
   
  // Print the sum and exit
  printf("Sum = %d\n", sum);
  
  exit(0);  

  // Return success (Won't reach but good habit)
  return 0;

}


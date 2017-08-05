/*
 * pagesim.cpp
 *
 * Student Name: Saurabh Tomar
 * Student Number: --
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *
 * This program simulates various paging algorithms.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <array>
#include <vector>
#include <string>

using namespace std;

struct elem {
 int key;
 int val;
};

struct page {
 page(): used(0) {}
 int val;
 int used;
};

/* ========== Optimal Class =========== */

class Optimal {

private:
  elem *arr;
  int *frames;
  int loaded;
  int cap;
  int faults;
  vector<int>data;
  int time;

public:
  /* Initializing the size frames, the number of elements array
  * @param capacity Number of frames
  * @param nums The reference string in vector format 
  */
  Optimal(int capacity, vector<int> nums) {
    arr = new elem[cap];
    frames = new int[capacity];
    loaded = 0;
    faults = 0;
    time = 0;
    cap = capacity;
    fill_n(frames,capacity,-1);
    data = nums;
  }

  /* Stores the positions of the current frames in relation to when they occur in 
   * the reference strings. Compares these times of occurence and replaces at 
   * appropriate position.
   * @param value Value in the array of "stack". Needs to be same as key.   *
   */
  void set(int value) {
    time++;
    for (int i = 0; i < loaded; i ++) {
       if (frames[i] == value) {
          return;
       }
    }
    faults++;
    if (loaded == cap) {
	//bool found = false;
        int pos = 0;
        int max;
	bool emptyfound = false;
	
	// create a temp array holding the positions of the current frames in memory	    
	for (int m = 0; m < loaded; m++) {
	    arr[m].key = -1;
	    arr[m].val = -1;
	    for(std::vector<int>::size_type p = time; p != data.size(); p++) {
		if (frames[m] == data[p]) {
		    arr[m].key = p;
		    arr[m].val = p;
		    break;
		}
	    }
	}
	
	// if a position is found such that a frame will never be recalled in the
	// future then replace it.
	for (int n = 0; n < loaded; n++) {
	    if (arr[n].key == -1) {
		pos = n;
		emptyfound = true;
		break;	
	    }
	}

	// Otherwise check to see who has the most max position. i.e. wont be
	// used in the longest time and return the position value.
	if (!emptyfound) {
	    max = arr[0].key;
	    pos = 0;
	    for (int r = 1; r < loaded; r++) {
		if(arr[r].key > max) {
		    max = arr[r].key;
		    pos = r;
		}
	    }
	}
	// replace the position with the incoming value.
	frames[pos] = value;

    } else {
        frames[loaded] = value;
        loaded++;
    }
  }
  
  /* prints a human readable report
   */
  void printdata() {
    printf("Optimal:\n");
    printf("\t- frames: ");

    for(int i = 0; i < cap; i++) {
       printf("%d ",frames[i]);
    }
    printf("\n");
    printf("\t- page faults: %d\n", faults);
  }

};

/* ============  LRU Class =========== */

class LRU {

private:
  elem *arr;
  int *frames;
  int loaded;
  int cap;
  int faults;

public:
  /* Initializing the size frames, the number of elements array
  * @param capacity Number of frames 
  */
  LRU(int capacity) {
    arr = new elem[capacity];
    frames = new int[capacity];
    loaded = 0;
    faults = 0;
    cap = capacity;
    fill_n(frames,capacity,-1);
  }
  
  /* Element mover. Moves element to the end of the array
   * @param elemindex Index of the element to move to the end of frame array
   */
  void putback(int elemindex) {
    if (elemindex == loaded-1) {
      return;
    }
    elem current = arr[elemindex];
    for (int i = elemindex; i < loaded - 1; i++) {
       arr[i] = arr[i+1];
    }
    arr[loaded-1] = current;
  }
  /* Searches the current cue to see if a page fault has occured. Otherwise
   * moves the recently used item to the bottom of the stack of last used
   * items. Adjusts the frames so that they contain the appropriate replacement
   * of the replaced value with the new value.
   * @param key Key in the array of "stack". Needs to be same as value.
   * @param value Value in the array of "stack". Needs to be same as key.   *
   */
  void set(int key, int value) {
    for (int i = 0; i < loaded; i ++) {
       if (arr[i].key == key) {
	  arr[i].val = value;
          putback(i);
          return;
       }
    }
    faults++;
    int oldval = arr[0].val;
    if (loaded == cap) {
	for (int i = 0; i < loaded-1; i++) {
           arr[i] = arr[i+1];
        }
        arr[loaded-1].key = key;
	arr[loaded-1].val = value;
	LRU::frameadjust(oldval, value);
    } else {
	arr[loaded].key = key;
        arr[loaded].val = value;
        frames[loaded] = value;
        loaded++;
    }
  }
  
  /* Does the frame replacement so that the old value gets replaced in the new 
   * value in the right frame for display purposes later.
   * @param old Old value to be replaced
   * @param newval New value to be replaced with
   */
  void frameadjust(int old, int newval) {
    for(int i = 0; i < cap; i++) {
       if (frames[i] == old) {
          frames[i] = newval;
       }
    }
  }

  /* prints a human readable report
   */
  void printdata() {
    printf("LRU:\n");
    printf("\t- frames: ");

    for(int i = 0; i < cap; i++) {
       printf("%d ",frames[i]);
    }
    printf("\n");
    printf("\t- page faults: %d\n", faults);
  }

};

/* ================== Clock Class ================= */

class Clock {

private:
  page *frames;
  int cap;
  int loaded;
  int faults;
  int pointer;
  
public:

  /* Initializing the size frames, the number of elements array
  * @param capacity Number of frames 
  */
  Clock(int capacity) {
    frames = new page[capacity];
    faults = 0;
    cap = capacity;
    loaded = 0;
    pointer = 0;
  }
  
  /* This runs the Clock cache algorithm. 
   * @param value The next value in the reference string
   */
  void set(int value) {
    // Check if the value is already in the current set of 
    // loaded frames. 
    for (int k = 0; k < loaded; k++) {
       if (frames[k].val == value) {
          // If found and marked used exit. No page fault.
	  if (frames[k].used == 1) {
	     return;
          }
	  // If fouund and not marked used, mark it.
          if (frames[k].used == 0) {
	     frames[k].used = 1;
             return;
          }

       }

    } // for loop
    faults++;
    if (loaded == cap) {
       // Start moving the pointer through the frames
       while(1) {
	  // If the current value the pointer is pointing at 
	  // doesn't match set it to unused
          if(frames[pointer].val != value) {
	     frames[pointer].used = 0;
	  }
	  // Move the pointer
       	  pointer++;
   	  // Cycle the pointer back to the begining
      	  if (pointer%loaded == 0) pointer = 0;
 	  // If the next frame it encounters is an unused value
 	  // then replace this value with the new incoming value
 	  // and mark it used.
	  if (frames[pointer].used == 0) {
	     frames[pointer].val = value;
 	     frames[pointer].used = 1;
	     return;
  	  }
       }
    } else {
       // Frames are not completely full, keep loading and faulting.
       frames[loaded].val = value;
       frames[loaded].used = 1;
       loaded++;
    }

  }

  /* prints a human readable report
   */
  void printdata() {
    printf("Clock:\n");
    printf("\t- frames: ");

    for(int i = 0; i < cap; i++) {
       printf("%d ",frames[i]);
    }
    printf("\n");
    printf("\t- page faults: %d\n", faults);
  }

};

/* ================== Main ======================== */

int main( int argc, char ** argv)
{
 /// parse command line arguments
 int frames = 0;
 if( argc != 1 && argc != 2) {
  printf("Usage: count [frames] < [file containing simulation numbers]\n");
  exit(-1);
 }
 if( argc == 2) frames = atoi( argv[1]);
 /// handle invalid arguments
 if( frames < 1 || frames > 20) {
  printf("Bad arguments. 1 <= frames <= 20!\n");
  exit(-1);
 }
 
 vector<int> input;
 int64_t entries = 0;
 int num; 
 string line;

 getline(cin, line);
 istringstream iss(line);
 while(iss >> num) {
   
  if( num < 0 || num > 100) {
    printf("Bad number input. 1 <= frame number <= 100");
    exit(-1);
  } else {
    input.push_back(num);
  }
  entries++;
  if (entries > 5000) {
    printf("The reference string cannot contain more than 5000 entries");
    exit(-1);
  }

 }
 
 // process here
 
 // Load 
 Optimal * OptSim = new Optimal(frames, input);
 LRU * LRUSim = new LRU(frames);
 Clock * ClockSim = new Clock(frames);

 // Insert
 for(std::vector<int>::size_type i = 0; i != input.size(); i++) {
    OptSim -> set(input[i]);
    LRUSim -> set(input[i],input[i]);
    ClockSim -> set(input[i]);
 }

 // Output
 OptSim -> printdata();
 LRUSim -> printdata();
 ClockSim -> printdata(); 

 return 0;
}

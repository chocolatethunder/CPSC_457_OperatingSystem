/**********************************************
Last Name:  Tomar
First Name:  Saurabh
Student ID: --
Course: CPSC 457
Tutorial Section: T02
Assignment: 2
Question: 6

File name: scan.cpp

This program displays all the files ending with the first argument provided
and displays the number of entries specified in the second command line argument.

**********************************************/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

int match(char text[], char pattern[]) {
  int c, d, e, text_length, pattern_length, position = -1;
 
  text_length    = strlen(text);
  pattern_length = strlen(pattern);
  
  // If the pattern to search is bigger then the filename then skip 
  if (pattern_length > text_length) {
    return -1;
  }
  
  for (c = 0; c <= text_length - pattern_length; c++) {
    position = e = c;
    // Start finding the pattern
    for (d = 0; d < pattern_length; d++) {
      if (pattern[d] == text[e]) {
        e++;
      }
      else {
        break;
      }
    }
    // Check if the pattern is at the end of the string
    if (d == pattern_length) {
      if (position == (text_length - pattern_length - 1)) {
	return position;
      }
    }
  }
  return -1;
}

int main( int argc, char ** argv) {
  // handle command line arguments
  if(argc != 3) {
    fprintf(stderr, "Usage: ./scan [file ending in string] [number of results to display].\n");
    exit(-1);
  }

  if (atoi(argv[2]) < 0) { 
    fprintf(stderr, "Usage: ./scan [file ending in string] [number of results to display].\n");
    exit(-1);
  }
  
  // Get the list of files
  FILE * fp = popen( "find . -type f" , "r");
  if( fp == NULL) {
    perror("popen failed:");
    exit(-1);
  }

  // Read in all filenames but only store
  // only the ones that match the pattern
  char buff[MAX_FNAME_SIZE];
  int nFiles = 0;
  int head = atoi(argv[2]);
  char * files[MAX_FILES];
  while(fgets(buff,MAX_FNAME_SIZE,fp) ) {
    int len = strlen(buff) - 1;
    if (match (buff,argv[1]) != -1) {
    	files[nFiles] = strndup(buff,len);
    	nFiles ++;
    }
  }
  fclose(fp);

  // Get file sizes for each file and sum them up
  long long totalSize = 0;
  struct stat st;
  
  // Storage structure for file data
  struct fileDB {
    std::string name;
    int size;
    // Constructor
    fileDB(const std::string& s, int k) : name(s), size(k) {}
    // Override the > operator so it knows to use size as the sort parameter
    bool operator > (const fileDB& str) const {
       return (size > str.size);
    }
  }; 
  
  // Initialize vector
  std::vector <fileDB> entry;
 
  for(int i = 0 ; i < nFiles ; i ++ ) {
    if( 0 != stat(files[i],&st)) {
      perror("stat failed:");
      exit(-1);
    }
    // Store file into vector
    entry.push_back(fileDB(files[i], st.st_size));
  }
  
  // Sort the vector by size 
  std::sort(entry.begin(), entry.end(), greater<fileDB>());
  
  // Print out the sorted results
  for (std::vector<int>::size_type index = 0; (index != entry.size() && index < head); index++) {
    cout << entry[index].name << " " <<  entry[index].size << endl;
    // Save total size
    totalSize += entry[index].size;
  }
  // Print out the total size
  printf("Total size: %lld \n", totalSize);
  
  // Clean up
  for(int i = 0; i < nFiles ; i ++ ) {
    free(files[i]);
  }
  entry.clear();  
  
  // Return success
  return 0;

}


/**********************************************
Last Name:  Tomar
First Name:  Saurabh
Student ID: --
Course: CPSC 457
Tutorial Section: T02
Assignment: 1
Question: 5

File name: myCat.cpp
**********************************************/

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main (int argc, char * const argv[])
{
    // Get the file name
    string filename;

    if (argc != 2)
    {
        cout << "Usage: myCat <input file> " << endl;
        return -1;
    }
    else
    {
        filename = argv[1];
    }

    // Open the file
    ifstream infile;
    infile.open(filename.c_str());
    
	// store the contents of a file into a temp buffer
    stringstream buffer;
	buffer << infile.rdbuf();
	
	// output the contents of the buffer to standard output
	cout << buffer.str() << endl;	
    
    infile.close();
    
    return 0;
}

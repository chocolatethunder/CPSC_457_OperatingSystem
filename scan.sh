#!/bin/bash

#**********************************************
# Last Name:  Tomar
# First Name:  Saurabh
# Student ID: --
# Course: CPSC 457
# Tutorial Section: T02
# Assignment: 2
# Question: 5
#
# File name: scan.sh
#
# This bashscript prints a list of files that had the suffix provided by the first argument
# lists the amount of entries on the screen specified by the second argument.
#
# It does so by first finding all the files using the find command with -name flag which 
# it then  passes the disk utility du arguments to get the files size in bytes, through which it
# sorts the first column for the file size descending, finally getting passed to awk to 
# be properly formatted, summed, and displayed on the screen.  
#
#**********************************************

echo "$(find . -type f -name "*$1" | xargs du -b | sort -k 1 -n -r | head -n $2 | awk '{ total += $1 ; print $2 " " $1 }  END { print "Total size: " total }' )"

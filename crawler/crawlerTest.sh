#!/bin/bash
# File: 		crawlerTest.sh
#
# Description: 	Tests the executable ./crawler for all boundary cases, and cases of good output.
#		For good output, writes to stdout the number of files created, and a snippet of a sample file.
#
# Input: 	No input.
#
# Output: 	Outputs error messages for boundary cases, or the number of files created
# 		and a snippet of a sample file for good cases.
# 
# Pseudocode: 	The program goes through each boundary case and good case, providing a
# 		brief description of what was tested, what output should be expected,
# 		and then it prints the output.
# 	

filename=`date +"%a_%b_%d_%T_%Y"` # crawlerTestlog.$filename

# Boundary Cases
echo "BOUNDARY CASES." > crawlerTestlog.$filename
echo "An appropriate error message will be printed for each case." >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Case of not 3 arguments
echo "1." >> crawlerTestlog.$filename
echo "This tests for the case where the number of arguments passed is not 3." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Case of the seed URL having improper domain
echo "2." >> crawlerTestlog.$filename
echo "This tests for the case where the seed URL doesn't have the old-www domain." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://www.cs.dartmouth.edu/~cs50/tse/ ./data 0" >> crawlerTestlog.$filename

./crawler http://www.cs.dartmouth.edu/~cs50/tse/ ./data 0 >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Case of a nonexistent directory
echo "3." >> crawlerTestlog.$filename
echo "This tests for the case where the directory passed does not exist. './abcdef' does not exist on my machine." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./abcdef 0" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./abcdef 0 >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Case of invalid directory path format.
echo "4." >> crawlerTestlog.$filename
echo "This tests for the case where the directory format is wrong, ie there is a '/' at the end of the path." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data/ 0" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data/ 0 >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Case of an invalid number passed as depth
echo "5." >> crawlerTestlog.$filename
echo "This tests for the case where the depth passed is invalid, ie not an integer number." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1a" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1a >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename

# Case of the passed depth exceeding MAX DEPTH of 4.
echo "6." >> crawlerTestlog.$filename
echo "This tests for the case where the depth passed is greater than the MAX DEPTH of 4." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 5" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 5 >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename 


# Case of when the seed URL is invalid, ie dead link. 
echo "7." >> crawlerTestlog.$filename
echo "This tests for the case where the seed URL is a dead link, or throws other >400 errors." >> crawlerTestlog.$filename
echo "The url http://old-www.cs.dartmouth.edu/deadlink.html passed is a dead link." >> crawlerTestlog.$filename
echo "The program should throw an error." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/deadlink.html ./data 1" >> crawlerTestlog.$filename

./crawler http://old-www.cs.dartmouth.edu/deadlink.html ./data 1 >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Good output
echo "GOOD OUTPUTS." >> crawlerTestlog.$filename
echo "The last line of 'ls' will be shown to show how many files have been created." >> crawlerTestlog.$filename
echo "The first 4 lines of a sample file will also be printed to show how each created file should look like." >> crawlerTestlog.$filename
printf "\n" >> crawlerTestlog.$filename


# Depth 0
echo "8." >> crawlerTestlog.$filename
echo "This tests for depth 0." >> crawlerTestlog.$filename
echo "Only the seed URL file should be created, with the filename 1." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 0" >> crawlerTestlog.$filename

rm -rf ./data # Remove the directory.
mkdir ./data # Create a new directory to ensure it's empty.

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 0 > /dev/null # Run the program
ls ./data | sort -n | tail -1 >> crawlerTestlog.$filename # Print the number of files
printf "\n" >> crawlerTestlog.$filename

echo "Preview of ./1" >> crawlerTestlog.$filename
head -4 ./data/1 >> crawlerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> crawlerTestlog.$filename


# Depth 1
echo "9." >> crawlerTestlog.$filename
echo "This tests for depth 1." >> crawlerTestlog.$filename
echo "7 files should be created, 1 for depth 0, and 6 for depth 1." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1" >> crawlerTestlog.$filename

rm -rf ./data # Remove the directory.
mkdir ./data # Create a new directory to ensure it's empty.

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1 > /dev/null # Run the program
ls ./data | sort -n | tail -1 >> crawlerTestlog.$filename # Print the number of files
printf "\n" >> crawlerTestlog.$filename

echo "Preview of ./5" >> crawlerTestlog.$filename
head -4 ./data/5 >> crawlerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> crawlerTestlog.$filename


# Depth 2
echo "10." >> crawlerTestlog.$filename
echo "This tests for depth 2." >> crawlerTestlog.$filename
echo "1705 files should be created, 1 for depth 0, 6 for depth 1, and 1698 for depth 2." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 2" >> crawlerTestlog.$filename

rm -rf ./data # Remove the directory.
mkdir ./data # Create a new directory to ensure it's empty.

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 2 > /dev/null # Run the program
ls ./data | sort -n | tail -1 >> crawlerTestlog.$filename # Print the number of files
printf "\n" >> crawlerTestlog.$filename

echo "Preview of ./532" >> crawlerTestlog.$filename
head -4 ./data/532 >> crawlerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> crawlerTestlog.$filename


# Depth 3
echo "11." >> crawlerTestlog.$filename
echo "This tests for depth 3." >> crawlerTestlog.$filename
echo "1705 files should be created, 1 for depth 0, 6 for depth 1, 1698 for depth 2, and 0 for depth 3." >> crawlerTestlog.$filename

printf "\n" >> crawlerTestlog.$filename
echo "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3" >> crawlerTestlog.$filename

rm -rf ./data # Remove the directory.
mkdir ./data # Create a new directory to ensure it's empty.

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3 > /dev/null # Run the program
ls ./data | sort -n | tail -1 >> crawlerTestlog.$filename # Print the number of files
printf "\n" >> crawlerTestlog.$filename

echo "Preview of ./1000" >> crawlerTestlog.$filename
head -4 ./data/1000 >> crawlerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> crawlerTestlog.$filename

#!/bin/bash
# File: 		BATS.sh
#
# Description: 		Tests the executable ./indexer for all boundary cases, and cases of good output.
#			For good output, writes to stdout a snippet of the index file(s) created. For the case 
# 			of 3 arguments, the two files are also compared to see if the program recreated
# 			the index correctly.
#
# Input: 		No input.
#
# Output: 		Outputs error messages for boundary cases, or snippet(s) of sample file(s) for good cases.
# 			For the case of 3 arguments, a statement of whether the two files are the same or not
# 			is also printed.
# 
# Pseudocode: 		The program goes through each boundary case and good case, providing a
# 			brief description of what was tested, what output should be expected,
# 			and then it prints the output.
# 	

filename=`date +"%a_%b_%d_%T_%Y"` # IndexerTestlog.$filename

# Introductory print statements
echo "Indexer Log" > IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename


# Print Build time, hostname, and Operating System information.
echo "Build Start: `date`" >> IndexerTestlog.$filename
echo "Hostname: $HOSTNAME" >> IndexerTestlog.$filename
printf "Operating System: " >> IndexerTestlog.$filename
uname -o >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename

# Compile Program
make clean > /dev/null
make > /dev/null

# Variables for convenience.
DATA_PATH=~/cs50/labs/lab5/crawler/data
INDEX_FILE=index.dat
NEW_INDEX_FILE=new_index.dat

# Build Indexer
# ./indexer ~/cs50/labs/lab5/crawler/data index.dat new_index.dat

# Date & Time Stamp of the beginning of the build
# echo "Build Started at `date`" >> IndexerTestlog.$filename



# Boundary Cases
echo "BOUNDARY CASES." >> IndexerTestlog.$filename
echo "An appropriate error message will be printed for each case." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename




# Case of invalid number of arguments (not 2 or 3).
echo "1." >> IndexerTestlog.$filename
echo "This tests for the case where there is an invalid number of args passed (not 2 or 3)." >> IndexerTestlog.$filename
echo "The program should throw an error." >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "Input: ./indexer $DATA_PATH" >> IndexerTestlog.$filename

printf "Output: " >> IndexerTestlog.$filename
./indexer $DATA_PATH >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename



# Case of nonexistent TARGET_DIRECTORY.
echo "2." >> IndexerTestlog.$filename
echo "This tests for the case where the TARGET_DIRECTORY does not exist." >> IndexerTestlog.$filename
echo "The program should throw an error." >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "At time of testing, there was no ./testing directory on the machine." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename

echo "Input: ./indexer ./testing $INDEX_FILE" >> IndexerTestlog.$filename
printf "Output: " >> IndexerTestlog.$filename
./indexer ./testing $INDEX_FILE >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename



# Case of empty TARGET_DIRECTORY.
echo "3." >> IndexerTestlog.$filename
echo "This tests for the case where the TARGET_DIRECTORY is empty and has no webpages to be indexed." >> IndexerTestlog.$filename
echo "The program should throw an error." >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "For this test, I used an empty ./data directory." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename
echo "Input: ./indexer ./data test.dat" >> IndexerTestlog.$filename

touch test.dat
mkdir ./data
printf "Output: " >> IndexerTestlog.$filename
./indexer ./data test.dat >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename



# Case of nonexistent index file(s).
echo "4." >> IndexerTestlog.$filename
echo "This tests for the case where the file(s) passed do(es) not exist." >> IndexerTestlog.$filename
echo "The program should throw an error." >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "The program checks the validity of test.dat and then if passed, new_test.dat." >> IndexerTestlog.$filename
echo "For this test, test.dat exists but new_test.dat does not." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename

echo "Input: ./indexer $DATA_PATH test.dat new_test.dat" >> IndexerTestlog.$filename
printf "Output: " >> IndexerTestlog.$filename
./indexer $DATA_PATH test.dat new_test.dat >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename



# Case of non-empty index file(s).
echo "5." >> IndexerTestlog.$filename
echo "This boundary case refers to when the index.dat and the new_index.dat files are not empty." >> IndexerTestlog.$filename
echo "The program just prints warning messages that the files will be overwritten and continues on." >> IndexerTestlog.$filename
echo "For the sake of boundary case testing and speed, the warning statements are printed below without the program actually being run." >> IndexerTestlog.$filename
echo "To observe the warning statements while the program is being run, refer to Good Output case 2." >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename

printf "Output: " >> IndexerTestlog.$filename
echo "index.dat is not empty. The contents will be overwritten." >> IndexerTestlog.$filename
echo "new_index.dat is not empty. The contents will be overwritten." >> IndexerTestlog.$filename
printf "\n\n" >> IndexerTestlog.$filename




# Remove any temporary testing files/directories created.
rmdir ./data 
rm test.dat

# Remove the index files and create empty ones.
rm $INDEX_FILE
rm $NEW_INDEX_FILE

touch $INDEX_FILE
touch $NEW_INDEX_FILE




# Good output
echo "GOOD OUTPUT." >> IndexerTestlog.$filename
echo "The first 4 lines of the index files will be printed to show how the files should look like." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename


# Case of 2 arguments passed.
echo "Case of 2 arguments passed." >> IndexerTestlog.$filename
echo "The program will create an index and save it to the file passed." >> IndexerTestlog.$filename
echo "Input: ./indexer $DATA_PATH $INDEX_FILE" >> IndexerTestlog.$filename
echo "Output: " >> IndexerTestlog.$filename
./indexer $DATA_PATH $INDEX_FILE >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "Preview of $INDEX_FILE" >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename
head -4 $INDEX_FILE >> IndexerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> IndexerTestlog.$filename


# Case of 3 arguments passed.
echo "Case of 3 arguments passed." >> IndexerTestlog.$filename
echo "The program will create an index, save it to the first file arg, recreate the index, and save it to the second file arg." >> IndexerTestlog.$filename
echo "Input: ./indexer $DATA_PATH $INDEX_FILE $NEW_INDEX_FILE" >> IndexerTestlog.$filename
echo "Output: " >> IndexerTestlog.$filename
./indexer $DATA_PATH $INDEX_FILE $NEW_INDEX_FILE >> IndexerTestlog.$filename

printf "\n" >> IndexerTestlog.$filename
echo "Preview of $INDEX_FILE" >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename
head -4 $INDEX_FILE >> IndexerTestlog.$filename # Print first 4 lines of file.
printf "\n" >> IndexerTestlog.$filename

echo "Preview of $NEW_INDEX_FILE" >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename
head -4 $NEW_INDEX_FILE >> IndexerTestlog.$filename # Print first 4 lines of file.
printf "\n\n" >> IndexerTestlog.$filename


# Compare the two files to see if they are the same.
echo "File Testing." >> IndexerTestlog.$filename
echo "Compare the two files to see if they are the same." >> IndexerTestlog.$filename
echo "If they are the same, then this means the program can read in and write out index storage file accurately." >> IndexerTestlog.$filename
printf "\n" >> IndexerTestlog.$filename

printf "Test output: " >> IndexerTestlog.$filename
FILE_CMP=`diff -q $INDEX_FILE $NEW_INDEX_FILE` 
if [ "$FILE_CMP" = "" ]
then
	echo "The two files are the same!" >> IndexerTestlog.$filename
else
	echo "The two files are different!" >> IndexerTestlog.$filename
fi

# Print build end time
printf "\n\n" >> IndexerTestlog.$filename
echo "Build End: `date`" >> IndexerTestlog.$filename

# Cleanup
make clean > /dev/null













#!/bin/bash
# File: 		QEBATS.sh
#
# Description: 	Tests the executable ./query for all boundary cases, and cases of good output.
#				For good output, writes to stdout the list of matching doc ids and urls. 
#
# Input: 		No input.
#
# Output: 		Outputs error messages for boundary cases, or list of matching doc ids and urls.
# 
# Pseudocode: 	The program goes through each boundary case and good case, providing a
# 				brief description of what was tested, what output should be expected,
# 				and then it prints the output.
# 	

filename=`date +"%a_%b_%d_%T_%Y"` # QueryTestlog.$filename

# Introductory print statements
echo "Query Log" > QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Print Build time, hostname, and Operating System information.
echo "Build Start: `date`" >> QueryTestlog.$filename
echo "Hostname: $HOSTNAME" >> QueryTestlog.$filename
printf "Operating System: " >> QueryTestlog.$filename
uname -o >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

# Compile Program
make clean > /dev/null
make 1> /dev/null 2> /dev/null

# Variables for convenience.
INDEX_FILE=../../indexer/index.dat
HTML_DIRECTORY=../../crawler/data



# Boundary Cases
echo "BOUNDARY CASES." >> QueryTestlog.$filename
echo "An appropriate error message will be printed for each case." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename




# Case of invalid number of arguments (not 2).
echo "1." >> QueryTestlog.$filename
echo "This tests for the case where there is an invalid number of args passed (not 2)." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename

printf "\n" >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE" >> QueryTestlog.$filename

printf "Output: " >> QueryTestlog.$filename
../query $INDEX_FILE >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of nonexistent INDEX_FILE.
echo "2." >> QueryTestlog.$filename
echo "This tests for the case where the INDEX_FILE does not exist." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename

printf "\n" >> QueryTestlog.$filename
echo "At time of testing, there was no test_file file on the machine." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename

echo "Input: ../query test_file $HTML_DIRECTORY" >> QueryTestlog.$filename
printf "Output: " >> QueryTestlog.$filename
../query test_file $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of nonexistent HTML_DIRECTORY.
echo "3." >> QueryTestlog.$filename
echo "This tests for the case where the HTML_DIRECTORY is empty and has no webpages to be indexed." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename

printf "\n" >> QueryTestlog.$filename
echo "For this test, I used a nonexistent ./data directory." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE ./data" >> QueryTestlog.$filename

printf "Output: " >> QueryTestlog.$filename
../query $INDEX_FILE ./data >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of empty query.
echo "4." >> QueryTestlog.$filename
echo "This tests for the case where the query line is empty." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: ''" >> QueryTestlog.$filename
echo "" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of non-ASCII characters or non-whitespace characters passed to the query.
echo "5." >> QueryTestlog.$filename
echo "This tests for the case where non-ASCII characters, or non-whitespace characters, are passed as the query." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: life2good brother" >> QueryTestlog.$filename
echo "life2good brother" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of the query line starting with AND or OR.
echo "6." >> QueryTestlog.$filename
echo "This tests for the case where the first word of the query line is AND or OR." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: AND life good" >> QueryTestlog.$filename
echo "AND life good" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of the query line ending with AND or OR.
echo "7." >> QueryTestlog.$filename
echo "This tests for the case where the last word of the query line is AND or OR." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: life good OR" >> QueryTestlog.$filename
echo "life good OR" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of the query line containing successive logical operators together.
echo "8." >> QueryTestlog.$filename
echo "This tests for the case where the query contains a line of logical operators together." >> QueryTestlog.$filename
echo "The program should throw an error." >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: life good OR AND OR power" >> QueryTestlog.$filename
echo "life good OR AND OR power" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Case of the query line containing uppercase and lowercase letters.
echo "9." >> QueryTestlog.$filename
echo "This tests for the case where the query contains words in uppercase and lowercase." >> QueryTestlog.$filename
echo "The program should run normally, outputting list of doc_id and url." >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
echo "Stdin: HINdrANCE" >> QueryTestlog.$filename
echo "HINdrANCE" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n\n\n" >> QueryTestlog.$filename




# Good output
echo "GOOD OUTPUT." >> QueryTestlog.$filename
echo "The list of doc_ids and urls are printed to show how the output should look like." >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Testing a query with OR.
echo "1." >> QueryTestlog.$filename
echo "Testing a query with OR." >> QueryTestlog.$filename
echo "The program will output a list of docs that contain a union of the words." >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: libation" >> QueryTestlog.$filename
echo "libation" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: hindrance" >> QueryTestlog.$filename
echo "hindrance" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: libation OR hindrance" >> QueryTestlog.$filename
echo "libation OR hindrance" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Testing a query with AND.
echo "2." >> QueryTestlog.$filename
echo "Testing a query with AND." >> QueryTestlog.$filename
echo "The program will output a list of docs that contain an intersection of the words." >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: squash" >> QueryTestlog.$filename
echo "squash" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: whale" >> QueryTestlog.$filename
echo "whale" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: squash AND whale" >> QueryTestlog.$filename
echo "squash AND whale" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Testing a query with no operators (AND operation).
echo "3." >> QueryTestlog.$filename
echo "Testing a query with no operators (AND operation)." >> QueryTestlog.$filename
echo "The program will perform the same way it would with a query that contains 'AND'." >> QueryTestlog.$filename
echo "Compare this output with the above." >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: squash whale" >> QueryTestlog.$filename
echo "squash whale" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename


# Testing a query with many words and operators.
echo "4." >> QueryTestlog.$filename
echo "Testing a query with many words and operators." >> QueryTestlog.$filename
echo "Input: ../query $INDEX_FILE $HTML_DIRECTORY" >> QueryTestlog.$filename
printf "\n\n" >> QueryTestlog.$filename

echo "Stdin: dog OR cat OR life OR power AND chill" >> QueryTestlog.$filename
echo "dog OR cat OR life OR power AND chill" | ../query $INDEX_FILE $HTML_DIRECTORY >> QueryTestlog.$filename
printf "\n\n\n\n" >> QueryTestlog.$filename



# Unit Testing.
echo "Below, we unit test each of the important functions used to build query." >> QueryTestlog.$filename
printf "\n" >> QueryTestlog.$filename
./queryengine_test >> QueryTestlog.$filename


# Print build end time
printf "\n\n" >> QueryTestlog.$filename
echo "Build End: `date`" >> QueryTestlog.$filename

# Cleanup
make clean > /dev/null













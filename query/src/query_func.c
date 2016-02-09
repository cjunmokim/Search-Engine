/* ========================================================================== */
/* File: query_test.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * Author: Caleb Junmo Kim
 * Date: 8/12/2015
 *
 * This file includes all the functionality used in query.c.
 * This file is created for the purpose of unit testing with queryengine_test.c
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/stat.h>			     // stat functionality
#include <string.h>			     // string functionality
#include <ctype.h>			     // character functionality
#include <unistd.h>			     // sleep functionality
#include <stdlib.h> 			     // memory functionality
#include <math.h> 			     // math functionality

// ---------------- Local includes  e.g., "file.h"
#include "qhashtable.h" 		     // hashtable functionality
#include "file.h" 			     // file functionality
#include "qweb.h" 			     // web/html functionality
#include "query.h"

// ---------------- Constant definitions
char *operator1 = "AND";
char *operator2 = "OR";

// ---------------- Macro definitions
#define MAX 1000 // Max number of characters for a single query search.
#define MATH_MAX(X, Y) (((X) > (Y)) ? (X) : (Y)) // Returns max of the two numbers passed.

// ---------------- Structures/Types

// ---------------- Private variables
char *file; 								 // passed file path
char *dir_path; 							 // passed directory path
// DocumentNode *temp_list;					 // temp_list
// DocumentNode *final_list;					 // final list

// ---------------- Private prototypes

/*
 * GetLinks - get all matched DocumentNodes for a given query.
 * @line: query line to be searched for.
 * @Index: InvertedIndex containing all word-document pairs.
 * 
 * Returns 1 if successful.
 * Returns 0 if not successful.
 *
 * Pseudocode:
 *     1. Read in each word of the query.
 *     2. Save all matched DocumentNodes in temp_list.
 *     2. If an AND is passed, or there is no logical operator between two words, intersection operation is done on temp_list.
 *     3. If an OR is passed, data in temp_list is flushed out to final_list using the union operation.
 *     4. Continue until end of line. 
 */

int GetLinks(char *line, HashTable *Index) {
	
	// Declare variables.
	char *buf;
	char word[MAX];
	int flag; // flag to do union or intersection operations.
	int count; // variable to count the position of a word in the line.
	
	// Initialize variables.
	buf = line;
	flag = 1;
	count = 0;
	temp_list = NULL;
	final_list = NULL;
	
	// Loop through the line and do the appropriate operations.
	while (sscanf(buf, "%s", word) == 1) {
		
		count++;
		
		// If word is AND, then ignore and read in new word.
		if (strcmp(word, operator1) == 0) {
			if (count == 1) { // If there is no previous word, then throw an error.
				return 0;
			}
			
			// Increment position in line.
			buf = strstr(buf, word) + strlen(word);
			continue;
		}
		
		// If word is OR, then tell the program to do OR operation.
		if (strcmp(word, operator2) == 0) {
			flag = 2; // Set flag to union operation.
			if (count == 1) { // If there is no previous word, then throw an error.
				return 0;
			}
		}
		
		// Hold onto original copy of word in case NormalizeWord() changes its content.
		char *word_old = (char *)calloc(1, strlen(word) + 1);
		strcpy(word_old, word);
		
		// Change word to lowercase.
		if (strcmp(word, operator1) != 0 && strcmp(word, operator2) != 0) {
			NormalizeWord(word); // Normalize if word is not an operator.
		}
		
		// Add list of docs to temp_list.
		// Case when it is the first word of the block.
		if (count == 1) {
			
			// Declare variables.
			WordNode *current; // variable for traversal.
			DocumentNode *ptr, *ptr2; // variables for traversal.
			int num;
			
			// Case when the word is in the InvertedIndex.
			if ((num = InHashTable(word, Index))) {
				unsigned long index = JenkinsHash(word, MAX_HASH_SLOT); // Get the hash code.
				current = Index->table[index]->data;
				
				// Loop until we get the matching WordNode.
				for (int i=1; i < num; i++) {
					current = current->next;
				}
				
				// Loop through each DocumentNode and add to temp_list.
				for (ptr = current->page; ptr != NULL; ptr = ptr->next) {
					
					// Declare and initialize a DocumentNode with the same values as ptr.
					DocumentNode *dn;
					dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
					dn->doc_id = ptr->doc_id;
					dn->freq = ptr->freq;
					
					// Add the new DocumentNode to temp_list.
					if (temp_list == NULL) { // Case when temp_list is empty.
						temp_list = dn;
						ptr2 = temp_list;
					}
					else { // Case when temp_list is nonempty.
						ptr2->next = dn;
						ptr2 = ptr2->next;
					}
				}
			}
		}
		else { // If not first word of the block, then do the operation.
		
			// Check if the current operation is "AND".
			if (flag == 1) {
				And(word, Index);
			}
			
			// Check if the current operation is "OR".
			if (flag == 2) {
				if (temp_list != NULL) {
					Or();
				}
				flag = 1; // Set flag back to "AND" operation.
				count = 0; // Set word count to 0 to signal the start of a new block of words.
			}
		}
		
		// Increment position in the query line to read in next word.
		buf = strstr(buf, word_old) + strlen(word_old);
		
		free(word_old); // Cleanup.
	}
	
	// If the last word of the query line is an operator, throw an error.
	if (strcmp(word, operator1) == 0 || strcmp(word, operator2) == 0) {
		return 0;
	
	}
	
	// If nonempty, flush out temp_list to final_list.
	if (temp_list != NULL) { 
		Or();
	}
	
	return 1; // Return 1 if successful.
}	


/*
 * And - Perform intersection operation.
 * @word: word to get new list of matching DocumentNodes from.
 * @Index: InvertedIndex containing all word-document pairs.
 * 
 * Pseudocode:
 *     1. Find the start of the list of matching DocumentNodes for the word.
 *     2. For each DocumentNode in temp_list, go through the new list looking for doc_id matches.
 *     2. If there is a match, keep that DocumentNode in temp_list and increment frequency.
 *     3. If there is no match, delete that DocumentNode from temp_list.
 *     4. Continue until end of temp_list.
 */

void And(char *word, HashTable *Index) {

	unsigned long index = JenkinsHash(word, MAX_HASH_SLOT); // Get the hash code.
	
	// Declare variables for traversal.
	WordNode *current;
	DocumentNode *ptr, *ptr2, *runner, *no_need;
	int num;
	
	// Get matching WordNode of word if it is in the InvertedIndex.
	if ((num = InHashTable(word, Index))) {
		current = Index->table[index]->data;
		// Loop until we get the matching WordNode.
		for (int i=1; i < num; i++) {
			current = current->next;
		}
		ptr2 = current->page; // Set to start of the list of document nodes for the current word.
	}
	else {
		ptr2 = NULL;
	}
	
	// Initialize variables. 
	ptr = temp_list;
	
	while (ptr != NULL) {
	
		// Check that the word is in the InvertedIndex.
		if (num) {
			ptr2 = current->page; // Set to start of the list of document nodes for the current word.
			
			// Loop until the end of the new list of matching DocumentNodes.
			while (ptr2 != NULL) {
				// Check for a match in doc_id.
				if (ptr->doc_id == ptr2->doc_id) {
					ptr->freq += ptr2->freq; // Add the frequencies.
					break;
				}
				ptr2 = ptr2->next;
			}
			
			// Case of no match.
			if (ptr2 == NULL) {
			
				// Check if we need to delete the first node of temp_list.
				if (ptr == temp_list) { 
					temp_list = temp_list->next;
				}
				else { // All other cases.
					runner->next = runner->next->next;
				}
				no_need = ptr;
				ptr = ptr->next;
				
				// Free the node to be deleted.
				no_need->next = NULL;
				free(no_need);
				no_need = NULL;
				
			}
			else { // Case of match.
				runner = ptr;
				ptr = ptr->next;
			}
		}
		else { // Word is not in the InvertedIndex.
			ptr = NULL;
			FreeList(0);
		}
	}
}


/*
 * Or - Perform union operation.
 * 
 * Returns 0 and terminates.
 * 
 * Pseudocode:
 *     1. If final_list is NULL, set final_list to temp_list, temp_list to NULL, and return.
 *     2. For each DocumentNode in temp_list, go through final_list looking for doc_id matches.
 *     2. If there is a match, keep that DocumentNode in final_list and increment frequency.
 *     3. If there is no match, create a copy DocumentNode and add to end of final_list.
 *     4. Continue until end of temp_list.
 */

int Or() {

	// Case of empty final_list, set final_list to temp_list.
	if (final_list == NULL) {
		final_list = temp_list;
		temp_list = NULL;

		return 0;
	}

	// Declare variables.
	DocumentNode *ptr, *ptr2, *runner; // variables for traversal.
	
	// For each DocumentNode of temp_list, loop through all DocumentNodes of final_list.
	for (ptr = temp_list; ptr != NULL; ptr = ptr->next) {
		for (ptr2 = final_list; ptr2 != NULL; ptr2 = ptr2->next) {
			
			// Check for a match in doc_id.
			if (ptr->doc_id == ptr2->doc_id) {
				ptr2->freq = MATH_MAX(ptr->freq, ptr2->freq); // Set freq as the maximum of the two.
				break;
			}
			if (ptr2->next == NULL) {
				runner = ptr2; // variable for appending DocumentNode to final_list.
			}
		}
		
		// If there is no match in doc_id, then append a DocumentNode to final_list.
		if (ptr2 == NULL) {
			DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
			dn->doc_id = ptr->doc_id;
			dn->freq = ptr->freq;
			
			runner->next = dn;
		}
	}
	
	// Cleanup.
	FreeList(0);

	return 0;

}


/*
 * Sort - Sort the list of DocumentNodes by rank, from highest to lowest.
 * 
 * Pseudocode:
 *     1. Put DocumentNodes into an array.
 *     2. Sort the array by rank.
 *     2. Refactor sorted array into list of DocumentNodes.
 */

void Sort() {

	DocumentNode *ptr; // variable for traversal.
	int i=0;
	
	// Loop through the list to get its size.
	for (ptr = final_list; ptr != NULL; ptr = ptr->next) {
		i++;
	}
	
	// Declare and initialize an array of DocumentNode*.
	DocumentNode *arr[i];
	memset(arr, 0, i);
	
	i=0;
	
	// Loop through the list and add DocumentNode* to array.
	for (ptr = final_list; ptr != NULL; ptr = ptr->next) {
		arr[i] = ptr;
		i++;
	}

	qsort((void *)arr, i, sizeof(DocumentNode *), key_compare); // Sort array by rank.
	
	// Recreate the list of DocumentNodes from the sorted array.
	for (int j = 0; j < i; j++) {
		if (j == 0) {
			final_list = arr[j];
			ptr = final_list;
		}
		else {
			ptr->next = arr[j];
			ptr = ptr->next;
		}
	}
	ptr->next = NULL;
}


/*
 * key_compare - key compare function for qsort.
 * @e1: element to compare.
 * @e2: element to compare.
 *
 * Returns -1 if e1>e2, 1 if e1<e2, 0 if e1==e2.
 *
 * Pseudocode:
 *     1. Get the frequency of the two passed DocumentNodes.
 *     2. Return appropriate comparison value.
 */

int key_compare(const void *e1, const void *e2) {

	DocumentNode *ia = *(DocumentNode **)e1;
	DocumentNode *ib = *(DocumentNode **)e2;
	int f1 = ia->freq;
	int f2 = ib->freq;

	if (f1 > f2) {
		return -1;
	}
	else if (f1 < f2) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * FreeList - free memory of DocumentNode lists.
 * @choice: the type of list to free. If 0, free temp_list, and if 1, free final_list.
 *
 * Returns 0 and terminates.
 *
 * Pseudocode:
 *     1. For the appropriate list, loop through each DocumentNode.
 *     2. Free DocumentNode and move on to the next one.
 *     3. Free until list is empty.
 */
 
int FreeList(int choice) {

	DocumentNode *ptr; // variable for traversal.
	
	// Free temp_list.
	if (choice == 0) {
		ptr = temp_list;
	
		while (ptr != NULL) {
			DocumentNode *ptr2 = ptr;
			ptr = ptr->next;
			free(ptr2);
		}
		temp_list = NULL;
	}
	
	else { // Free final_list.
		ptr = final_list;
	
		while (ptr!= NULL) {
			DocumentNode *ptr2 = ptr;
			ptr = ptr->next;
			free(ptr2);
		}
		final_list = NULL;
	}

	return 0;
}


/*
 * Display - display query matches to output.
 *
 * Returns 1 if successful.
 * Returns 0 if not.
 *
 * Pseudocode:
 *     1. Get each DocumentNode of final_list.
 *     2. Get filename and open a stream to that file.
 *     3. Read in the URL address from stream.
 *     4. Output doc_id and url to stdout.
 *     5. Cleanup memory and close stream.
 */

int Display() {
	
	DocumentNode *dn_ptr; // variable for traversal.
	
	// Loop through each DocumentNode of the query match.
	for (dn_ptr = final_list; dn_ptr != NULL; dn_ptr = dn_ptr->next) {
	
		// Create and get the filename to read from.
		char *filename;
		filename = (char *)calloc(1, MAX);
		sprintf(filename, "%s/%d", dir_path, dn_ptr->doc_id); // Get the filename.
		filename = realloc(filename, strlen(filename) + 1);
		
		// Create and open a stream to the filename.
		FILE *fp;
		fp = fopen(filename, "r");
		
		// Read in the html address from the file.
		char *line = (char *)calloc(1, MAX);
		if (!fgets(line, MAX, fp)) {
			free(filename);
			free(line);
			free(fp);
			return 0;
		}
		line = realloc(line, strlen(line) + 1);
		
		// Write the doc_id and the url for each match.
		printf("DOCUMENT ID: %d ", dn_ptr->doc_id);
		printf("URL: %s", line);
		
		// Cleanup.
		free(filename);
		free(line);
		fclose(fp);
	}
	return 1;
}

	
	

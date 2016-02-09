/* ========================================================================== */
/* File: qhashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * Author: Caleb Junmo Kim
 * Date: 8/12/2015
 *
 * This file includes all the functionality related to the InvertedIndex.
 * Please go to the top of each function for more detailed descriptions.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "qhashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions
#define MAX 100000
// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int AddWord(char *, int, HashTable *);
int InHashTable(char *, HashTable *);
int CleanHashTable(HashTable *);
int InitializeHashTable(HashTable *);
int FreeHashTable(HashTable *);
void NormalizeWord(char *);
HashTable *ReadFile(char *, HashTable *);

// Function to compute the hash code for a given string.
unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}


/*
 * AddWord - adds a word to the InvertedIndex.
 * @WORD: word to be added.
 * @doc_ID: doc_ID of the doc in which the word was found.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 1 if successful, 0 if not.
 *
 * Assumptions:
 *     1. InvertedIndex has been initialized.
 *
 * Pseudocode:
 *     1. Compute the hash code.
 *     2. Check if the word already exists in the InvertedIndex.
 *     3. If it does, then see if there is a matching DocumentNode. Increment frequency if found.
 *     4. If there is no matching DocumentNode, create a new one and add to the InvertedIndex.
 *     5. If the word does not exist in the InvertedIndex, create a new WordNode and add to the InvertedIndex.
 *
 */
 
int AddWord(char *WORD, int doc_ID, HashTable *Index) {
	
	NormalizeWord(WORD);
	unsigned long index = JenkinsHash(WORD, MAX_HASH_SLOT); // Get the hash code.
	
	WordNode *current; // node ptr for traversal
	WordNode *node; // new node for adding
	
	// Check that the word is valid.
	if (!WORD) {
		return 0;
	}
	
	// Check if the word already exists.
	int num;
	if ((num = InHashTable(WORD, Index))) {
		current = Index->table[index]->data;
		
		// Loop until we get the matching WordNode.
		for (int i=1; i < num; i++) {
			current = current->next;
		}
		
		DocumentNode *ptr; // ptr for traversal
		// Search if the WordNode has the matching DocumentNode.
		for (ptr = current->page; ptr->next != NULL; ptr = ptr->next) {
			// Check whether the correct DocumentNode is in the index or not.
			if (ptr->doc_id == doc_ID) {
				ptr->freq++; // Increment frequency if match is found.
				return 1;
			}
		}
		if (ptr->doc_id == doc_ID) {
			ptr->freq++; // Increment frequency if match is found.
			return 1;
		}
		
		// There is no matching DocumentNode, so create a new DocumentNode.
		DocumentNode *doc_node;
		doc_node = (DocumentNode *)calloc(1, sizeof(DocumentNode));
		doc_node->doc_id = doc_ID;
		doc_node->freq = 1;
		
		ptr->next = doc_node; // Add this DocumentNode to the list of DocumentNodes.
		return 1;
	}
	
	// Word does not exist in the Index.
	
	// Create and initialize a new WordNode.
	node = (WordNode *)calloc(1, sizeof(WordNode));
	if (!node) {
		return 0;
	}
	node->word = (char *)malloc(strlen(WORD)+1);
	if (!node->word) {
		return 0;
	}
	strcpy(node->word, WORD);
	
	// Add a DocumentNode to the WordNode. 
	node->page = (DocumentNode *)calloc(1, sizeof(DocumentNode));
	node->page->doc_id = doc_ID;
	node->page->freq = 1;
	
	// Case when there is no WordNode in the bin.
	if (Index->table[index]->data == NULL) {
		Index->table[index]->data = node;
	}
	else { // Case when there is already a WordNode in the bin, ie collision.
		// Loop through until we get the last element.
		for(current = Index->table[index]->data; current->next != NULL; current = current->next);
		current->next = node; // Set the new WordNode as the last element of the list.
	}
	return 1;
}



/*
 * InHashTable - checks whether a word is in the InvertedIndex.
 * @WORD: word to be searched.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 0 if the word is not in the InvertedIndex.
 * Returns i if match is found, where i refers to the position of the matching WordNode in the bin.
 * 		For instance, if the matching WordNode is the second WordNode in the bin, then i = 2. 
 *
 * Assumptions:
 *     1. InvertedIndex has been initialized.
 *
 * Pseudocode:
 *     1. Compute the hash code.
 *     2. Loop through all the WordNodes of the bin.
 *     3. If the matching WordNode is found, then return.
 *     4. Else, return 0.
 *
 */

int InHashTable(char *WORD, HashTable *Index) {
	unsigned long index = JenkinsHash(WORD, MAX_HASH_SLOT); // Get the hash code.
	WordNode *current; // node ptr for traversal
	
	int i = 0;
	// Loop through until the last element of the list of HashTableNodes for the bin.
	for(current = Index->table[index]->data; current != NULL; current = current->next) {
		i++;
		int result = strcmp(current->word, WORD); // Check whether the word is in the index or not.
		if (result == 0) { // Word is already in the index.
			return i;
		}
	}
	return 0;
}

/*
 * CleanHashTable - frees all WordNodes and DocumentNodes in the InvertedIndex.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 0 at program termination.
 *
 * Assumptions:
 *     1. InvertedIndex has been initialized.
 *
 * Pseudocode:
 *     1. Loop through the entire InvertedIndex.
 *     2. Add WordNode and word pointers to arrays of WordNode and word pointers.
 *     3. Add DocumentNode pointers of each WordNode to array of DocumentNode pointers.
 *     4. Looping backwards through the three arrays, free the content in them.
 *
 */

int CleanHashTable(HashTable *Index) {
	
	// Variable for traversal.
	WordNode *current; 
	
	// Loop through the entire InvertedIndex.
	for (int index=0; index < MAX_HASH_SLOT; index++) {
		WordNode *wn_ptr[100] = {0}; // Array to hold pointers to WordNodes.
		char *word_ptr[100] = {0}; // Array to hold pointers to words of WordNodes.
		DocumentNode *dn_ptr[MAX] = {0}; // Array to hold pointers to DocumentNodes.
		
		int i=0; // counter for wn_ptr and word_ptr.
		
		// Check that the bin is not empty.
		if (Index->table[index]->data != NULL) {
			DocumentNode *ptr;
			// Loop through each WordNode of the bin.
			for(current = Index->table[index]->data; current != NULL; current = current->next) {
			
				// Add WordNode and its word pointer to the respective arrays.
				wn_ptr[i] = current;
				word_ptr[i] = current->word;
				
				int j = 0; // counter for dn_ptr.
				// Loop through each document node of the word node.
				for (ptr = current->page; ptr != NULL; ptr = ptr->next) {
					dn_ptr[j] = ptr;
					j++;
				}
				j--; // Decrement to account for the increment when program exits the for loop.
				
				// Loop through the dn_ptr backwards.
				for (int k=j; k >=0; k--) {
					free(dn_ptr[k]); // Free memory allocated to the document nodes.
				}
				i++;
			}
			i--; // Offset the final i increment.
		}
		
		// Loop through the word_ptr, wn_ptr arrays backwards.
		for (int m=i; m>=0; m--) {
			free(word_ptr[m]); // Free memory allocated to char *word in the WordNode.
			free(wn_ptr[m]); // Free memory allocated to the WordNode.
		}
		
		Index->table[index]->data = NULL; // Make sure the data isn't pointing to any WordNode.
	}
	return 0;
}


/*
 * FreeHashTable - frees all HashTableNodes of the InvertedIndex.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 0 after function is run.
 *
 * Assumptions:
 *     1. Each HashTableNode is empty, and is not pointing to any structure.
 *
 * Pseudocode:
 *     1. Loop through each bin of the InvertedIndex.
 *     2. Free HashTableNode.
 */

// Function to clear all the HashTableNodes allocated.
int FreeHashTable(HashTable *Index) {
	// Loop through each bin.
	for (int i=0; i < MAX_HASH_SLOT; i++) {
		free(Index->table[i]); // Free the HashTableNode.
		Index->table[i] = NULL;
	}
	return 0;

}


/*
 * InitializeHashTable - initializes the InvertedIndex by creating empty HashTableNodes.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 0 after function is run.
 *
 * Assumptions:
 *     1. InvertedIndex is empty / has not been initialized.
 *
 * Pseudocode:
 *     1. Loop through each bin of the InvertedIndex.
 *     2. Declare and initialize empty HashTableNodes.
 */
 
int InitializeHashTable(HashTable *Index) {
	// Loop through each bin of the index to initialize each node.
	for (int i=0; i < MAX_HASH_SLOT; i++) {
		Index->table[i] = (HashTableNode *)calloc(1, sizeof(HashTableNode)); // Initialize the hash table node.
		Index->table[i]->data = NULL; // Set the void * data to NULL for the node.
	}
	return 0;
}


/*
 * ReadFile - Read from a file to create an InvertedIndex.
 * @file_name: file to be read.
 * @New_Index: pointer to an InvertedIndex to be created.
 *
 * Returns a pointer to an InvertedIndex.
 *
 * Pseudocode:
 *     1. Read each line of file_name.
 *     2. Parse for the word and the number of docs the word appears in.
 *     3. Create a WordNode and add to the InvertedIndex.
 *     4. Loop through the rest of the line, parsing for doc_id and freq.
 *     5. Create a DocumentNode and add to the InvertedIndex.
 */

HashTable *ReadFile(char *file_name, HashTable *New_Index) {
	
	// Declare variables.
	FILE *fp;
	char *buf;
	char *WORD; // word
	char *num; // number of docs the word appears in.
	char *ptr; // pointer referencing position in the current line.
	
	// Initialize variables.
	fp = fopen(file_name, "r");
	buf = (char *)malloc(MAX);
	WORD = (char *)malloc(100);
	num = (char *)malloc(10);
	
	// Read each line of file_name.
	while (fgets(buf, MAX, fp) != NULL) {
		
		sscanf(buf, "%s %s", WORD, num);
		ptr = buf + strlen(WORD) + strlen(num) + 2; // Update position.
		
		// Create and initialize new word node.
		WordNode *wn;
		wn = (WordNode *)calloc(1, sizeof(WordNode)); // Initialize a new WordNode.
		wn->word = (char *)malloc(strlen(WORD) + 1);
		strcpy(wn->word, WORD);
		
		unsigned long index = JenkinsHash(WORD, MAX_HASH_SLOT); // Get the hash code.
		
		// Check that the bin is empty.
		if (New_Index->table[index]->data == NULL) {
			New_Index->table[index]->data = wn;
		}
		else { // The bin is not empty.
			WordNode *wn_ptr; // pointer for traversal.
			
			// Loop until the last wordnode.
			for(wn_ptr = New_Index->table[index]->data; wn_ptr->next != NULL; wn_ptr = wn_ptr->next);
			wn_ptr->next = wn; // Set the new WordNode as the last element of the WordNode list.
		}

		// Loop while position in the line is less than or equal to the end of the line.
		while (ptr <= buf + strlen(buf) - 2) { // -2 is to account for whitespace and newline char at the end.
			
			// Variables used for parsing to get the doc_id and freq.
			char *number1, *number2;
			int doc_id, freq;
			
			// Read in doc_id and freq as char *.
			number1 = calloc(1, 10);
			number2 = calloc(1, 10);
			sscanf(ptr, "%s %s", number1, number2);
			
			// update position in the line.
			ptr += strlen(number1) + strlen(number2) + 2;
			
			// Convert doc_id and freq from char* to int.
			sscanf(number1, "%d", &doc_id);
			sscanf(number2, "%d", &freq);
			
			// Create and initialize a DocumentNode.
			DocumentNode *dn;
			dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
			dn->doc_id = doc_id;
			dn->freq = freq;
			
			DocumentNode *dn_ptr; // pointer for traversal.
			
			// Check if there is no document node associated with the word node.
			if (wn->page == NULL) {
				wn->page = dn;
			}
			else {
				// Loop to the end of the DocumentNode list.
				for (dn_ptr = wn->page; dn_ptr->next != NULL; dn_ptr = dn_ptr->next);
				dn_ptr->next = dn;
			}
			
			// Cleanup.
			free(number1);
			free(number2);
		}
	}
	
	// Cleanup.
	free(WORD);
	free(num);
	free(buf);
	fclose(fp);

	return New_Index;
}

/* ========================================================================== */
/* File: chashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Caleb Junmo Kim
 * Date: 8/2/2015
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "chashtable.h"                       // hashtable functionality
#include "utils.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
HashTable URLsVisited; // HashTable variable


// ---------------- Private prototypes
int AddToHashTable(char *URL);
int InHashTable(char *URL);
int FreeHashTable();

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

// Function to add a url to the hash table.
int AddToHashTable(char *URL) {
	
	unsigned long index = JenkinsHash(URL, MAX_HASH_SLOT); // Get the hash code.
	HashTableNode *current; // node ptr for traversal
	HashTableNode *node; // new node for adding
	
	// Check that the url is valid.
	if (!URL) {
		return 0;
	}
	
	// Check that the url does not already exist in the hash table.
	if (InHashTable(URL)) {
		return 0;
	}
	
	node = (HashTableNode *)calloc(1, sizeof(HashTableNode)); // Initialize a new node.
	if (!node) { // Check that there were no problems allocating memory to the node.
		return 0;
	}
	node->url = (char *)malloc(strlen(URL)+1);
	if (!node->url) {
		return 0;
	}
	strcpy(node->url, URL);
	
	// Case when there is no element in the bin.
	if (URLsVisited.table[index] == NULL) {
		URLsVisited.table[index] = node;
	}
	else { // Case when there is already an element in the bin, ie collision.
		// Loop through until we get the last element.
		for(current = URLsVisited.table[index]; current->next != NULL; current = current->next);
		current->next = node; // Set the new node as the last element of the list.
	}
	return 1;
}

// Function to check whether the url is in the hash table or not.
int InHashTable(char *URL) {
	unsigned long index = JenkinsHash(URL, MAX_HASH_SLOT); // Get the hash code.
	HashTableNode *current; // node ptr for traversal
	current = URLsVisited.table[index];
	
	// Loop through until the last element of the list of HashTableNodes for the bin.
	for(current = URLsVisited.table[index]; current != NULL; current = current->next) {
		int result = strcmp(current->url, URL); // Check whether the url is in the hash table or not.
		if (result == 0) { // URL is already in the hash table.
			return 1;
		}
	}
	return 0;
}

// Function to free all memory dynamically allocated to the hash table.
int FreeHashTable() {
	HashTableNode *current; // node ptr for traversal
	
	// Loop through the entire hash table.
	for (int index=0; index < MAX_HASH_SLOT; index++) {
		HashTableNode *cur[100] = {0}; // Array to hold pointers to HashTableNodes
		char *urlAdd[100] = {0}; // Array to hold pointers to urls of HashTableNodes
		int i=0;
		
		// Check that the bin is not empty.
		if (URLsVisited.table[index] != NULL) {
			// Loop through until the last element.
			for(current = URLsVisited.table[index]; current != NULL; current = current->next) {
				// Add HashTableNode and its url pointer to the respective arrays.
				cur[i] = current;
				urlAdd[i] = current->url;
				i++;
			}
			i--; // Offset the final i increment.
		}
		
		// Loop through the two arrays backwards.
		for (int j=i; j>=0; j--) {
			free(urlAdd[j]); // Free the memory allocated to the url address of each HashTableNode.
			free(cur[j]); // Free the memory allocated to the HashTableNode.
		}
	}
	return 0;
}

/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine web crawler
 *
 * Author: Caleb Junmo Kim
 * Date: August 10, 2015
 *
 * Input: A TARGET DIRECTORY of crawled webpages, a file to save the index to, and if passed,
 * 		  another file to recreate the index and save this new index to, for comparison.
 *
 * Output: Outputs error messages if boundary cases are reached. Otherwise, there is no
 * 	       output, but instead the index is saved to the file(s) passed.
 *
 * Error Conditions: 1) Command line arguments are invalid.
 * 		     		 2) Memory cannot be allocated sufficiently.
 *
 * Special Considerations: The TARGET DIRECTORY should not contain a '/' at the end for correct parsing.
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/stat.h>			     // stat functionality
#include <curl/curl.h>                       // curl functionality
#include <string.h>			     // string functionality
#include <ctype.h>			     // character functionality
#include <unistd.h>			     // sleep functionality
#include <stdlib.h> 			 // memory functionality
// ---------------- Local includes  e.g., "file.h"
#include "iweb.h"                             // web/html functionality
#include "file.h"							 // file/dir functionality
#include "ihashtable.h" 						 // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions
#define MAX 100000
// ---------------- Structures/Types

// ---------------- Private variables
char *dir_path; 							 // passed directory path
char *file; 								 // passed file path
char *new_file; 							 // passed new file path


// ---------------- Private prototypes
char *LoadDocument(char *);
int GetDocumentId (char *);
int UpdateIndex(char *, int, HashTable *);
int InitializeHashTable();
int AddWord(char *, int, HashTable *);
int InHashTable(char *, HashTable *);
int SaveIndexToFile(HashTable *, char *);
int CleanHashTable (HashTable *);
int FreeHashTable(HashTable *);
HashTable *ReadFile(char *, HashTable *);

/* ========================================================================== */

int main (int argc, char *argv[]) {
	
	// Check arguments
	
	// Check that there are the correct number of arguments.
	if (argc != 3 && argc != 4) {
		printf("Please input the correct number of arguments.\n");
		return 1;
	
	}
	
	// Check that the target directory exists.
	if (!IsDir(argv[1])) {
		printf("Please input a valid TARGET_DIRECTORY.\n");
		return 1;
	}
	
	// Since the directory exists, store the path in dir_path.
	dir_path = calloc(1, strlen(argv[1]) + 1);
	strcpy(dir_path, argv[1]);
	
	// Check that the target file is valid.
	if (!IsFile(argv[2])) {
		printf("Please input a valid file.\n");
		return 1;
	}
	
	// Check that the target file is empty.
	FILE *fp;
	fp = fopen(argv[2], "r+");
	if (fp) {
		if (fgetc(fp) != EOF) {
			printf("index.dat is not empty. The contents will be overwritten.\n");
		}
	}
	fclose(fp);
	
	// Since the file is valid, store the name in file.
	file = calloc(1, strlen(argv[2]) + 1);
	strcpy(file, argv[2]);
	
	
	// Argument check for the new file if passed.
	if (argc == 4) {
		// Check that the new_index.dat file is valid.
		if (!IsFile(argv[3])) {
			printf("Please input a valid new_index.dat file.\n");
			return 1;
		}
	
		// Check that the new_index.dat file is empty.
		FILE *op;
		op = fopen(argv[2], "r+");
		if (op) {
			if (fgetc(op) != EOF) {
				printf("new_index.dat is not empty. The contents will be overwritten.\n");
			}
		}
		fclose(op);
		
		// Since the new file is valid, store the name in new_file.
		new_file = calloc(1, strlen(argv[3]) + 1);
		strcpy(new_file, argv[3]);
	}
	
	// Declare InvertedIndex and initialize it.
	HashTable Index;
	InitializeHashTable(&Index);
	
	// Get all file names in TARGET_DIRECTORY
	char **file_names;
	int num_files;
	
	num_files = GetFilenamesInDir(argv[1], &file_names);
	if (num_files < 0) {
		printf("Error getting file names from TARGET_DIRECTORY.\n");
		return 1;
	}
	else if (num_files == 0) {
		printf("TARGET_DIRECTORY is empty.\n");
		return 1;
	}
	
	printf("Building Index!\n");
	
	// Declare variables for building the InvertedIndex.
	char *doc;
	char *word;
	int doc_Id;
	int pos;

	// Loop through each html file.
	for (int i = 0; i < num_files; i++) {
		
		doc = LoadDocument(file_names[i]); // Store html content into a string.
		doc_Id = GetDocumentId(file_names[i]); // Get document_id.
		
		pos = 0; // Set the position to start of the document string.
		
		// Loop through a document string to get each word.
		while ((pos = GetNextWord(doc, pos, &word)) > 0) {
		
			// Update the InvertedIndex for the specified word.
			UpdateIndex(word, doc_Id, &Index);
			free(word);
		}
		// Cleanup.
		free(doc);
		free(file_names[i]);
	}
	
	SaveIndexToFile(&Index, file); // Save the built InvertedIndex to the target file.
	CleanHashTable(&Index); // Free all memory associated with the Hash Table.
	
	printf("Done!\n");
	
	// If there are 3 args passed, recreate index and save to a new file for comparison.
	if (argc == 4) {
		printf("Testing!\n");
		HashTable *ptr; // A new index pointer.
		
		ptr = ReadFile(file, &Index); // Read in from index.dat to create a new index.
		SaveIndexToFile(ptr, new_file); // Save the recreated index to a new file, ie new_index.dat.
		CleanHashTable(ptr); // Free all memory associated with the Hash Table.
		
		printf("Test complete!\n");
	}
	
	// Cleanup.
	free(file_names);
	FreeHashTable(&Index); // Free all hashtablenodes.
	free(dir_path);
	free(file);
	free(new_file);
	return 0;

}



// Helper Functions

/*
 * LoadDocument - Load a document into a string.
 * @file_name: file to be loaded.
 *
 * Returns the string of html if successful.
 * If not successful, returns NULL.
 *
 * Pseudocode:
 *     1. Get the full filename, complete with the directory.
 *     2. Set the position of the file to the start of the third line.
 *     3. Read each line and add to a string variable.
 */

char *LoadDocument(char *file_name) {

	// Declare variables.
	FILE *fp;
	char buf[MAX];
	char *full_file_name;
	char *html;
	
	full_file_name = (char *)calloc(1, strlen(dir_path) + strlen(file_name) + 2); // Allocate enough memory to hold dir_path, file_name, and '/'.
	// Write to the filename variable.
	if (sprintf(full_file_name, "%s/%s", dir_path, file_name) == EOF) {
    	return NULL;
	}
	
	// Initialize variables.
	fp = fopen(full_file_name, "r");
	
	// Set the position in the file to the third line.
	for (int i=0; i < 2; i++) {
		fgets(buf, MAX, fp);
	}
	
	html = (char *)calloc(1, 1);
	
	// Read in from the third line.
	while (fgets(buf, MAX, fp) != NULL) {
		html = realloc(html, strlen(html) + strlen(buf) + 1); // Reallocate memory as appropriate.
		strcat(html, buf); // Add the line to html.
	}
	
	// Cleanup.
	fclose(fp);
	free(full_file_name);
	return html;
}

/*
 * GetDocumentId - gets the unique doc_id of each file.
 * @file_name: file to get the doc_id from.
 *
 * Returns the doc_id, which is an integer.
 *
 * Assumptions:
 *     1. Passed file_name is not NULL.
 *
 * Pseudocode:
 *     1. Read in file_name as an integer.
 *     2. Return this integer as the doc_id.
 */
 
int GetDocumentId (char *file_name) {
	int id;
	sscanf(file_name, "%d", &id); // Store doc_id into id;
	return id;
}

/*
 * UpdateIndex - updates the InvertedIndex for each word of a document.
 * @word: word to be added to the InvertedIndex.
 * @documentId: document the word comes from.
 * @Index: pointer to the InvertedIndex.
 *
 * Returns 1 if successful, 0 if not successful.
 *
 * Assumptions:
 *     1. InvertedIndex has been initialized.
 *
 * Pseudocode:
 *     1. Check that the args are valid.
 *     2. Add the word to the Index.
 */
 
int UpdateIndex(char *word, int documentId, HashTable *Index) {
	if (!word) {
		return 0;
	}
	
	// Add the word to the Index.
	AddWord(word, documentId, Index);
	
	return 1;
}

/*
 * SaveIndexToFile - outputs the InvertedIndex to a file.
 * @Index: pointer to the InvertedIndex.
 * @file_name: file to be written to.
 *
 * Returns 1 if successful, 0 if not successful.
 *
 * Assumptions:
 *     1. InvertedIndex has been initialized.
 *
 * Pseudocode:
 *     1. Check that the args are valid.
 *     2. Add the word to the Index.
 */

int SaveIndexToFile(HashTable *Index, char *file_name) {
	// Declare and open a stream to file_name.
	FILE *fp;
	fp = fopen(file_name, "w");
	
	WordNode *current; // variable for traversal
	
	// Loop through each bin of the Index.
	for (int i=0; i < MAX_HASH_SLOT; i++) {
	
		// Loop through each Word Node, if it exists.
		for (current = Index->table[i]->data; current != NULL; current = current->next) {
			fprintf(fp, "%s", current->word); // Write word to file.
			
			DocumentNode *ptr; // variable for traversal
			char arr[MAX]={0}; // variable to hold all information about the current word.
			int num=0; // variable to get the number of docs the current word appears in.
			
			// Loop through each document node.
			for (ptr = current->page; ptr != NULL; ptr = ptr->next) {
				char arr2[10];
				sprintf(arr2, "%d %d ", ptr->doc_id, ptr->freq);
				strcat(arr, arr2); // Add doc_id, freq to arr.
				
				num++;
			}
			
			// Write information to file.
			fprintf(fp, " %d", num);
			fprintf(fp, " %s", arr);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	return 1;
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


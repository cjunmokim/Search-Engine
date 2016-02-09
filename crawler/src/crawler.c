/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Caleb Junmo Kim
 * Date: July 25, 2015
 *
 * Input: A seed url to crawl, a directory to write files to, and the depth for crawling.
 *
 * Output: Outputs error messages if boundary cases are reached. Otherwise, there is no
 * 	   output, but instead a file is created in the specified directory for each
 *         url crawled.
 *
 * Error Conditions: 1) Command line arguments are invalid.
 * 		     2) URL is invalid, or dead, or cannot be normalized.
 * 		     3) Memory cannot be allocated sufficiently.
 *
 * Special Considerations: The seed url should contain a '/' at the end for correct parsing.
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

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "cweb.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "chashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions
#define MAX 4 				     // Max depth.

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
int pageID = 1; 			     // pageId for each html
int depth = 0; 				     // depth
char path[6] = {0}; 			     // directory path

// ---------------- Private prototypes
int WriteFile(WebPage *, char *, int); 	     // Function to create a file for each webpage.
int CrawlPage(WebPage *); 		     // Function to crawl a webpage for links.
void FreeWebMemory(WebPage *); 		     // Function to free dynamically allocated WebPage memory.


/* ========================================================================== */

int main(int argc, char* argv[])
{
    // check command line arguments
	
	// Check that there are three input parameters.
	if (argc != 4) {
		printf("Please input three parameters: seed URL, directory, and max depth.\n");
		return 1;
	}
	
	// Check that the seed url has proper domain (old-www).
	if (strncmp(argv[1], URL_PREFIX, 15) != 0) {
		printf("The seed URL domain must be old-www.\n");
		return 1;
	}
	
	// Check that the directory already exists.
	struct stat st;
	if (stat(argv[2], &st) == 0 && S_ISDIR(st.st_mode));
	else { // If the directory does not exist, terminate the program.
		printf("The directory %s cannot be found. Please enter an existing directory.\n", argv[2]);
		return 1;
	}
	
	// Check that the directory path does not have a '/' at the end for ease in writing filenames.
	if (argv[2][strlen(argv[2]) - 1] == '/') {
		printf("Please do not add '/' at the end of the directory path.\n");
		return 1;
	}
	
	// Check the third argument.
	// Loop through each letter of the first argument and check that it is indeed a number.
	for (int i = 0; i < strlen(argv[3]); i++) {
		if (!isdigit((int)argv[3][i])) {
 			printf("Please input a valid number for the depth.\n");
   			return 1;
   		}
	}
   	
  	sscanf(argv[3], "%d", &depth); // Store the argument as an integer.
	
	// Check that the depth specified does not exceed max depth.
	if (depth > MAX) {
		printf("Search depth cannot exceed MAX depth of 4. Please enter a valid depth.\n");
		return 1;
	}
	
    	// init curl
    	curl_global_init(CURL_GLOBAL_ALL);

    	// setup seed page
	WebPage *page = calloc(1, sizeof(WebPage));
	page->url = (char *)malloc(strlen(argv[1]) + 1);
	MALLOC_CHECK(stderr, page->url); // Check that memory was allocated.
	strcpy(page->url, argv[1]); // Copy the seed url to page->url.

    	// get seed webpage
	if (!GetWebPage(page)) {
		printf("The seed URL is invald. Please enter a valid seed URL.\n");
		FreeWebMemory(page);
		return 1;
	}
	
	// Normalize the seed url.
	if (!NormalizeURL(page->url)) {
		printf("Seed URL cannot be normalized.\n");
		FreeWebMemory(page);
		return 1;
	}
	
    	// write seed file
    	strcpy(path, argv[2]); // Let var path contain the directory path.
    	WriteFile(page, path, pageID);
	
    	// add seed page to hashtable
    	if (!AddToHashTable(page->url)) {
		FreeWebMemory(page);
    		return 1;
   	 }
	
	// Initialize URLList.
	if (!InitList()) {
		FreeWebMemory(page);
		return 1;
	}
	
    	// extract urls from seed page
    	if (!CrawlPage(page)) {
    		FreeHashTable(); // Free all memory dynamically allocated to the hash table.
		FreeWebMemory(page);
    		return 1;
    	}

	// while there are urls to crawl
    	while (URLList.tail != NULL) {

    		// get next webpage from list
    		WebPage *next = PopList();

        	// write page file
        	pageID++;
		if (!WriteFile(next, argv[2], pageID)) { // Check that the WriteFile worked.
			FreeWebMemory(next);
			return 1;
		}

        	// extract urls from webpage and then cleanup.
    		CrawlPage(next);
		FreeWebMemory(next);
    	}
    	// Memory cleanup.
	FreeHashTable(); // Free memory dynamically allocated to the hash table.
	FreeWebMemory(page); // Free memory dynamically allocated to the Webpage variable.
	
    	// cleanup curl
    	curl_global_cleanup();
    
    	return 0;
}



// Helper Functions


// Function to write the webpage to a file.
int WriteFile(WebPage *wp, char *path, int pageID) {
	// Declare and initialize a filename variable.
	char *filename = (char *)malloc(20);
    	MALLOC_CHECK(stderr,filename);
    
    	// Write to the filename variable.
	if (sprintf(filename, "%s/%d", path, pageID) == EOF) {
    		printf("There is an error with the directory path.\n");
    		return 0;
    	}
    
    	// Declare and open a filepath with the filename.
    	FILE *fp;
    	fp = fopen(filename, "w+"); // Open a new file by the filename.
	
	// Check that there were no errors opening the file.
   	if (fp == NULL) {
    		printf("Error reading file.\n");
    		return 0;
    	}
    
    	// Write to each file of an html.
    	fprintf(fp, "%s\n%d\n%s", wp->url, wp->depth, wp->html);
    
    	// Cleanup.
	fclose(fp);
	free(filename);
	return 1;
}

// Function to crawl a given webpage for links.
int CrawlPage(WebPage *wp) {
	
	char *result; // variable to hold the url.
    	int pos = 0; // position in each html page.
    	WebPage *newPage; // New webpage.
    
    	// Check that the depth does not exceed the depth passed.
    	if (wp->depth >= depth) {
    		return 0;
    	}
    
    	printf("\n");
    	printf("[crawler]: Crawling - %s\n", wp->url); // Print the url being curled.
    	printf("\n");
    
    	// Loop through each html page to get all its urls.
    	while ((pos = GetNextURL(wp->html, pos, wp->url, &result)) >= 0) {
    
    		// Check that the url has proper domain (old-www).
		if (strncmp(result, URL_PREFIX, strlen(URL_PREFIX)) != 0) {
			free(result);
			continue;
		}
 		
		// Normalize the url.
    		if (!NormalizeURL(result)) {
    			free(result);
    			continue;
    		}
    	
    		// Check that the url isn't already in the hash table.
    		if (!InHashTable(result)) {
    			AddToHashTable(result); // Add the url to the hash table.
    		
    			// Setup new page for each url.
			newPage = calloc(1, sizeof(WebPage));
			newPage->depth = wp->depth + 1;
			newPage->url = (char *)malloc(strlen(result) + 1);
			if (!newPage->url) { // Check that memory was allocated.
				continue;
			}
			strcpy(newPage->url, result);

			// Get html for each url.
			if (!GetWebPage(newPage)) {
				FreeWebMemory(newPage);
				free(result);
				continue;
			}
			
			printf("[crawler]: Parser found link - %s\n", result);
			
			// Add to the list of webpages to be visited.
			if (!AppendList(newPage)) {
				free(result);
				return 0;
			}
    		}
    		free(result);
    	}
	return 1;
}

// Function to free dynamically allocated WebPage memory.
void FreeWebMemory(WebPage * wp) {
	free(wp->html);
	free(wp->url);
	free(wp);
}

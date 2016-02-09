/* ========================================================================== */
/* File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Caleb Junmo Kim
 * Date: 8/2/2015
 *
 * You should include in this file your functionality for the list as
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
#include "list.h"                            // hashtable functionality


// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
List URLList;

// ---------------- Private prototypes
int AppendList(WebPage *);
WebPage *PopList();
int InitList();


// Function to append to the end of the DLL.
int AppendList(WebPage *wp) {

	// Declare variable.
	ListNode *node;
	
	// Check to make sure the webpage is valid.
	if (!wp) {
		return 0;
	}
	
	// Initialize a new node to add.
	node = (ListNode *)calloc(1, sizeof(ListNode));
	if (!node) {
		return 0;
	}
	
	node->page = wp; // Put the Webpage into node->page.
	
	// Case when the list is empty.
	if (URLList.tail == NULL) {
		URLList.head = node;
		URLList.tail = node;
	}
	else { // Case when the list is not empty.
		URLList.tail->next = node;
		node->prev = URLList.tail;
		URLList.tail = URLList.tail->next; // Make the new element the tail of the DLL.
	}
	return 1;
}


// Function to pop the first element of the DLL.
WebPage *PopList() {
	
	// Declare variables.
	ListNode *node;
	WebPage *p;
	
	// Case when the list is empty.
	if (URLList.head == NULL) {
		printf("The List is empty\n");
		return NULL;
	}
	
	// Case when the list has one element.
	if (URLList.head == URLList.tail) {
		URLList.tail = NULL; // Set the tail to null.
	}
	
	node = URLList.head;
	URLList.head = URLList.head->next;
	p = node->page;
	
	// Check that the new head node does not have a previous node.
	if (URLList.head) {
		URLList.head->prev = NULL;
	}
	
	// Free the node.
	free(node);
	node = NULL;

	return p;
}

// Function to initialize the list to null.
int InitList() {

	// Check that the URLList is not empty.
	if (URLList.head || URLList.tail) {
		printf("List is not empty. Empty list first before initializing.\n");
		return 0;
	}
	
	URLList.head = NULL;
	URLList.tail = NULL;
	return 1;
}

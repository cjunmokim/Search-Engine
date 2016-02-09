/* ========================================================================== */
/* File: query_func.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * This is the header file for query_func.c
 *
 */
/* ========================================================================== */
#ifndef QUERY_H
#define QUERY_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables
DocumentNode *temp_list;					 // temp_list
DocumentNode *final_list;					 // final list

// ---------------- Prototypes/Macros
int GetLinks(char *, HashTable *);
void And(char *, HashTable *);
int Or();
void Sort();
int key_compare(const void *, const void *);
int FreeList(int);
int Display();


#endif // QUERY_H

/* ========================================================================== */
/* File: query.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * This file contains the definitions for an inverted index of words.
 *
 */
/* ========================================================================== */
#ifndef QUERY_H
#define QUERY_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

// typedef struct DocumentNode {
//   struct DocumentNode *next;         // pointer to the next member of the list.
//   int doc_id;                        // document identifier
//   int freq;                          // number of occurrences of the word
// } DocumentNode;
// 
// typedef struct WordNode {
//   struct WordNode *next;            // pointer to the next word (for collisions)
//   char *word;                       // the word
//   DocumentNode *page;               // pointer to the first element of the page list.
// } WordNode;
// 
// typedef struct HashTableNode {
//     void *data;                               // generic pointer for any type of node
// } HashTableNode;
// 
// typedef struct HashTable {
//     HashTableNode *table[MAX_HASH_SLOT];     // actual index
// } HashTable;

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


/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

#endif // HASHTABLE_H

// Filename: Test cases for query.c
// Description: A unit test for the query search engine.
// 

//
// Test Harness Spec:
// ------------------
//
//  It uses these functions but they are not unit tested in this test harness:
//
//  int InitializeHashTable(HashTable *);
//  int InHashTable(char *, HashTable *);
//  int CleanHashTable (HashTable *);
//  int FreeHashTable(HashTable *);
//  HashTable *ReadFile(char *, HashTable *);
//  void NormalizeWord(char *);
//  int key_compare(const void *, const void *);
//  int FreeList(int);
// 
//  It tests the following functions:
//
//  int GetLinks(char *, HashTable *);
//  void And(char *, HashTable *);
//  int Or();
//  void Sort();
//  int Display();
//
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.
//
//  Test Cases:
//  -----------
//   
//  The test harness runs a number of test cases to test the code.
//  The approach is to first set up the environment for the test,
//  invoke the function to be tested, then validate the state of
//  the data structures using the SHOULD_BE macro. This is repeated
//  for each test case. 
//
//  The test harness isolates the functions under test and setting
//  up the environment the code expects when integrated in the real
//  system.
// 
//
//
//  The following test cases  (1-7) are for function:
//
//  int GetLinks(char *, HashTable *);
//
//  Test case: GETLINKS:1
//  This test case calls GetLinks() for the condition where the first word passed is an operator.
//  Result is to return 0 and terminate prematurely.
//
//  Test case: GETLINKS:2
//  This test case calls GetLinks() for the condition where query has some word with upper and lowercase letters.
//  GetLinks() should return 1 and final_list should be the same list of docs as if the query were all lowercase.
//
//  Test case: GETLINKS:3
//  This test case calls GetLinks() for the condition where query has some word not in InvertedIndex.
//  GetLinks() should return 1 and final_list should be NULL.
//
//  Test case: GETLINKS:4
//  This test case calls GetLinks() where the query contains "OR", ie libation OR hindrance.
//  Libation appears once in doc #1467, and hindrance appears once in doc #259 and in doc #891.
//  GetLinks() should return 1 and final_list should contain the above three nodes.
//
//  Test case: GETLINKS:5
//  This test case calls GetLinks() where the query contains "AND", ie squash AND whale.
//  squash and whale appear together in doc #1130 only.
//  GetLinks() should return 1 and final_list should contain the above node.
//
// Test case: GETLINKS:6
// This test case calls GetLinks() where the query contains no operator, so AND is implied, ie squash whale.
// squash and whale appear together in doc #1130 only.
// GetLinks() should return 1 and final_list should contain the above node.
//
// Test case: GETLINKS:7
// This test case calls GetLinks() where the query contains multiple AND and OR operators.
// GetLinks() should return 1 and final_list should not be NULL.
//
//
//  The following test cases (1-3) for function:
//
//  void And(char *, HashTable *);
//
//  Test case: AND:1
//  This test case calls And() for the condition where the temp_list is NULL.
//  temp_list will be NULL.
//
//  Test case: AND:2
//  This test case calls And() for the condition where word is not in InvertedIndex.
//  The word "bumble" passed below does not exist in the InvertedIndex.
//  temp_list will be NULL.
//
//  Test case: AND:3
//  This test case calls And() for the condition where word is in InvertedIndex, and there is a match.
//  Thus, the temp_list will remain the same, and its frequency / rank will be the sum.
//  "libation" appears in one document #1467 once.
//
//
//  The following test cases (1) for function:
//
//  int Or();
//
//  Test case: OR:1
//  This test case calls Or() for the condition where the temp_list is NULL.
//  temp_list will be NULL.
//
//  Test case: OR:2
//  This test case calls Or() for the condition where temp_list and final_list share the same node.
//  Then, the rank should be the higher of the two.
//  temp_list will be NULL.
//
//
//  The following test cases (1) for function:
//
//  void Sort();
//
//  Test case: SORT:1
//  This test case calls Sort() where final_list contains a list of DocumentNodes.
//  final_list will be sorted by rank from highest to lowest.
//
//
//  The following test cases (1) for function:
//
//  int Display();
//
//  Test case: DISPLAY:1
//  This test case calls Display() where final_list contains a list of DocumentNodes.
//  Each DocumentNode of final_list will be printed in "DOCUMENT ID:  URL:  " format.
//  Returns 1 upon completion.





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h> 				 // math functionality
#include "../src/qhashtable.h" 						 // hashtable functionality
#include "../src/file.h" 							 // file functionality
#include "../src/qweb.h" 								// web/html functionality
#include "../src/query_func.h" 							 // query functionality

// Useful MACROS for controlling the unit tests.

// each test should start by setting the result count to zero

#define START_TEST_CASE  int rs=0

// check a condition and if false print the test condition failed
// e.g., SHOULD_BE(dict->start == NULL)
// note: the construct "#x" below is the sstringification preprocessor operator that
//       converts the argument x into a character string constant

#define SHOULD_BE(x) if (!(x))  {rs=rs+1; \
    printf("Line %d test [%s] Failed\n", __LINE__,#x); \
  }


// return the result count at the end of a test

#define END_TEST_CASE return rs

//
// general macro for running a best
// e.g., RUN_TEST(TestDAdd1, "DAdd Test case 1");
// translates to:
// if (!TestDAdd1()) {
//     printf("Test %s passed\n","DAdd Test case 1");
// } else { 
//     printf("Test %s failed\n", "DAdd Test case 1");
//     cnt = cnt +1;
// }
//

#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}


// Global variables.
HashTable Index;
HashTable *ptr;
char *file = "../../indexer/index.dat";
char *dir_path = "../../crawler/data";


// Test case: GETLINKS:1
// This test case calls GetLinks() for the condition where the first word passed is an operator.
// Result is to return 0 and terminate prematurely.

int TestGETLINKS1() {
  START_TEST_CASE;
  
  char *query = "AND life"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list == NULL);
  SHOULD_BE(result == 0);
  
  // Cleanup.
  FreeList(1);
  
  
  END_TEST_CASE;
}


// Test case: GETLINKS:2
// This test case calls GetLinks() for the condition where query has some word with upper and lowercase letters.
// GetLinks() should return 1 and final_list should be the same list of docs as if the query were all lowercase.

int TestGETLINKS2() {
  START_TEST_CASE;
  
  char *query = "liBaTion"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list != NULL);
  SHOULD_BE(result == 1);
  
  DocumentNode *cpy = final_list;
  
  char *query2 = "libation";
  GetLinks(query2, ptr);
  SHOULD_BE(final_list != NULL);
  
  DocumentNode *ptr, *ptr2;
  for (ptr = cpy; ptr != NULL; ptr = ptr->next) {
  	for (ptr2 = final_list; ptr2 != NULL; ptr2 = ptr2->next) {
  		SHOULD_BE(ptr->doc_id == ptr2->doc_id);
  		SHOULD_BE(ptr->freq == ptr2->freq);
  	}
  }
  
  // Cleanup.
  FreeList(1);
  final_list = cpy;
  FreeList(1);
  
  END_TEST_CASE;
}


// Test case: GETLINKS:3
// This test case calls GetLinks() for the condition where query has some word not in InvertedIndex.
// GetLinks() should return 1 and final_list should be NULL.

int TestGETLINKS3() {
  START_TEST_CASE;
    
  char *query = "bumblearhr"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list == NULL);
  SHOULD_BE(result == 1);
  END_TEST_CASE;
}


// Test case: GETLINKS:4
// This test case calls GetLinks() where the query contains "OR", ie libation OR hindrance.
// Libation appears once in doc #1467, and hindrance appears once in doc #259 and in doc #891.
// GetLinks() should return 1 and final_list should contain the above three nodes.

int TestGETLINKS4() {
  START_TEST_CASE;
  
  char *query = "libation OR hindrance"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list != NULL);
  
  DocumentNode *ptr;
  int i = 0;
  int arr[3] = {1467, 259, 891};
  for (ptr = final_list; ptr != NULL; ptr = ptr->next) {
  	SHOULD_BE(ptr->doc_id == arr[i] && ptr->freq == 1);
  	i++;
  }
  
  SHOULD_BE(result == 1);
  
  FreeList(1);
  END_TEST_CASE;
}


// Test case: GETLINKS:5
// This test case calls GetLinks() where the query contains "AND", ie squash AND whale.
// squash and whale appear together in doc #1130 only.
// GetLinks() should return 1 and final_list should contain the above node.

int TestGETLINKS5() {
  START_TEST_CASE;
  
  char *query = "squash AND whale"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list != NULL);
  SHOULD_BE(final_list->doc_id == 1130);
  SHOULD_BE(final_list->next == NULL);
  SHOULD_BE(result == 1);
  
  FreeList(1); // Cleanup.
  END_TEST_CASE;
}


// Test case: GETLINKS:6
// This test case calls GetLinks() where the query contains no operator, so AND is implied, ie squash whale.
// squash and whale appear together in doc #1130 only.
// GetLinks() should return 1 and final_list should contain the above node.

int TestGETLINKS6() {
  START_TEST_CASE;
  
  char *query = "squash whale"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list != NULL);
  SHOULD_BE(final_list->doc_id == 1130);
  SHOULD_BE(final_list->next == NULL);
  SHOULD_BE(result == 1);
  
  FreeList(1);
  END_TEST_CASE;
}


// Test case: GETLINKS:7
// This test case calls GetLinks() where the query contains multiple AND and OR operators.
// GetLinks() should return 1 and final_list should not be NULL.

int TestGETLINKS7() {
  START_TEST_CASE;
  
  char *query = "squash AND whale OR libation"; // The query we want to test with.
  
  int result = GetLinks(query, ptr);
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list != NULL);
  SHOULD_BE(final_list->doc_id == 1130);
  SHOULD_BE(final_list->next->doc_id == 1467);
  SHOULD_BE(final_list->next->next == NULL);
  SHOULD_BE(result == 1);
  
  FreeList(1);
  END_TEST_CASE;
}


// Test case: AND:1
// This test case calls And() for the condition where the temp_list is NULL.
// temp_list will be NULL.

int TestAND1() {
  START_TEST_CASE;
  
  char *word = "life";
  temp_list = NULL;
  And(word, ptr);
  
  SHOULD_BE(temp_list == NULL);
  
  END_TEST_CASE;
}


// Test case: AND:2
// This test case calls And() for the condition where word is not in InvertedIndex.
// The word "bumble" passed below does not exist in the InvertedIndex.
// temp_list will be NULL.

int TestAND2() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 2;
  
  char *word = "bumble";
  temp_list = dn;
  And(word, ptr);

  SHOULD_BE(temp_list == NULL);
  
  
  END_TEST_CASE;
}


// Test case: AND:3
// This test case calls And() for the condition where word is in InvertedIndex, and there is a match.
// Thus, the temp_list will remain the same, and its frequency / rank will be the sum.
// "libation" appears in one document #1467 once.

int TestAND3() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 2;
  
  char *word = "libation"; // The query we want to test with.
  temp_list = dn;
  And(word, ptr);
  
  SHOULD_BE(temp_list != NULL);
  SHOULD_BE(temp_list->doc_id == 1467);
  SHOULD_BE(temp_list->freq == 3); // Frequency is the sum of the two.
  
  free(dn); // Cleanup.
  END_TEST_CASE;
}


// Test case: OR:1
// This test case calls Or() for the condition where the temp_list is NULL.
// temp_list will be NULL.

int TestOR1() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 2;
  temp_list = dn;
  
  final_list = NULL;
  
  Or();
  
  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list->doc_id == 1467);
  SHOULD_BE(final_list->freq == 2);
  
  free(dn); // Cleanup.
  
  END_TEST_CASE;
}


// Test case: OR:2
// This test case calls Or() for the condition where temp_list and final_list share the same node.
// Then, the rank should be the higher of the two.
// temp_list will be NULL.

int TestOR2() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 2;
  temp_list = dn;
  
  DocumentNode *dn2 = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn2->doc_id = 1467;
  dn2->freq = 8;
  final_list = dn2;
  
  Or();

  SHOULD_BE(temp_list == NULL);
  SHOULD_BE(final_list->doc_id == 1467);
  SHOULD_BE(final_list->freq == 8); // Frequency is the higher of the two.
  
  free(dn2);
  
  END_TEST_CASE;
}


// Test case: SORT:1
// This test case calls Sort() where final_list contains a list of DocumentNodes.
// final_list will be sorted by rank from highest to lowest.

int TestSORT1() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 4;
  final_list = dn;
  
  dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1460;
  dn->freq = 10;
  final_list->next = dn;
  
  dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 160;
  dn->freq = 1;
  final_list->next->next = dn;
  
  
  Sort();
  
  SHOULD_BE(final_list->doc_id == 1460);
  SHOULD_BE(final_list->next->doc_id == 1467);
  SHOULD_BE(final_list->next->next->doc_id == 160);
  
  FreeList(1); // Cleanup.
  
  END_TEST_CASE;
}


// Test case: DISPLAY:1
// This test case calls Display() where final_list contains a list of DocumentNodes.
// Each DocumentNode of final_list will be printed in "DOCUMENT ID:  URL:  " format.
// Returns 1 upon completion.

int TestDISPLAY1() {
  START_TEST_CASE;
  
  DocumentNode *dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1467;
  dn->freq = 4;
  final_list = dn;
  
  dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 1460;
  dn->freq = 10;
  final_list->next = dn;
  
  dn = (DocumentNode *)calloc(1, sizeof(DocumentNode));
  dn->doc_id = 160;
  dn->freq = 1;
  final_list->next->next = dn;
  
  
  int result = Display();
  
  SHOULD_BE(result == 1);
  
  FreeList(1); // Cleanup.
  
  END_TEST_CASE;
}



// Main program.
int main(int argc, char** argv) {
  int cnt = 0;
	
  // Load and recreate an InvertedIndex from index file.
  InitializeHashTable(&Index);
  ptr = ReadFile(file, &Index);
  
  // Perform the tests.
  RUN_TEST(TestGETLINKS1, "GetLinks Test case 1");
  RUN_TEST(TestGETLINKS2, "GetLinks Test case 2");
  RUN_TEST(TestGETLINKS3, "GetLinks Test case 3");
  RUN_TEST(TestGETLINKS4, "GetLinks Test case 4");
  RUN_TEST(TestGETLINKS5, "GetLinks Test case 5");
  RUN_TEST(TestGETLINKS6, "GetLinks Test case 6");
  RUN_TEST(TestGETLINKS7, "GetLinks Test case 7");
  
  RUN_TEST(TestAND1, "And Test case 1");
  RUN_TEST(TestAND2, "And Test case 2");
  RUN_TEST(TestAND3, "And Test case 3");
  
  RUN_TEST(TestOR1, "Or Test case 1");
  RUN_TEST(TestOR2, "Or Test case 2");
  
  RUN_TEST(TestSORT1, "Sort Test case 1");
  
  RUN_TEST(TestDISPLAY1, "Display Test case 1");
  
  
  // Cleanup.
  CleanHashTable(ptr);
  FreeHashTable(ptr);
  
  if (!cnt) {
    printf("All passed!\n"); return 0;
  } else {
    printf("Some fails!\n"); return 1;
  }
}


/*
 * list.h
 *
 * Description: Header file for list.c . Doubly linked list data structure, function declarations, etc. 
 * Author: Zeth Santos
 * Created: Oct 9 2023
*/


// List data type
#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>

#define LIST_SUCCESS 0
#define LIST_FAIL -1

typedef struct Node_s Node;
struct Node_s {
    void* data;
    int next;  // Index to the next node
    int prev;  // Index to the previous node
};

enum ListOutOfBounds {
    LIST_OOB_START,
    LIST_OOB_END
};
typedef struct List_s List;
struct List_s {
    int start;  // Index to the first node
    int end;    // Index to the last node
    int curr;   // Index to the current node
    int count;  // Number of elements in the list
};

// Max number of unique lists
#define LIST_MAX_NUM_HEADS 10

// Max number of nodes over all lists
#define LIST_MAX_NUM_NODES 100

// Create new list
List* List_create();

// Count items in list
int List_count(List* pList);

// Get first item, set to current item
void* List_first(List* pList);

// Get the last item, set it as the current item
void* List_last(List* pList); 

// Advance to the next item in the list
void* List_next(List* pList);

// Get previous item in the list
void* List_prev(List* pList);

// Get the current item in the list
void* List_curr(List* pList);

// Insert item after the current item
int List_insert_after(List* pList, void* pItem);

// Function to insert an item before the current item
int List_insert_before(List* pList, void* pItem);

// Function to append an item to the end of the list
int List_append(List* pList, void* pItem);

// Function to prepend an item to the start of the list
int List_prepend(List* pList, void* pItem);

// Function to remove the current item from the list
void* List_remove(List* pList);

// Function to trim the last item from the list
void* List_trim(List* pList);

// Function to concatenate two lists
void List_concat(List* pList1, List* pList2);

// Typedef for the function to free an item's memory
typedef void (*FREE_FN)(void* pItem);

// Function to free the list and its items
void List_free(List* pList, FREE_FN pItemFreeFn);

// Typedef for the comparator function
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);

// Function to search for an item in the list
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);

#endif
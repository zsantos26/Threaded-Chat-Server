/*
 * list.h
 *
 * Description: Header file for list.c . Doubly linked list data structure, function declarations, etc. 
 * 
 * Author: Zeth Santos
 * Date: Oct 9 2023
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

// Create new lis
List* List_create();

// Count items in list
int List_count(List* pList);
  
 
#endif
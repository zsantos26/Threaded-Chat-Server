/*
 * list.c
 *
 * Description: Implementing functions declared in list.h Doubly linked list data structure, function declarations, etc. 
 * 
 * Author: Zeth Santos
 * Date: Oct 10 2023
*/

#include "list.h"
#include <assert.h>
#include <stdio.h>

// Static arrays for nodes and heads
static Node nodes[LIST_MAX_NUM_NODES];
static List lists[LIST_MAX_NUM_HEADS];

// Free lists
static int freeNodes[LIST_MAX_NUM_NODES];
static int freeNodeTop = -1;  // Top of the stack for free nodes

static int freeHeads[LIST_MAX_NUM_HEADS];
static int freeHeadTop = -1;  // Top of the stack for free lists

// Create new list
List* List_create() {
    static bool initialized = false;
    if (!initialized) {
        // Populate free lists
        for (int i = 0; i < LIST_MAX_NUM_NODES; i++) {
            freeNodes[++freeNodeTop] = i;
        }
        for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            freeHeads[++freeHeadTop] = i;
        }
        initialized = true;
    }

    if (freeHeadTop == -1) {
        return NULL;  // No free lists available
    }
    int newListIndex = freeHeads[freeHeadTop--];
    List* newList = &lists[newListIndex];
    newList->start = -1;
    newList->end = -1;
    newList->curr = -1;
    newList->count = 0;
    return newList;
}

// Count items in list
int List_count(List* pList) {
    assert(pList != NULL);
    return pList->count;
}

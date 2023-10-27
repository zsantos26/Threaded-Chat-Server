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


void* List_first(List* pList) {
    assert(pList != NULL);
    if (pList->start == -1) {
        pList->curr = -1;
        return NULL;
    }
    pList->curr = pList->start;
    return nodes[pList->curr].data;
}


void* List_last(List* pList) {
    assert(pList != NULL);
    if (pList->end == -1) {
        pList->curr = -1;
        return NULL;
    }
    pList->curr = pList->end;
    return nodes[pList->curr].data;
}

void* List_next(List* pList) {
    assert(pList != NULL);
    if (pList->curr == -1 || nodes[pList->curr].next == -1) {
        pList->curr = -1;
        return NULL;
    }
    pList->curr = nodes[pList->curr].next;
    return nodes[pList->curr].data;
}


void* List_prev(List* pList) {
    assert(pList != NULL);
    if (pList->curr == -1 || nodes[pList->curr].prev == -1) {
        pList->curr = -1;
        return NULL;
    }
    pList->curr = nodes[pList->curr].prev;
    return nodes[pList->curr].data;
}


void* List_curr(List* pList) {
    assert(pList != NULL);
    if (pList->curr == -1) {
        return NULL;
    }
    return nodes[pList->curr].data;
}

int List_insert_after(List* pList, void* pItem) {
    assert(pList != NULL);
    if (freeNodeTop == -1) {
        return LIST_FAIL;  // No free nodes available
    }
    int newNodeIndex = freeNodes[freeNodeTop--];
    nodes[newNodeIndex].data = pItem;
    if (pList->curr == -1) {
        // If no current item, insert at the end.
        nodes[newNodeIndex].next = -1;
        nodes[newNodeIndex].prev = -1;
        pList->start = newNodeIndex;
        pList->end = newNodeIndex;
    } else {
        // Insert after the current item.
        nodes[newNodeIndex].next = nodes[pList->curr].next;
        nodes[newNodeIndex].prev = pList->curr;
        if (nodes[pList->curr].next != -1) {
            nodes[nodes[pList->curr].next].prev = newNodeIndex;
        } else {
            pList->end = newNodeIndex;  // Update end if inserting at the end.
        }
        nodes[pList->curr].next = newNodeIndex;
    }
    pList->curr = newNodeIndex;  // Update current to the newly inserted item.
    pList->count++;
    return LIST_SUCCESS;
}

int List_insert_before(List* pList, void* pItem) {
    assert(pList != NULL);
    
    if (freeNodeTop == -1) {
        return LIST_FAIL; // No free nodes available
    }

    int newNodeIndex = freeNodes[freeNodeTop--];
    nodes[newNodeIndex].data = pItem;

    if (pList->curr == -1) {
        // If no current item, insert at the start.
        nodes[newNodeIndex].prev = -1;
        nodes[newNodeIndex].next = pList->start;
        
        if (pList->start != -1) {
            nodes[pList->start].prev = newNodeIndex;  // Update end if list was initially empty.
        } else {
            pList->end = newNodeIndex;
        }
        
        pList->start = newNodeIndex;
    } else {
        // Insert before the current item.
        nodes[newNodeIndex].prev = nodes[pList->curr].prev;
        nodes[newNodeIndex].next = pList->curr;
        nodes[pList->curr].prev = newNodeIndex;
        
        if (nodes[newNodeIndex].prev != -1) {
            nodes[nodes[newNodeIndex].prev].next = newNodeIndex;
        } else {
            pList->start = newNodeIndex;  // Update start if inserting at the beginning.
        }
    }

    pList->curr = newNodeIndex;  // Update current to the newly inserted item.
    pList->count++;
    return LIST_SUCCESS;
}

int List_append(List* pList, void* pItem) {
    assert(pList != NULL);
    if (freeNodeTop == -1) {
        return LIST_FAIL; // No free nodes available
    }

    // Get an available node
    int newNodeIndex = freeNodes[freeNodeTop--];
    nodes[newNodeIndex].data = pItem;
    nodes[newNodeIndex].next = -1; // It will be the last node
    nodes[newNodeIndex].prev = pList->end;
    
    if (pList->end != -1) {
        nodes[pList->end].next = newNodeIndex; // old last node's next will be this
    }
    pList->end = newNodeIndex; // new last node
    
    if (pList->start == -1) { // If the list was empty
        pList->start = newNodeIndex;
    }

    // Make the new node the current item and update the count
    pList->curr = newNodeIndex;
    pList->count++;
    return LIST_SUCCESS;
}


int List_prepend(List* pList, void* pItem) {
    assert(pList != NULL);

    // Check for available nodes
    if (freeNodeTop == -1) {
        return LIST_FAIL;  // No free nodes available
    }
    int newNodeIndex = freeNodes[freeNodeTop--];
    nodes[newNodeIndex].data = pItem;

    // If the list is empty, set start, end, and current pointers
    if (pList->start == -1) {  // If the list is empty
        nodes[newNodeIndex].next = -1;
        nodes[newNodeIndex].prev = -1;
        pList->start = newNodeIndex;
        pList->end = newNodeIndex;
        pList->curr = newNodeIndex;
    } else {
        // Otherwise, link the new node at the start
        nodes[newNodeIndex].next = pList->start;
        nodes[newNodeIndex].prev = -1;
        nodes[pList->start].prev = newNodeIndex;
        pList->start = newNodeIndex;
        pList->curr = newNodeIndex;
    }
    pList->count++;
    return LIST_SUCCESS;
}
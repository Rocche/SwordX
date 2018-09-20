#ifndef SORTED_LIST_H_INCLUDED
#define SORTED_LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
 
 //  stuctures

    typedef struct occurrencyNode{
        int occurrency;
        struct sortedNode* first;
    }occurrencyNode;

    typedef struct sortedNode{
        char* word;
        struct sortedNode* next;
    }sortedNode;

//  sorted list management functions

    occurrencyNode* create_sl_root (occurrencyNode*);
    occurrencyNode* reallocate_sl_root(occurrencyNode*);
    occurrencyNode* create_occurrencyNode(occurrencyNode*, int occurrency);
    sortedNode* create_sortedNode (sortedNode*,char*);

#endif // SORTED_LIST_H_INCLUDED

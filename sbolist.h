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

    occurrencyNode* create_occurrencyNode(int occurrency);
    sortedNode* create_sortedNode (char*);
    int compare_occurrencyNodes (const void*,const void*);
    void print_sorted_list(FILE* ,occurrencyNode**);

#endif // SORTED_LIST_H_INCLUDED

#ifndef SORTED_LIST_H_INCLUDED
#define SORTED_LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

/*
sl_root is the root node of the sorted list.
It contains the number of occurency groups (elements) and an array of occurency nodes (representing occurencies groups).
sl_root -> oc_nodes[n_oc_nodes] -> chain_list_of_sl_nodes
*/
typedef struct sl_root {
    int elements;
    struct occurrencyNode *oc_nodes;
} sl_root;

/*
The occurencyNode represents a group of words with same occurencies. Words are stored in sortedNode structures.
occurencyNode.occurrency represents the group's occurency value.
occyrencyNode.first is a pointer to the first element of the linked list composed by sortedNode structures.
*/
typedef struct occurrencyNode {
    int occurrency;
    struct sortedNode *first;
} occurrencyNode;

/*
sortedNode represents a node of an occurency group's linked list.
sortedNode.word is the word token from the trie structure.
sortedNode.next is a pointer to the next node of the linked list.
*/
typedef struct sortedNode {
    char *word;
    struct sortedNode *next;
} sortedNode;

/*method used to create the sl_root structure*/
sl_root *create_sl_root(void);

/*method used to create the occurencyNode structure*/
occurrencyNode *create_occurrencyNode(int occurrency);

/*method used to create the sortedNode structure*/
sortedNode *create_sortedNode(char *);

/*method used by the qsort function. It compares occurencies of a pair of nodes*/
int compare_occurrencyNodes(const void *, const void *);

/*method used to print the sorted list in the output file*/
void print_sorted_list(FILE *, sl_root*);

/*method used to free all the sorted list structure's allocated memory*/
void destroy_sorted_list(sl_root*);

#endif // SORTED_LIST_H_INCLUDED

#include "memory_operations.h"
#include "sbolist.h"

/*Creates sorted list's root*/
sl_root* create_sl_root(){
    /*Memory allocation*/
    sl_root *sl_root;
    sl_root = malloc_object(sl_root, sizeof(sl_root));
    /*Initializes attributes*/
    sl_root->oc_nodes = malloc_object(sl_root->oc_nodes, sizeof(occurrencyNode));
    sl_root->elements = 0;
}

/*Creates an occurency node*/
occurrencyNode *create_occurrencyNode(int occurrency)
{
    /*Memory allocation*/
    occurrencyNode *oc_node;
    oc_node = malloc_object(oc_node, sizeof(occurrencyNode));
    /*Initializes attributes*/
    oc_node->occurrency = occurrency;
    oc_node->first = NULL;
    return oc_node;
}

/*Creates a sortedNode*/
sortedNode *create_sortedNode(char *word)
{
    /*Memory allocation*/
    sortedNode *sl_node;
    sl_node = malloc_object(sl_node, sizeof(sortedNode));
    /*Initializes attrivutes*/
    sl_node->word = word;
    sl_node->next = NULL;
    return sl_node;
}

/*
Method used by qsort to decide the order of a pair of nodes,
if returns positive value, qsort will change the order
*/
int compare_occurrencyNodes(const void *node1, const void *node2)
{
    return ((occurrencyNode *)node1)->occurrency -
           ((occurrencyNode *)node2)->occurrency;
}

/*prints trie's words in alphabetical order*/
void print_sorted_list(FILE *dest_fp, sl_root *sl_root)
{
    /*check all occurency nodes*/
    for ( int i=0; i<sl_root->elements ; i++ ){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]); 
        /*take all current occurrency node's sl_nodes*/       
        sortedNode *sl_node = oc_node.first;
        while (sl_node != NULL)
        {
            /*print the line in output file */
            fprintf(dest_fp, "%s: %i\n", sl_node->word, oc_node.occurrency);
            sl_node = sl_node->next;
        }
    }
}

/*free sorted list's allocated memory*/
void destroy_sorted_list(sl_root *sl_root)
{
    /*take occurency nodes one by one*/
    for ( int i=0; i<sl_root->elements ; i++ ){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]);   
        /*take the first sl_node of the current occurency group*/     
        sortedNode *sl_node = oc_node.first;
        sortedNode* next;
        /*take all sl_nodes of current occurency group*/
        while (sl_node != NULL)
        {
            next = sl_node->next;
            /*free sl_node allocated memory*/
            free(sl_node);
            sl_node = next;
        }
    }
    /*at the end, free the sorted list's root*/
    free(sl_root);
}
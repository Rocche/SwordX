#include "utils.h"
#include "sbolist.h"

sl_root* create_sl_root(){
    sl_root *sl_root;
    sl_root = malloc(sizeof(sl_root));
    check_heap(sl_root);
    sl_root->oc_nodes = malloc(sizeof(occurrencyNode));
    sl_root->elements = 0;
}

occurrencyNode *create_occurrencyNode(int occurrency)
{
    occurrencyNode *oc_node;
    oc_node = malloc(sizeof(occurrencyNode));
    check_heap(oc_node);
    oc_node->occurrency = occurrency;
    oc_node->first = NULL;
    return oc_node;
}

sortedNode *create_sortedNode(char *word)
{
    sortedNode *sl_node;
    sl_node = malloc(sizeof(sortedNode));
    check_heap(sl_node);
    sl_node->word = word;
    sl_node->next = NULL;
    return sl_node;
}

int compare_occurrencyNodes(const void *node1, const void *node2)
{
    return ((occurrencyNode *)node1)->occurrency -
           ((occurrencyNode *)node2)->occurrency;
}

void print_sorted_list(FILE *dest_fp, sl_root *sl_root)
{
    for ( int i=0; i<sl_root->elements ; i++ ){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]);        
        sortedNode *sl_node = oc_node.first;
        while (sl_node != NULL)
        {
            fprintf(dest_fp, "\t %i %s\n", oc_node.occurrency, sl_node->word);
            sl_node = sl_node->next;
        }
    }
}
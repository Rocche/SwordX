#include "memory_operations.h"
#include "sbolist.h"

/*metodo che crea la radice della sorted list*/
sl_root* create_sl_root(){
    /*allocazione della memoria con dimensione della struttura sl_root*/
    sl_root *sl_root;
    sl_root = malloc_object(sl_root, sizeof(sl_root));
    /*inizializzazione degli attributi della struttura sl_root*/
    sl_root->oc_nodes = malloc_object(sl_root->oc_nodes, sizeof(occurrencyNode));
    sl_root->elements = 0;
}

/*metodo che crea un occurency node*/
occurrencyNode *create_occurrencyNode(int occurrency)
{
    /*allocazione della memoria*/
    occurrencyNode *oc_node;
    oc_node = malloc_object(oc_node, sizeof(occurrencyNode));
    /*inizializzazione attributi*/
    oc_node->occurrency = occurrency;
    oc_node->first = NULL;
    return oc_node;
}

/*metodo che crea un sorted node*/
sortedNode *create_sortedNode(char *word)
{
    /*allocazione in memoria*/
    sortedNode *sl_node;
    sl_node = malloc_object(sl_node, sizeof(sortedNode));
    /*inizializzazione attributi*/
    sl_node->word = word;
    sl_node->next = NULL;
    return sl_node;
}

/*
metodo usato dal qsort per la comparazione tra due nodi
se ritorna un valore negativo, l'ordine dei due nodi rimane lo stesso
altrimenti viene cambiato dal qsort
*/
int compare_occurrencyNodes(const void *node1, const void *node2)
{
    return ((occurrencyNode *)node1)->occurrency -
           ((occurrencyNode *)node2)->occurrency;
}

/*metodo che stampa nel file di destinazione il trie (nel caso di ordine alfabetico)*/
void print_sorted_list(FILE *dest_fp, sl_root *sl_root)
{
    /*scorre tutti gli occurrency node contenuti nel root della sorted list*/
    for ( int i=0; i<sl_root->elements ; i++ ){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]); 
        /*si scorrono tutti i sorted node dell'occurency node di riferimento*/       
        sortedNode *sl_node = oc_node.first;
        while (sl_node != NULL)
        {
            /*si stampa la linea nel file */
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
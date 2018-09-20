#include "sbolist.h"

occurrencyNode* create_sl_root (occurrencyNode* sl_root) {

    sl_root = malloc(sizeof(occurrencyNode*));
    if (sl_root == NULL) {
        perror("Error: malloc failed for sl_root");
        exit(EXIT_FAILURE);
    }
    return sl_root;
}

occurrencyNode* reallocate_sl_root (occurrencyNode* sl_root) {

    sl_root = realloc(sl_root, sizeof(sl_root)+sizeof(occurrencyNode*));
    if (sl_root == NULL) {
        perror("Error: realloc failed for sl_root");
        exit(EXIT_FAILURE);
    }
    return sl_root;
}

occurrencyNode* create_occurrencyNode (occurrencyNode* oc_node, int occurrency) {

    oc_node = malloc(sizeof(occurrencyNode));
    if (oc_node == NULL) {
        perror("Error: malloc failed for oc_node");
        exit(EXIT_FAILURE);
    }
    oc_node->occurrency=occurrency;
    oc_node->first = NULL;
    return oc_node;
}

sortedNode* create_sortedNode (sortedNode* sl_node, char* word) {

    sl_node = malloc(sizeof(sortedNode));
    if (sl_node == NULL) {
        perror("Error: malloc failed for sl_node");
        exit(EXIT_FAILURE);
    }
    sl_node->word = word;
    sl_node->next = NULL;
    return sl_node;
}


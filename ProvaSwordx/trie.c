#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sbolist.h"
#include "trie.h"


void increase_occurrencies (trieNode *node) { node->occurrencies++; }
int get_occurrencies (trieNode *node) { return node->occurrencies; }

trieNode* create_trieNode (trieNode* t_node) {

    t_node = malloc(sizeof(trieNode));
    if (t_node == NULL) {
        perror("Error: malloc failed for t_node\n");
        exit(EXIT_FAILURE);
    }
    t_node->occurrencies = 0;
    for (int i = 0; i <= CHARSET; i++) {
        t_node->children[i] = NULL;
    }
    return t_node;
}

void add_word (trieNode *node, char *str, char* trie_word,int index) {

    char c = *(str+index);
    *(trie_word+index) = c;

    if (node->children[charset_to_int(c)] != NULL) {
        if (strlen(str) > strlen(trie_word)) {
            index++;
            add_word(node->children[charset_to_int(c)],str,trie_word,index);
        } else if (strlen(str) == strlen(trie_word)) {
            increase_occurrencies(node->children[charset_to_int(c)]);
            index=0;
        }
    } else {
        node = add_nodes(node,str,index);
        index=0;
    }
}

trieNode* add_nodes (trieNode* node, char* str,int index) {

    trieNode* new_node = create_trieNode(new_node);
    node->children[charset_to_int(*(str+index))] = new_node;
    index++;
    if (*(str+index) != '\0') {
        add_nodes(new_node,str,index);
    } else {
        increase_occurrencies(new_node);
    }
    return node;
}

void print_trie(FILE *file, trieNode *node, char* trie_word, int index) {

    for ( int i=0 ; i<=CHARSET ; i++ ) {
        if ( node->children[i] != NULL ) {
            *(trie_word+index) = int_to_charset(i);
            if ( get_occurrencies(node->children[i]) != 0 ) {
                trie_word[index+1]='\0';
                fprintf(file, "%s: %i\n", trie_word, get_occurrencies(node->children[i]));
            }
            index++;
            print_trie(file, node->children[i],trie_word,index);
            index--;
        }
    }
}

occurrencyNode** sort_trie_by_occurrencies (occurrencyNode** sl_root, trieNode* t_node, char* trie_word, int index) {

    for ( int i=CHARSET ; i>=0 ; i-- ) {
        if ( t_node->children[i] != NULL) {
            *(trie_word+index) = int_to_charset(i);
            index++;
            sort_trie_by_occurencies(sl_root, t_node->children[i],trie_word,index);
        }
        if ( t_node->occurrencies != 0 ) {
            sl_root = add_to_sbolist(sl_root,trie_word, t_node->occurrencies);
        }
        index--;
    }
    return sl_root;
}


occurrencyNode** add_to_sbolist (occurrencyNode** sl_root, char* word, int occurrencies) {

    for ( int i=0 ; i<=sizeof(sl_root) ; i+sizeof(occurrencyNode*)){
        if ( *sl_root+i == occurrencies) {
            oc_node-
            (*sl_root+i)->first            
        }
    }
    occurrencyNode* oc_node = create_occurrencyNode(oc_node,occurrencies);
    sl_root = reallocate_sl_root(sl_root);
    *(sl_root+sizeof(sl_root)) = oc_node;
    return sl_root;
}

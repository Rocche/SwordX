#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"

//  stuctures

    typedef struct trieNode{
        int occurrencies;
        struct trieNode* children[CHARSET];
    }trieNode;



//  trie management functions

    void increase_occurrencies(trieNode*);
    int get_occurrencies(trieNode*);

    trieNode* create_trieNode(trieNode*);
    void add_word(trieNode*,char*,char*,int);
    trieNode* add_nodes(trieNode*, char*,int);
    void print_trie(FILE*, trieNode*,char*,int);
    occurrencyNode** sort_trie_by_occurrencies(occurrencyNode**,trieNode*,char*,int);
    occurrencyNode** add_to_sbolist(occurrencyNode**,char*,int);
    void print_sorted_list(void);

#endif // TRIE_H_INCLUDED

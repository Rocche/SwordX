#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdio.h>
#include "utils.h"

typedef struct trieNode{
    struct trieNode* children[CHARSET];
    int occurrencies;
}trieNode;

typedef struct sortedList{
    char *occurrencies;
}sortedList;

static trieNode *trie_root; // trie root node
static struct sortedList *sl_root; // sorted list root node

static char *trie_word; // word built visiting node after node
static size_t tw_len; // trie word length

static int occurrencies;

trieNode* create_trie(void);
void reallocate_tw(size_t,int);
void add_word(char*, trieNode*);
void increase_occurrencies(trieNode*);
void get_occurrencies(trieNode*);
void add_nodes(char*, trieNode*);
void print_trie(FILE*, trieNode*);
sortedList* create_sortedList(void);
void sort__trie_by_occurencies(trieNode*);

#endif // TRIE_H_INCLUDED

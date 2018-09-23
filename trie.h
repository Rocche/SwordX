#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "sbolist.h"

typedef struct trieNode {
    int occurrencies;
    struct trieNode *children[CHARSET];
} trieNode;


char int_to_charset(int);
int charset_to_int(char);
char* allocate_trie_word(char *);

trieNode *create_trieNode();
void add_word(trieNode *, char *);
trieNode *add_nodes(trieNode *, char *, int);
void print_trie(FILE *, trieNode *);

void sort_trie_by_occurrencies(trieNode *, occurrencyNode **);
void add_to_sbolist(occurrencyNode **, char *, int);

#endif // TRIE_H_INCLUDED

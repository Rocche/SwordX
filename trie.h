#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "sbolist.h"

#define CHARSET 35 //   10 digits + 26 characters

static const int LOWERCASE_OFFSET = 'a' - 'A';
static const char BASE_DIGIT = '0';
static const char BASE_CHAR = 'a';
static const int CHAR_OFFSET = 10;

typedef struct trieNode{
    int occurrencies;
    struct trieNode *children[CHARSET];
} trieNode;

char int_to_charset(int);
int charset_to_int(char);
char *reallocate_trie_word(char *);
char *init_tw();

trieNode *create_trieNode();
void add_word(trieNode *, char *);
trieNode *add_nodes(trieNode *, char *, int);
void print_trie(FILE *, trieNode *);

sl_root *sort_trie_by_occurrencies(trieNode *, sl_root*);
sl_root *add_to_sbolist(sl_root*, char *, int);

#endif // TRIE_H_INCLUDED

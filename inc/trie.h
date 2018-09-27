#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "sbolist.h"

/* https://it.wikipedia.org/wiki/Caratteri_alfanumerici */

/*Charset macro: contains digits (from 0 to 9) and characters (from a to z)*/
#define CHARSET 36 //   10 digits + 26 characters

/*Constants for CHARSET convertions*/
static const int LOWERCASE_OFFSET = 'a' - 'A';
static const char BASE_DIGIT = '0';
static const char BASE_CHAR = 'a';
static const int CHAR_OFFSET = 10;

/*
Structure which defines the trie data structure.
If "occurrencies" value is 0, it means that it is a character of an input word, but is not the last character of that word.
Otherwise, the node represents an actual word, built from the very first node (except for trie_root) up to this node, converting
the c value in trieNode.children[c] to represent the actual character.
*/
typedef struct trieNode{
    int occurrencies;
    struct trieNode *children[CHARSET];
} trieNode;

/*translates int value into his charset's corresponding value*/
char int_to_charset(int);

/*Translates characters' values from the ASCII table to a CHARSET int.*/
int charset_to_int(char);

/*Initialize a word that will be contained in the trie structure*/
char *init_tw();

/*increments a word contained in the trie structure by 1*/
char *reallocate_trie_word(char *);

/*Initializes a trieNode structure*/
trieNode *create_trieNode();

/*Adds the input word to the trie*/
void add_word(trieNode *, char *);

/*Creates a new trie branch for the input word, starting from the last node already existing for that word*/
trieNode *add_nodes(trieNode *, char *, int);

/*Method that prints the trie's content into the output file in alphabetical order*/
void print_trie(FILE *, trieNode *);

/*Order tire's word by their occurency value*/
sl_root *sort_trie_by_occurrencies(trieNode *, sl_root*);

/*Adds a trie_word to the sorted list*/
sl_root *add_to_sbolist(sl_root*, char *, int);

/*Free trie's allocated memory space*/
void destroy_trie(trieNode*);

#endif // TRIE_H_INCLUDED

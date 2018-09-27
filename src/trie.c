#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sbolist.h"
#include "trie.h"
#include "memory_operations.h"

/*translates int value into his charset's corresponding value*/
char int_to_charset(int n)
{
    if (n >= 0 && n < CHAR_OFFSET)
    {
        return (n + BASE_DIGIT);
    }
    else if (n >= CHAR_OFFSET && n < CHARSET)
    {
        return (n - CHAR_OFFSET + BASE_CHAR);
    }
    else
    {
        perror("(char) int_to_charset (int): input not valid\n");
        exit(EXIT_FAILURE);
    }
}

/*
Translates characters' values from the ASCII table to a CHARSET int.
CHARSET int scale goes from 0 to CHARSET.
It converts all the characters in lower case.
*/
int charset_to_int(char c)
{
    if (isdigit(c))
    {
        return (c - BASE_DIGIT);
    }
    else if (isalpha(c))
    {
        return (int)(tolower(c)) - BASE_CHAR + CHAR_OFFSET;
    }
    else
    {
        perror("(int) charset_to_int (char): input not valid\n");
        exit(EXIT_FAILURE);
    }
}

/*initializes a trie_word*/
char *init_tw()
{
    /*Allocates 2 char spaces (one for the next character to be added, one for the terminal character)*/
    char *trie_word = calloc_object(trie_word, 2, sizeof(char));
    trie_word[1] = '\0';
    return trie_word;
}

/*increments trie_word size by 1*/
char *reallocate_trie_word(char *trie_word)
{
    /*reallocates the memory of trie_word adding 2 char spaces (one for the next character to be added and one for the terminal character)*/
    trie_word = realloc_object(trie_word, sizeof(char) * (strlen(trie_word) + 2));
    trie_word[strlen(trie_word) + 1] = '\0';
    return trie_word;
}

/*initializes trieNode structure*/
trieNode *create_trieNode()
{
    /*allocates memory space*/
    trieNode *t_node = malloc_object(t_node, sizeof(trieNode));
    /*initialize attributes*/
    t_node->occurrencies = 0;
    for (int i = 0; i < CHARSET; i++)
    {
        t_node->children[i] = NULL;
    }
    return t_node;
}



/*adds input word to the trie structure*/
void add_word(trieNode *root, char *str)
{
    /*initializes the trie_word*/
    char *trie_word = init_tw();
    int str_index = 0;
    trieNode *node = root;

    /*takes the first character of the string*/
    char c = str[str_index];
    /*loops until the end of the string*/
    while (c != '\0')
    {
        /*insert the character to the corresponding position of the trie_word*/
        trie_word[str_index] = c;

        /*Gets an integer value from c, then checks if the current node's child in that position does exist*/
        if (node->children[charset_to_int(c)] != NULL)
        {
            /*if the trie_word length has reached the input string length, we can increase the occurency value by 1 and exit the function*/
            if (strlen(str) == strlen(trie_word))
            {
                node->children[charset_to_int(c)]->occurrencies++;
                /*free the trie_word's allocated memory*/
                free(trie_word);
                return;
            }
            /*otherwise, we increase trie_word's size and recursively call the function*/
            else
            {
                trie_word = reallocate_trie_word(trie_word);
                node = node->children[charset_to_int(c)];
                str_index++;
                c = str[str_index];
            }
        }
        /*if the node doesn't have an existing child for that character, it creates a new trie branch representing the input word*/
        else
        {
            node = add_nodes(node, str, str_index);
            /*free the trie_word's allocated memory*/
            free(trie_word);
            return;
        }
    }
    /*free the trie_word's allocated memory*/
    free(trie_word);
}

/*creates a new trie branch for the input word, starting from the last node already existing for that word*/
trieNode *add_nodes(trieNode *node, char *str, int index)
{
    /*initialize a new trie node*/
    trieNode *new_node = create_trieNode(new_node);
    /*adds new_node to the parent's array of pointers*/
    node->children[charset_to_int(str[index])] = new_node;
    index++;
    /*if the word has not come to the end, it recursively call the funciton*/
    if (str[index] != '\0')
    {
        node = add_nodes(new_node, str, index);
    }
    /*otherwise, we can increase the word occurency by 1 and exit the funciton*/
    else
    {
        new_node->occurrencies++;
    }
    return node;
}

/*
Prints words contained in the trie structure in the output file,
Visting the trie structure in this way permits to print words in alphabetical order.
*/
void print_trie(FILE *file, trieNode *node)
{
    /*boolean used to check if the trie_word has been already initialized*/
    static bool init;
    static char *trie_word;

    /*if init is false, initialize the trie_word*/
    if (!init)
    {
        trie_word = init_tw(trie_word);
        init = true;
    }
    /*otherwise increase its size*/
    else
    {
        trie_word = reallocate_trie_word(trie_word);
    }

    /*takes all children nodes*/
    for (int i = 0; i < CHARSET; i++)
    {
        /*checks if children node exists and convert int value to charset*/
        if (node->children[i] != NULL)
        {
            trie_word[strlen(trie_word)] = int_to_charset(i);
            /*print the word if has a positive occurency*/
            if (node->children[i]->occurrencies > 0)
            {
                /*adds terminal character*/
                trie_word[strlen(trie_word) + 1] = '\0';
                fprintf(file, "%s: %i\n", trie_word, node->children[i]->occurrencies);
            }

            /*keep visiting the trie structure*/
            print_trie(file, node->children[i]);
        }
    }
    /*adjust triue_word before going back to the previous node of the trie structure*/
    trie_word[strlen(trie_word) - 1] = '\0';
}

/*Inserts trie's words in a sl_root structure containing words ordered by occurrency*/
sl_root *sort_trie_by_occurrencies(trieNode *t_node, sl_root *sl_root)
{
    /*boolean used to check if the trie_word has been already initialized*/
    static bool init;
    static char *trie_word;

    /*if init is false, initialize the trie_word*/
    if (!init)
    {
        trie_word = init_tw(trie_word);
        init = true;
    }
    /*otherwise increase its size*/
    else
    {
        trie_word = reallocate_trie_word(trie_word);
    }

    /*takes all children nodes*/
    for (int i = CHARSET - 1; i >= 0; i--)
    {
        /*checks if children node exists, converts int value to charset and recursively call current function with child node*/
        if (t_node->children[i] != NULL)
        {
            trie_word[strlen(trie_word)] = int_to_charset(i);
            sl_root = sort_trie_by_occurrencies(t_node->children[i], sl_root);
        }
    }

    /*if the node has at least one occurrency, adds the word to sorted list structure*/
    if (t_node->occurrencies > 0)
    {
        trie_word[strlen(trie_word) + 1] = '\0';
        sl_root = add_to_sbolist(sl_root, trie_word, t_node->occurrencies);

    }
    /*adjust triue_word before going back to the previous node of the trie structure*/
    trie_word[strlen(trie_word) - 1] = '\0';

    /*returns the sl_root structure*/
    return sl_root;
}

/*Adds a word to sl_root structure*/
sl_root *add_to_sbolist(sl_root *sl_root, char *word, int occurrencies)
{
    /*creates a temporary memory allocation for the word*/
    char* str = malloc_object(str, strlen(word)+1);
    strcpy(str,word);
    /*initialize a new sl_node structure with the current word*/
    sortedNode* sl_node = create_sortedNode(str);
    /*takes all sl_root's occurencyNodes*/
    for ( int i=0; i<sl_root->elements ; i++){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]);
        /*if an occurrency node representing the word occurrency exists, add sl_node to the linked list*/
        if (oc_node.occurrency == occurrencies){
            sl_node->next = oc_node.first;
            oc_node.first = sl_node;
            sl_root->oc_nodes[i] = oc_node;
            return sl_root;
        }
    }
    /*if an occurrency node representing the word occurrency doesn't exists, creates a pointer to a new occurrencyNode with the specified occurrency*/
    occurrencyNode* p_oc_node = create_occurrencyNode(occurrencies);
    /*Link the sl_node to the pointer*/
    p_oc_node->first = sl_node;

    /*sl_root has one more oc_node, so we increase elements counter*/
    sl_root->elements++;
    /*increase its allocation memory*/
    sl_root->oc_nodes = realloc_object(sl_root->oc_nodes, sizeof(occurrencyNode)*(sl_root->elements));
    /*the last oc_node will be the previously created pointer*/
    sl_root->oc_nodes[sl_root->elements-1] = *p_oc_node;
    return sl_root;
}

/*Free trie structure's allocated memory starting from a specified node*/
void destroy_trie(trieNode* node){
    /*free node's allocated memory*/
    for (int i = 0; i < CHARSET; i++)
    {
        if (node->children[i] != NULL)
        {
            destroy_trie(node->children[i]);
        }
    }
    /*free the input node*/
    free(node);
}
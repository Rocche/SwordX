#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sbolist.h"
#include "trie.h"

char int_to_charset(int n)
{
    char result;
    if (n >= 0 && n < CHAR_OFFSET)
    {
        result = (n + BASE_DIGIT);
    }
    else if (n >= CHAR_OFFSET && n <= CHARSET)
    {
        result = (n - CHAR_OFFSET + BASE_CHAR);
    }
    else
    {
        perror("(char) int_to_charset (int): input not valid\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

int charset_to_int(char c)
{
    int result;
    if (isdigit(c))
    {
        result = (c - BASE_DIGIT);
    }
    else if (isalpha(c))
    {
        result = (int)(tolower(c)) - BASE_CHAR + CHAR_OFFSET;
    }
    else
    {
        perror("(int) charset_to_int (char): input not valid\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

void allocate_trie_word(char *trie_word, int tw_len)
{
    if (tw_len > 0)
    {
        trie_word = realloc(trie_word, sizeof(char) * (tw_len + 2));
    }
    else
    {
        trie_word = malloc(sizeof(char) * 2);
    }
    check_heap(trie_word);
    trie_word[tw_len + 1] = '\0';
}

trieNode *create_trieNode()
{
    trieNode *t_node = malloc(sizeof(trieNode));
    check_heap(t_node);
    t_node->occurrencies = 0;
    for (int i = 0; i <= CHARSET; i++)
    {
        t_node->children[i] = NULL;
    }
    return t_node;
}

void add_word(trieNode *node, char *str)
{
    static char *trie_word;
    int tw_len = strlen(trie_word);
    char c = str[tw_len];

    allocate_trie_word(trie_word, tw_len);
    trie_word[tw_len] = c;

    if (node->children[charset_to_int(c)] != NULL)
    {
        if (strlen(str) > tw_len)
        {
            add_word(node->children[charset_to_int(c)], str);
        }
        else if (strlen(str) == tw_len)
        {
            node->children[charset_to_int(c)]->occurrencies++;
        }
    }
    else
    {
        node = add_nodes(node, str, tw_len);
    }
}

trieNode *add_nodes(trieNode *node, char *str, int index)
{
    trieNode *new_node = create_trieNode(new_node);
    node->children[charset_to_int(str[index])] = new_node;
    index++;
    if (str[index] != '\0')
    {
        node = add_nodes(new_node, str, index);
    }
    else
    {
        new_node->occurrencies++;
    }
    return node;
}

void print_trie(FILE *file, trieNode *node)
{
    static char *trie_word;
    int tw_len = strlen(trie_word);
    allocate_trie_word(trie_word, tw_len);

    for (int i = 0; i <= CHARSET; i++)
    {
        if (node->children[i] != NULL)
        {
            trie_word[tw_len] = int_to_charset(i);
            if (node->children[i]->occurrencies > 0)
            {
                trie_word[tw_len + 1] = '\0';
                fprintf(file, "%s: %i\n", trie_word, node->children[i]->occurrencies);
            }
            print_trie(file, node->children[i]);
        }
    }
    trie_word[tw_len]='\0';
}

void sort_trie_by_occurrencies(trieNode *t_node,occurrencyNode **sl_root)
{
    static char *trie_word;
    int tw_len = strlen(trie_word);
    allocate_trie_word(trie_word, tw_len);

    for (int i = CHARSET; i >= 0; i--)
    {
        if (t_node->children[i] != NULL)
        {
            trie_word[tw_len] = int_to_charset(i);
            sort_trie_by_occurrencies(t_node->children[i],sl_root);
        }
        if (t_node->occurrencies > 0)
        {
            trie_word[tw_len+1] = '\0';
            add_to_sbolist(sl_root, trie_word, t_node->occurrencies);
        }
    }
}

void add_to_sbolist(occurrencyNode **sl_root, char *word, int occurrencies)
{
    occurrencyNode *oc_node;
    for (int i = 0; i <= sizeof(sl_root); i += sizeof(occurrencyNode *))
    {
        oc_node = sl_root[i];
        if (oc_node->occurrency == occurrencies)
        {
            sortedNode *sl_node = create_sortedNode(word);
            sl_node->next = oc_node->first;
            oc_node->first = sl_node;
            return;
        }
    }
    oc_node = create_occurrencyNode(occurrencies);
    sl_root[sizeof(sl_root)-sizeof(occurrencyNode*)] = oc_node;
    sl_root = realloc(sl_root, sizeof(sl_root) + sizeof(occurrencyNode *));
    check_heap(sl_root);
}
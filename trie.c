#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sbolist.h"
#include "trie.h"

char int_to_charset(int n)
{
    if (n >= 0 && n < CHAR_OFFSET)
    {
        return (n + BASE_DIGIT);
    }
    else if (n >= CHAR_OFFSET && n <= CHARSET)
    {
        return (n - CHAR_OFFSET + BASE_CHAR);
    }
    else
    {
        perror("(char) int_to_charset (int): input not valid\n");
        exit(EXIT_FAILURE);
    }
}

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

char *reallocate_trie_word(char *trie_word)
{
    {
        trie_word = realloc(trie_word, sizeof(char) * (strlen(trie_word)+ 2));
        trie_word[strlen(trie_word) + 1] = '\0';
    }
    check_heap(trie_word);
    return trie_word;

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

char* init_tw (){
    char *trie_word = calloc(2, sizeof(char));
    check_heap(trie_word);
    trie_word[1]='\0';
    return trie_word;
}


void add_word (trieNode *root, char* str ){
    
    char *trie_word = init_tw();
    int str_index = 0;
    trieNode* node = root;

    char c = str[str_index];
    while(c != '\0'){
        trie_word[str_index]=c;
        if (node->children[charset_to_int(c)] != NULL) {
            if (strlen(str) == strlen(trie_word)) {
                node->children[charset_to_int(c)]->occurrencies++;
                free(trie_word);
                return;
            }
            else{
                trie_word = reallocate_trie_word(trie_word);
                node = node->children[charset_to_int(c)];
                str_index++;
                c = str[str_index];
            }
        } else {
            node = add_nodes(node,str,str_index);
            free(trie_word);
            return;
        }
    }
    free(trie_word);
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
    static bool init;
    static char *trie_word;

    if (!init){
        trie_word = init_tw(trie_word);
        init = true;
    } else {
        trie_word = reallocate_trie_word(trie_word);
    }
    

    for (int i = 0; i <= CHARSET; i++)
    {
        if (node->children[i] != NULL)
        {
            trie_word[strlen(trie_word)] = int_to_charset(i);
            if (node->children[i]->occurrencies > 0)
            {
                trie_word[strlen(trie_word) + 1] = '\0';
                fprintf(file, "%s: %i\n", trie_word, node->children[i]->occurrencies);
            }
            print_trie(file, node->children[i]);
        }
    }
    trie_word[strlen(trie_word) - 1] = '\0';
}

void sort_trie_by_occurrencies(trieNode *t_node, occurrencyNode **sl_root)
{
    static char *trie_word;
    trie_word = reallocate_trie_word(trie_word);

    for (int i = CHARSET; i >= 0; i--)
    {
        if (t_node->children[i] != NULL)
        {
            trie_word[strlen(trie_word)] = int_to_charset(i);
            sort_trie_by_occurrencies(t_node->children[i], sl_root);
        }
        if (t_node->occurrencies > 0)
        {
            trie_word[strlen(trie_word) + 1] = '\0';
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
    sl_root[sizeof(sl_root) - sizeof(occurrencyNode *)] = oc_node;
    sl_root = realloc(sl_root, sizeof(sl_root) + sizeof(occurrencyNode *));
    check_heap(sl_root);
}
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
        trie_word = realloc(trie_word, sizeof(char) * (strlen(trie_word) + 2));
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

char *init_tw()
{
    char *trie_word = calloc(2, sizeof(char));
    check_heap(trie_word);
    trie_word[1] = '\0';
    return trie_word;
}

void add_word(trieNode *root, char *str)
{

    char *trie_word = init_tw();
    int str_index = 0;
    trieNode *node = root;

    char c = str[str_index];
    while (c != '\0')
    {
        trie_word[str_index] = c;
        if (node->children[charset_to_int(c)] != NULL)
        {
            if (strlen(str) == strlen(trie_word))
            {
                node->children[charset_to_int(c)]->occurrencies++;
                free(trie_word);
                return;
            }
            else
            {
                trie_word = reallocate_trie_word(trie_word);
                node = node->children[charset_to_int(c)];
                str_index++;
                c = str[str_index];
            }
        }
        else
        {
            node = add_nodes(node, str, str_index);
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

    if (!init)
    {
        trie_word = init_tw(trie_word);
        init = true;
    }
    else
    {
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

sl_root *sort_trie_by_occurrencies(trieNode *t_node, sl_root *sl_root)
{
    static bool init;
    static char *trie_word;

    // reallocazione della trie_word
    if (!init)
    {
        trie_word = init_tw(trie_word);
        init = true;
    }
    else
    {
        trie_word = reallocate_trie_word(trie_word);
    }

    // ricorsione per vista trie
    for (int i = CHARSET; i >= 0; i--)
    {
        if (t_node->children[i] != NULL)
        {
            trie_word[strlen(trie_word)] = int_to_charset(i);
            sl_root = sort_trie_by_occurrencies(t_node->children[i], sl_root);
        }
    }

    // aggiunta della parola alla sorted list (se ha almeno una occorrenza)
    if (t_node->occurrencies > 0)
    {
        trie_word[strlen(trie_word) + 1] = '\0';
        sl_root = add_to_sbolist(sl_root, trie_word, t_node->occurrencies);

    }
    // cancellazione ultimo carattere della trie_word
    trie_word[strlen(trie_word) - 1] = '\0';

    return sl_root;
}

sl_root *add_to_sbolist(sl_root *sl_root, char *word, int occurrencies)
{
    char* str = malloc(strlen(word)+1);
    strcpy(str,word);
    sortedNode* sl_node = create_sortedNode(str);
    for ( int i=0; i<sl_root->elements ; i++){
        occurrencyNode oc_node = (sl_root->oc_nodes[i]);
        if (oc_node.occurrency == occurrencies){
            sl_node->next = oc_node.first;
            oc_node.first = sl_node;
            sl_root->oc_nodes[i] = oc_node;
            return sl_root;
        }
    }

    occurrencyNode* p_oc_node = create_occurrencyNode(occurrencies);
    p_oc_node->first = sl_node;

    sl_root->elements++;
    sl_root->oc_nodes = realloc (sl_root->oc_nodes, sizeof(occurrencyNode)*(sl_root->elements));
    check_heap(sl_root);
    sl_root->oc_nodes[sl_root->elements-1] = *p_oc_node;
    return sl_root;
}
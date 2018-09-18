#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

typedef struct{
    trieNode* children[CHARSET];
    int occurencies;
}trieNode;

typedef struct{
    char *occurrencies;
}sorted_list;

static trieNode *trie_root; // trie root node
static sorted_list *sl_root; // sorted list root node

static char *trie_word; // word built visiting node after node
static size_t tw_len; // trie word length

trieNode* create_trie(void);
void reallocate_tw(size_t,int);
void search_node(char*, *trieNode);
void add_nodes(char*, *trieNode);
void print_trie(FILE*, *trieNode);
sorted_list* create_sorted_list(void);
void add_to_sl(char*);

#endif // TRIE_H_INCLUDED

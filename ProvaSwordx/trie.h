#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

typedef struct{
    trieNode* children[CHARSET];
    int occurencies;
    // short last_checked;
}trieNode;

static trieNode *trie_root; // trie root node

static char *trie_word; // word built visiting node after node
static size_t tw_len; // trie word length

trieNode create_trie(void);
void check_tw_alloc(int);
void search_node(char*, *trieNode);
void add_nodes(char*, *trieNode);
void print_trie(FILE*, *trieNode);

#endif // TRIE_H_INCLUDED

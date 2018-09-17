#include <string.h>
#include <stdlib.h>
#include <error.h>

#include <trie.h>
#include <utils.h>


trieNode create_trie(){

    trie_root = malloc(sizeof(trieNode));
    if (trie_root == NULL) {
        printf("Error: malloc failed for trie_root\n");
        exit(EXIT_FAILURE);
    }

    trie_word = malloc(sizeof(char));
    check_tw_alloc(LINE);

}

void check_tw_alloc(int err_line){

    if (trie_word == NULL){
        printf("Error: allocation failed for trie_word on line %d\n", err_line);
        exit(EXIT_FAILURE);
    }
}

void search_node(char* str,trieNode* node){

    tw_len = strlen(trie_word);
    str_len = strlen(str);

    // trie visit
    for (char c= *str ; c<str_len ; c++){

        trie_word[tw_len] = c

        if (node->children[charset_to_int(c)]!=NULL){

            if (str_len < tw_len){

                perror("str_len < tw_len");
                exit(EXIT_FAILURE);

            } else if (str_len == tw_len){

                node->(children[i]->(++occurences))
                realloc(*trie_word, sizeof(char));
                check_tw_alloc(LINE);


            } else if (str_len > tw_len){

                realloc(*trie_word, tw_len+1)
                check_tw_alloc(LINE);
                get_substring(str);
                search_node(str,node->children[charset_to_int(c)]);

            }
        } else { add_nodes(str,node); }
    }
}

void add_nodes(char* str, trieNode* node){

    trieNode* new_node;
    new_node = malloc(sizeof(trieNode));
    if (new_node == NULL) {
        printf("Error: malloc failed for new_node\n");
        exit(EXIT_FAILURE);
    }
    node->children[charset_to_int(*str)] = new_node;
    get_substring(str);
    if (str!=NULL){
        add_nodes(str,node->children[charset_to_int(*str)]);
    }

}

void print_trie(FILE* file, trieNode* node){

    for (i=0; i=CHARSET;i++){
        tw_len = strlen(trie_word);
        trie_word[tw_len] == int_to_charset(i);
        if ((node->children[i]->occurencies)!=0){
            fprintf( file,"%s: %i\n",trie_word,node->children[i]->occurencies )
        }
        if (node->children[i]!=NULL){
            realloc(*trie_word,tw_len+1);
            check_tw_alloc(LINE);
            print_trie(file,node->children[i]);
        }
    }

}

void sort_trie(trieNode*){



}
















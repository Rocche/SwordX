#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "trie.h"
#include "utils.h"


trieNode* create_trie(){

    trie_root = malloc(sizeof(trieNode));
    if (trie_root == NULL) {
        printf("Error: malloc failed for trie_root\n");
        exit(EXIT_FAILURE);
    }
    reallocate_tw(sizeof(char)+1,__LINE__);

}

void reallocate_tw(size_t new_size,int err_line){

    trie_word = realloc(trie_word, new_size);
    if (trie_word == NULL){
        printf("Error: allocation failed for trie_word on line %d\n", err_line);
        exit(EXIT_FAILURE);
    }
    trie_word[new_size]='\0';
    tw_len = new_size-1;

}

void add_word(char* str,trieNode* node){

    str_len = strlen(str);

    // visit_trie
    for (char c= *str ; c<=str_len ; c++){

        trie_word[tw_len] = c;

        if (node->children[charset_to_int(c)]!=NULL){

            if (str_len < tw_len){

                perror("str_len < tw_len");
                exit(EXIT_FAILURE);

            } else if (str_len == tw_len){
                
                increase_occurrencies(node->children[charset_to_int(c)]);
                reallocate_tw(sizeof(char)+1,__LINE__);

            } else if (str_len > tw_len){

                reallocate_tw(tw_len+1,__LINE__);
                get_substring(str);
                add_word(str,node->children[charset_to_int(c)]);

            }
        } else {

            add_nodes(str,node);
            reallocate_tw(sizeof(char)+1,__LINE__);

        }
    }
}

void increase_occurrencies(trieNode* node){
    node->occurrencies++;
}

void get_occurrencies(trieNode* node){
    occurrencies=node->occurrencies;
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

    for (int i=0;i<=CHARSET;i++){
        trie_word[tw_len] == int_to_charset(i);
        if (node->children[i]!=NULL){

        }
        get_occurrencies(node->children[i]);
         if (occurrencies!=0){
            fprintf(file,"%s: %i\n",trie_word,occurrencies);
        }
        if (node->children[i]!=NULL){
            reallocate_tw(tw_len+1,__LINE__);
            print_trie(file,node->children[i]);
        }

    }
    reallocate_tw(tw_len,__LINE__);
}

sortedList* create_sorted_list(){

    sl_root = malloc(sizeof(sortedList));
    if (sl_root==NULL){
        perror("Error: malloc failed for sl_root");
        exit(EXIT_FAILURE);
    }

}

void sort_trie_by_occurrencies(trieNode* node){

    for (int i=0;i<=CHARSET;i++){
        if ((node->children[i]!=NULL)){
            get_occurrencies(node->children[i]);
            if(occurrencies>0){
                ;
            }
        }
    }
}
















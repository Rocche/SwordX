#include <string.h>
#include <stdlib.h>
#include <error.h>

#include <trie.h>
#include <utils.h>


trieNode* create_trie(){

    trie_root = malloc(sizeof(trieNode));
    if (trie_root == NULL) {
        printf("Error: malloc failed for trie_root\n");
        exit(EXIT_FAILURE);
    }
    reallocate_tw(sizeof(char)+1,LINE);

}

void reallocate_tw(size_t new_size,int err_line){

    realloc(*trie_word, new_size);
    if (trie_word == NULL){
        printf("Error: allocation failed for trie_word on line %d\n", err_line);
        exit(EXIT_FAILURE);
    }
    trie_word[new_size]='\0';
    tw_len = new_size-1;

}

void add_word(char* str,trieNode* node){

    str_len = strlen(str);

    // trie visit
    for (char c= *str ; c<=str_len ; c++){

        trie_word[tw_len] = c

        if (node->children[charset_to_int(c)]!=NULL){

            if (str_len < tw_len){

                perror("str_len < tw_len");
                exit(EXIT_FAILURE);

            } else if (str_len == tw_len){

                node->(children[i]->(++occurences))
                reallocate_tw(sizeof(char)+1,LINE);

            } else if (str_len > tw_len){

                reallocate_tw(tw_len+1,LINE);
                get_substring(str);
                add_word(str,node->children[charset_to_int(c)]);

            }
        } else {

            add_nodes(str,node);
            reallocate_tw(sizeof(char)+1,LINE);

        }
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

    for (int i=0;i<=CHARSET;i++){
        trie_word[tw_len] == int_to_charset(i);
        if ((node->children[i]->occurencies)!=0){
            fprintf( file,"%s: %i\n",trie_word,node->children[i]->occurencies )
        }
        if (node->children[i]!=NULL){
            reallocate_tw(tw_len+1,LINE);
            print_trie(file,node->children[i]);
        }

    }
    reallocate_tw(tw_len);
}

sorted_list* create_sorted_list(){

    sl_root = malloc(sizeof(sorted_list));
    if (sl_root==NULL){
        perror("Error: malloc failed for sl_root")
        exit(EXIT_FAILURE);
    }

}

void sort_trie_by_occurrencies(trieNode*){

    for (int i=0;i<=CHARSET;i++){
        if ((node->children[i]!=NULL){
            if((node->children[i]->occurrencies>0)){

            }
        }
    }
}
















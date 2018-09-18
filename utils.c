#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

//metodo che inizializza l'allocazione di memoria della stringa da far elaborare
//al trie. La dimensione è 1 carattere + carattere terminazione '\0'
char* init_current_string(){
    char* str = (char*)malloc(sizeof(char));
    if(str == NULL){
        printf("Errore nella gestione della memoria (inizializzazione stringa)");
        exit(1);
    }
    //*str = '\0';
    return str;
}

void add_char_to_str(char c, char* str){
    //printf("%lu\n", strlen(str));
    int new_length = strlen(str) + sizeof(char) * 2;
    str = (char*)realloc(str, new_length);
    if(str == NULL){
        printf("Errore nella gestione della memoria (inizializzazione stringa)");
        exit(1);
    }
    *(str + new_length - 2) = c;
    *(str + new_length - 1) = '\0';
    //printf("%c",c);
}
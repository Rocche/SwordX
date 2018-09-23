#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//inizializza l'allocazione di memoria per la stringa
char* init_current_string();
//aggiunge un carattere all'allocazione
void add_char_to_str(char, char*);

//controlla se il file è regular
bool is_regular_file(const char*);

//controlla se la stringa passate è il percorso per una directory
bool is_directory(const char*);

//controlla se la stringa è un symbolic link
bool is_symbolic_link(const char*);

#endif //FILE_OPERATIONS_H
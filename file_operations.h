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

//elabora le stringhe di un file
//usato const char* perchè il parametro deve essere argv*
void analyze_file(const char*, int, bool);

//elabora i file regolari di una directory intera
//il secondo parametro segnala se l'analisi è ricorsiva
void analyze_directory(const char*, int, int, bool);

//controlla se il file è regular
bool is_regular_file(const char*);

//controlla se la stringa passate è il percorso per una directory
bool is_directory(const char*);

//controlla se la stringa è un symbolic link
bool is_symbolic_link(const char*);

#endif //FILE_OPERATIONS_H
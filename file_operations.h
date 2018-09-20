#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_node.h"

//stringa corrente
static char* current_str;

//inizializza l'allocazione di memoria per la stringa
char* init_current_string();
//aggiunge un carattere all'allocazione
void add_char_to_str(char, char*);

//elabora le stringhe di un file
//usato const char* perchè il parametro deve essere argv*
void analyze_file(const char*);

//elabora i file regolari di una directory intera
//il secondo parametro segnala se l'analisi è ricorsiva
void analyze_directory(const char*, int);

//controlla se il file è regular
int is_regular_file(const char*);

//controlla se la stringa passate è il percorso per una directory
int is_directory(const char*);

void printFileNodes(FileNode*);

#endif //FILE_OPERATIONS_H
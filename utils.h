#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//stringa corrente
static char* current_str;

//inizializza l'allocazione di memoria per la stringa
char* init_current_string();
//aggiunge un carattere all'allocazione
void add_char_to_str(char, char*);

//elabora le stringhe di un file
void analyze_file(const char*);
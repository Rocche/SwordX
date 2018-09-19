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
//usato const char* perchè il parametro deve essere argv*
void analyze_file(const char*);

//elabora i file regolari di una directory intera
void analyze_directory(const char*);

//controlla se il file è regular
int is_regular_file(const char*);

//controlla se la stringa passate è il percorso per una directory
int is_directory(const char*);
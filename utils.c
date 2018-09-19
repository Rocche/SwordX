#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include "utils.h"

//metodo che inizializza l'allocazione di memoria della stringa da far elaborare
//al trie. La dimensione è 1 carattere + carattere terminazione '\0'
char *init_current_string()
{
    char *str = (char *)malloc(sizeof(char));
    if (str == NULL)
    {
        printf("Errore nella gestione della memoria (inizializzazione stringa)");
        exit(1);
    }
    return str;
}

//metodo che aggiunge un carattere alla stringa di riferimento
void add_char_to_str(char c, char *str)
{
    //printf("%lu\n", strlen(str));
    int new_length = strlen(str) + sizeof(char) * 2;
    str = (char *)realloc(str, new_length);
    if (str == NULL)
    {
        printf("Errore nella gestione della memoria (inizializzazione stringa)");
        exit(1);
    }
    *(str + new_length - 2) = c;
    *(str + new_length - 1) = '\0';
}

//metodo che elabora le stringhe di un file
void analyze_file(const char *path)
{
    printf("Si è scelto di contare le occorrenze di %s\n\n", (path));
    FILE *fp = fopen(path, "r");
    //se non è stato trovato il file
    if (fp == NULL)
    {
        perror("Could not open the file");
        //exit(EXIT_FAILURE);
    }
    else
    {
        char c;
        char *current_str;
        int isFirst = 1;
        while ((c = fgetc(fp)) != EOF)
        {
            if (isalpha(c) || isdigit(c))
            {
                if (isFirst)
                {
                    current_str = init_current_string();
                    isFirst = 0;
                }
                add_char_to_str(c, current_str);
            }
            else
            {
                //prima controllo che esista effettivamente il current_string
                if (current_str != NULL)
                {
                    printf("%s\n", current_str); //manda al trie
                    isFirst = 1;
                    free(current_str);
                    current_str = NULL;
                }
            }
        }
        fclose(fp);
    }
    printf("\n\n");
}

//metodo che elabora i file regolari in una directory
void analyze_directory(const char *path)
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir(path);
    if (dp != NULL)
    {
        while (ep = readdir(dp)){
            puts(ep->d_name);
        }
        closedir(dp);
    }
    else{
        perror("Couldn't open the directory");
    }
}
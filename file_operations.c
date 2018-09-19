#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#include "file_operations.h"

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

//metodo che controlla se il file specificato è regular
int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

//metodo che controlla se la stringa passata è il percorso per una directory
int is_directory(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
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
        exit(EXIT_FAILURE);
    }
    else
    {
        char c;
        //inizializzo a NULL per evitare errori nella gestione della 
        //memoria in caso di file che iniziano con valori non corretti
        char *current_str = NULL;
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
    //struttura contiene d_name, ovvero il nome del file in questione
    struct dirent *ep;

    dp = opendir(path);
    if (dp != NULL)
    {
        while (ep = readdir(dp)){
            char *file_path = malloc(strlen(path) + strlen(ep -> d_name) + 1);
            strcpy(file_path, path);
            strcat(file_path, ep -> d_name);
            if(is_regular_file(file_path)){
                analyze_file(file_path);
            }
            free(file_path);
        }
        closedir(dp);
    }
    else{
        perror("Couldn't open the directory");
    }
}
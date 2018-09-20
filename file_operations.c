#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#include "file_operations.h"
#include "file_node.h"

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
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

//metodo che controlla se la stringa passata è il percorso per una directory
int is_directory(const char *path)
{
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
                    //printf("a\n");
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
//ritorna il primo FileNode
void analyze_directory(const char *path, int recursive/*, FileNode* first*/)
{
    //puntatore al filenode corrente (inizializzato al first)
    //FileNode* current_file_node = first;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            char *filename = ent->d_name;
            if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0)
            {
                size_t path_size = (strlen(path) + strlen(filename) + 1);
                char *new_path = (char *)malloc(path_size * sizeof(char));
                //check_heap(path);
                strcpy(new_path, path);
                //se non c'è il carattere '/', realloco e aggiungo
                if(*(new_path + strlen(new_path) - 1) != '/'){
                    new_path = (char *)realloc(new_path, (path_size + 1)*sizeof(char));
                    strcat(new_path, "/");
                }
                strcat(new_path, filename);
                //aggiungo carattere '/0'
                *(new_path + strlen(new_path)) = '\0';
                if (is_regular_file(new_path)){
                    //printf("%s\n", new_path);
                    //FileNode* new_file_node = add_FileNode(new_path, current_file_node);
                    //current_file_node = new_file_node;
                    analyze_file(new_path);
                }
                if (is_directory(new_path) && recursive){
                    analyze_directory(new_path, 1/*, current_file_node*/);
                }
                free(new_path);
            }
        }
    }
}

void printFileNodes(FileNode *first)
{
    FileNode *current_file_node = first;
    printf("FILE PRESI:\n");
    while (current_file_node != NULL)
    {
        printf("%s\n", current_file_node->file_path);
        current_file_node = current_file_node->next;
    }
    printf("Finito\n");
}
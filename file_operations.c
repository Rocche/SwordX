#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "file_operations.h"
#include "heap.h"

//metodo che inizializza l'allocazione di memoria della stringa da far elaborare
//al trie. La dimensione è 1 carattere + carattere terminazione '\0'
char *init_current_string()
{
    char *str = (char *)malloc(sizeof(char));
    check_heap(str);
    return str;
}

//metodo che aggiunge un carattere alla stringa di riferimento
void add_char_to_str(char c, char *str)
{
    //printf("%lu\n", strlen(str));
    int new_length = strlen(str) + sizeof(char) * 2;
    str = (char *)realloc(str, new_length);
    check_heap(str);
    *(str + new_length - 2) = c;
    *(str + new_length - 1) = '\0';
}

//metodo che controlla se il file specificato è regular
bool is_regular_file(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISREG(buf.st_mode) ? true : false;
}

//metodo che controlla se la stringa passata è il percorso per una directory
bool is_directory(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISDIR(buf.st_mode) ? true : false;
}

//controlla se la stringa passata è un symbolic link
bool is_symbolic_link(const char *path)
{
    struct stat buf;
    if (lstat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISLNK(buf.st_mode) ? true : false;
}

//metodo che elabora le stringhe di un file
void analyze_file(const char *path, int min, bool alpha)
{
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        perror("Could not open file");
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fptr) != -1)
    {
        char *word;
        char *save;

        if (*(line + strlen(line) - 1) == '\n')
            *(line + strlen(line) - 1) = '\0'; // Strips \n from line

        word = strtok_r(line, " ", &save);

        while (word != NULL)
        {
            bool is_valid = true;

            if (strlen(word) >= min)
            {
                if (alpha)
                {
                    for (int i = 0; i < strlen(word); i++)
                    {
                        if (!isalpha(*(word + i)))
                        {
                            is_valid = false;
                            break;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < strlen(word); i++)
                    {
                        if (!isalpha(*(word + i)) && !isdigit(*(word + i)))
                        {
                            is_valid = false;
                            break;
                        }
                    }
                }
            }
            else{
                is_valid = false;
            }

            if(is_valid){
                printf("%s\n", word);
            }

            word = strtok_r(NULL, " ", &save);
        }
    }
    fclose(fptr);
    printf("\n\n");
}

//metodo che elabora i file regolari in una directory
//ritorna il primo FileNode
void analyze_directory(const char *path, int recursive, int min, bool alpha)
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
                check_heap(new_path);
                strcpy(new_path, path);
                //se non c'è il carattere '/', realloco e aggiungo
                if (*(new_path + strlen(new_path) - 1) != '/')
                {
                    new_path = (char *)realloc(new_path, (path_size + 1) * sizeof(char));
                    check_heap(new_path);
                    strcat(new_path, "/");
                }
                strcat(new_path, filename);
                //aggiungo carattere '/0'
                *(new_path + strlen(new_path)) = '\0';
                if (is_regular_file(new_path))
                {
                    analyze_file(new_path, min, alpha);
                }
                if (is_directory(new_path) && recursive)
                {
                    analyze_directory(new_path, 1, min, alpha);
                }
                free(new_path);
            }
        }
    }
}
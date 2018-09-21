#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "file_operations.h"
#include "utils.h"

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

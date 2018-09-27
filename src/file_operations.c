#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "file_operations.h"
#include "memory_operations.h"

/*check if string is a regular file's path*/
bool is_regular_file(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISREG(buf.st_mode) ? true : false;
}

/*check if string is a directory's path*/
bool is_directory(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISDIR(buf.st_mode) ? true : false;
}

/*check if string is a symbolic link's path*/
bool is_symbolic_link(const char *path)
{
    struct stat buf;
    if (lstat(path, &buf) != 0)
    {
        return false;
    }
    return S_ISLNK(buf.st_mode) ? true : false;
}

/*check if string is composed by alphanumerical characters only*/
bool is_alphanumerical_string(char* word){
    /*checks every character of the string*/
    for (int i = 0; i < strlen(word); i++)
    {
        if (!isalpha(*(word + i)) && !isdigit(*(word + i)))
        {
            return false;
        }
    }
    return true;
}

/*check if string is composed by alphabetical characters only*/
bool is_alphabetical_string(char* word){
    /*checks every character of the string*/
    for (int i = 0; i < strlen(word); i++)
    {
        if (!isalpha(*(word + i)))
        {
            return false;
        }
    }
    return true;
}


/*check if a string is in a blacklist*/
bool is_in_blacklist(char* word, char** blacklist, size_t blacklist_size){
    /*checks every string of the list*/
    for(int i = 0; i < blacklist_size; i++){
        /*compares the input word to the current blacklist's string*/
        if(strcmp(word, *(blacklist + i)) == 0){
            return true;
        }
    }
    return false;
}
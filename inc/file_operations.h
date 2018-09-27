#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*check if string is a regular file's path*/
bool is_regular_file(const char*);

/*check if string is a directory's path*/
bool is_directory(const char*);

/*check if string is a symbolic link's path*/
bool is_symbolic_link(const char*);

/*check if string is composed by alphanumerical characters only*/
bool is_alphanumerical_string(char*);

/*check if string is composed by alphabetical characters only*/
bool is_alphabetical_string(char*);

/*check if a string is in a blacklist*/
bool is_in_blacklist(char*, char**, size_t);

#endif //FILE_OPERATIONS_H
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "stddef.h"

#define CHARSET 35 //   10 digits + 26 characters

static const int LOWERCASE_OFFSET = 'a' - 'A';

static const char BASE_DIGIT = '0';
static const char BASE_CHAR = 'a';
static const int CHAR_OFFSET = 10;

//  functions
char* allocate_str(char*,int);
char* reallocate_str(char*,size_t,int);
char* clear_str(char*);
char int_to_charset(int);
int charset_to_int(char);

#endif // UTILS_H_INCLUDED

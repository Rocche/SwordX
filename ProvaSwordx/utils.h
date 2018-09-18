#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "stddef.h"

#define CHARSET 35 //   10 digits + 26 characters

static const int LOWERCASE_OFFSET = 'a' - 'A';

static const char BASE_DIGIT = '0';
static const char BASE_CHAR = 'a';
static const int CHAR_OFFSET = 10;

static size_t str_len; // input string length

char int_to_charset(int);
int charset_to_int(char);
char* get_substring(char*);

#endif // UTILS_H_INCLUDED

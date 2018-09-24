#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ctype.h>

/*https://it.wikipedia.org/wiki/Caratteri_alfanumerici*/
#define CHARSET 35 //   10 digits + 26 characters

static const int LOWERCASE_OFFSET = 'a' - 'A';
static const char BASE_DIGIT = '0';
static const char BASE_CHAR = 'a';
static const int CHAR_OFFSET = 10;

void check_heap(void*);
void print_logo();
#endif // UTILS_H_INCLUDED

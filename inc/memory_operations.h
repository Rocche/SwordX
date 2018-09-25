#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>

void check_heap(void*);
void* malloc_object(void*, size_t);
void* realloc_object(void*, size_t);
void* calloc_object(void*, int, size_t);

#endif // UTILS_H_INCLUDED

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ctype.h>
#include <stdlib.h>

/*Check if a pointer returned NULL after a memory allocation*/
void check_heap(void*);

/*Malloc of an object, then check_heap*/
void* malloc_object(void*, size_t);

/*Realloc of an object, then check_heap*/
void* realloc_object(void*, size_t);

/*Calloc of an object, then check_heap*/
void* calloc_object(void*, int, size_t);

#endif // UTILS_H_INCLUDED

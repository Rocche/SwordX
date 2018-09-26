#include "memory_operations.h"
#include <stdlib.h>
#include <stdio.h>


void check_heap(void *object)
{
    if (object == NULL)
    {
        perror("Errore nell'allocazione della memoria.");
        exit(EXIT_FAILURE);
    }
}

void* malloc_object(void* object, size_t size){
    object = malloc(size);
    check_heap(object);
    return object;
}

void* realloc_object(void* object, size_t size){
    object = realloc(object, size);
    check_heap(object);
    return object;
}

void* calloc_object(void* object, int elements, size_t size){
    object = calloc(elements, size);
    check_heap(object);
    return object;
}
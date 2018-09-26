#include "memory_operations.h"
#include <stdlib.h>
#include <stdio.h>

/*Checks if a pointer created by an allocation in memory is NULL*/
void check_heap(void *object)
{
    if (object == NULL)
    {
        perror("Errore nell'allocazione della memoria.");
        exit(EXIT_FAILURE);
    }
}

/*Malloc and check*/
void* malloc_object(void* object, size_t size){
    object = malloc(size);
    check_heap(object);
    return object;
}

/*Realloc and check*/
void* realloc_object(void* object, size_t size){
    object = realloc(object, size);
    check_heap(object);
    return object;
}

/*Calloc and check*/
void* calloc_object(void* object, int elements, size_t size){
    object = calloc(elements, size);
    check_heap(object);
    return object;
}
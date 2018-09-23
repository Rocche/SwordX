#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

void check_heap(void* object){
    if(object == NULL){
        perror("Errore nell'allocazione della memoria.");
        exit(EXIT_FAILURE);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "utils.h"

char* allocate_str (char* str, int err_line) {

    str = malloc(sizeof(char)+1);
    if (str == NULL){
        printf("Error: allocation failed for str on line %d\n", err_line);
        exit(EXIT_FAILURE);
    }
    return str;
}

char* reallocate_str (char* str, size_t size, int err_line) {

    str = realloc(str, size);
    if (str == NULL) {
        printf("Error: re-allocation failed for str on line %d\n", err_line);
        exit(EXIT_FAILURE);
    }
    return str;
}

char* clear_str(char* str){
        
    int str_len = strlen(str);
    for (int i=0; i<=str_len ; i++){
       *(str+i)='\0';
    }
   return str;
}

char int_to_charset(int n) {

    char result;
    if (n>=0 && n<CHAR_OFFSET){result=(n+BASE_DIGIT);}
    else if (n>=CHAR_OFFSET && n<=CHARSET){ result=(n-CHAR_OFFSET+BASE_CHAR);}
    else { perror("int_to_charset() input not valid\n"); exit(EXIT_FAILURE);}
    return result;

}

int charset_to_int(char c) {

    int result;
    if (isdigit(c)){result=(c-BASE_DIGIT);}
    else if (isalpha(c)) {result= (int)(tolower(c))-BASE_CHAR+CHAR_OFFSET;}
    else { perror("charset_to_int() input not valid\n"); exit(EXIT_FAILURE);}
    return result;

}


#include <utils.h>
#include <err.h>
#include <stdio.h>


char int_to_charset(int n){

    if (n>=0 && n<CHAR_OFFSET){return n;}
    else if (n>CHAR_OFFSET && n<=CHARSET){ return (n-CHAR_OFFSET+BASE_CHAR);}
    else { perror("int_to_charset() input not valid"); exit(EXIT_FAILURE)}

}

int charset_to_int(char c){

    if (isdigit(c)){return c}
    else if (isalpha(c)) {(return (tolower(c)-BASE_CHAR+CHAR_OFFSET))}
    else { perror("charset_to_int() input not valid"); exit(EXIT_FAILURE)}

}

char* get_substring(char* string){

    substring = malloc[sizeof((char)*(str_len-1)];
    for (j=0 ; j=str_len-1 ; j++){
        substring[j]=string[j+1]
    }
    free(*string);
    string = substring;
    return string;

}

#include <utils.h>
#include <err.h>
#include <stdlib.h>


char int_to_charset(int n){

    char result;
    if (n>=0 && n<CHAR_OFFSET){result=char(n);}
    else if (n>CHAR_OFFSET && n<=CHARSET){ result=char(n-CHAR_OFFSET+BASE_CHAR);}
    else { perror("int_to_charset() input not valid"); exit(EXIT_FAILURE)}
    return result;

}

int charset_to_int(char c;){

    int result;
    if (isdigit(c)){result=int(c);}
    else if (isalpha(c)) {result= int(tolower(c)-BASE_CHAR+CHAR_OFFSET)}
    else { perror("charset_to_int() input not valid"); exit(EXIT_FAILURE)}
    return result;

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

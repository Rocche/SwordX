#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char *argv[]){


    FILE  *source_fp, *dest_fp;
    int ch;

    if (argc != 2){
        fprintf(stderr, "usage: ProvaSwordx <source> <dest> \n");
        exit(EXIT_FAILURE);
    }

    if ((source_fp = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Can't open %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    dest_fp = fopen("/home/ale/Desktop/Files/swordx.out", "w");

    if (dest_fp == NULL){
        fprintf(stderr, "Can't open output file");
        fclose(source_fp);
        exit(EXIT_FAILURE);
    }

    while ((ch = getc(source_fp)) != EOF)
        while (isalpha(ch))
            ch = tolower(ch);

    fclose(source_fp);
    fclose(dest_fp);
    return 0;

}

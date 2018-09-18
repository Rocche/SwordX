#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parameters.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    FILE *fp;
    //controllo argomenti da linea di comando
    while (*(++argv))
    {
        //se inizia con un '-', l'argomento è una option
        if ((*argv)[0] == '-')
        {
            switch ((*argv)[1])
            {
            default:
                printf("Unknown option -%c\n", (*argv)[1]);
                break;
            case 'h':
                help();
                break;
            case 'v':
                printf("--verbose\n");
                break;
            }
        }
        //altrimenti è un parametro di input
        else
        {
            printf("Si è scelto di contare le occorrenze di %s\n\n", (*argv));
            fp = fopen(*argv, "r");
            //se non è stato trovato il file
            if (fp == NULL)
            {
                printf("File non trovato.\n");
                exit(1);
            }
            else
            {
                char c;
                char * current_str;
                int isFirst = 1;
                while((c = fgetc(fp)) != EOF){
                    if(isalpha(c) || isdigit(c)){
                        if(isFirst){
                            current_str = init_current_string();
                            isFirst = 0;
                        }
                        add_char_to_str(c, current_str);
                        //printf("%s", current_str);
                    }
                    else{
                        //prima controllo che esista effettivamente il current_string
                        if(current_str != NULL){
                            printf("%s\n", current_str);    //manda al trie
                            isFirst = 1;
                            free(current_str);
                            current_str = NULL;
                            //printf("%p", current_str);
                        }
                    }
                }
            }
            //fine
            printf("\n\n");
            fclose(fp);
        }
    }
    return 0;
}

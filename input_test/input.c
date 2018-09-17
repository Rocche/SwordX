#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE *fp;
    //controllo argomenti da linea di comando
    while (*(++argv))
    {
        //se inizia con un '-', l'argomento è una option
        if ( (*argv)[0] == '-' )
        {
            switch ((*argv)[1])  
            {
                default:
                    printf("Unknown option -%c\n", (*argv)[1]);
                    break;
                case 'h':
                    printf("--help\n");
                    break;
                case 'v':
                    printf("--verbose\n");
                    break;
            }
        }
        //altrimenti è un parametro di input
        else{
            printf("Si è scelto di contare le occorrenze di %s\n\n", (*argv));
            fp = fopen(*argv, "r");
            //se non è stato trovato il file
            if(fp == NULL){
                printf("File non trovato.\n");
                exit(1);
            }
            else{
                char c = fgetc(fp);
                while(c != EOF){
                    printf("%c", c);
                    c = fgetc(fp);
                }
            }
            printf("\n");
            fclose(fp);
        }
    }
    return 0;
}

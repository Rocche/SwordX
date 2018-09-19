#include <stdio.h>
#include <stdlib.h>

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
                show_help();
                exit(0);
            case 'v':
                printf("--verbose\n");
                break;
            }
        }
        //altrimenti è un parametro di input
        else
        {
            analyze_file(*argv);
        }
    }
    return 0;
}

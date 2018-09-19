#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"
#include "file_operations.h"

int main(int argc, char const *argv[])
{
    //inizializzazione opzioni
    int recursive = 0;
    int follow = 0;
    int exclude = 0;
    int alpha = 0;
    int min = 0;
    int ignore = 0;
    int sort_by_occurency = 0;
    int log = 0;

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
                exit(EXIT_SUCCESS);
            case 'r':
                recursive = 1;
                break;
            case 'e':
                exclude = 1;
                break;
            case 'a':
                alpha = 1;
                break;
            case 'm':
                min = 1;
                break;
            case 'i':
                ignore = 1;
                break;
            case 's':
                sort_by_occurency = 1;
                break;
            case 'l':
                log = 1;
                break;
            }
        }
        //altrimenti è un parametro di input
        else
        {
            if (is_regular_file(*argv))
            {
                analyze_file(*argv);
            }
            else if (is_directory(*argv))
            {
                if (recursive)
                {
                    analyze_directory(*argv, 1);
                }
                else
                {
                    analyze_directory(*argv, 0);
                }
            }
            else
            {
                perror("Argomento non riconosciuto");
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}

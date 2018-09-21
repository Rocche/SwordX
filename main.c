#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*argp guide: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
#include <argp.h>
#include <argz.h>
#include "file_operations.h"

//inizializzazione opzioni
bool recursive = false;
bool follow = false;
bool exclude = false;
bool alpha = false;
int min = 0;
bool ignore = false;
bool sort_by_occurency = false;
bool log_flag = false;

/*inizializza automaticamente l'opzione --version*/
const char *argp_program_version = "version 1.0";
/*struct per i parametri*/
struct arguments
{
    char *argz;
    size_t argz_len;
};
/*Azioni in base al tipo di opzione*/
static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *a = state->input;
    switch (key)
    {
    case 'r':
        recursive = true;
        break;
    case 'e':
        exclude = true;
        break;
    case 'f':
        follow = true;
        break;
    case 'a':
        alpha = true;
        break;
    case 'm':
        min = atoi(arg);
        break;
    case 'i':
        ignore = true;
        break;
    case 's':
        sort_by_occurency = true;
        break;
    case 'l':
        log_flag = true;
        break;
    case ARGP_KEY_ARG:
        argz_add(&a->argz, &a->argz_len, arg);
        break;
    case ARGP_KEY_INIT:
        a->argz = 0;
        a->argz_len = 0;
        break;
    case ARGP_KEY_END:
        if (argz_count(a->argz, a->argz_len) <= 0)
        {
            argp_failure(state, 1, 0, "Missing file to process, please check --help");
        }
        break;
    }
    return 0;
}

int main(int argc, char **argv)
{
    /*contiene la descrizione delle opzioni*/
    struct argp_option options[] = {
        {"recursive", 'r', 0, 0, "Check for subdirectories too"},
        {"exclude", 'e', "<file>", 0, "The specified file won't be considered"},
        {"follow", 'f', 0, 0, "Follow links too"},
        {"alpha", 'a', 0, 0, "Consider only words composed by alphabetical characters"},
        {"min", 'm', "<num>", 0, "Consider only words with at least <num> characters"},
        {"ignore", 'i', "<file>", 0, "Ignore words contained in <file> (a row is considered as a word)"},
        {"sortbyoccurrency", 's', 0, 0, "The analysis sorts words by occurency"},
        {"log", 'l', "<file>", 0, "At the end create a log file containing stats foreach file processed"},
        {0}};

    struct arguments arguments;
    //contiene le opzioni, callback function e descrittore in usage
    struct argp argp = {options, parse_opt, "<input1> <input2> ... <inputn>", "Count words occurencies in specified files or directories and save the reuslt in a .txt file"};

    if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0)
    {
        /*salva l'argomento precedente*/
        const char *previous = NULL;
        char *argument;
        /*controlla gli argomenti uno per uno*/
        while ((argument = argz_next(arguments.argz, arguments.argz_len, previous)))
        {
            previous = argument;

            /*si controlla il tipo del file*/
            if(is_symbolic_link(argument)){
                if(follow){
                    analyze_file(argument, min, alpha);
                }
                else{
                    printf("Saltato link %s: per abilitare i link, aggiungere opzione -f o --follow", argument);
                }
            }
            else if(is_regular_file(argument)){
                analyze_file(argument, min, alpha);
            }
            else if(is_directory(argument)){
                if(recursive){
                    analyze_directory(argument, 1, min, alpha);
                }
                else{
                    analyze_directory(argument, 0, min, alpha);
                }
            }
        }
        printf("\n");
        free(arguments.argz);
    }
    return 0;
}

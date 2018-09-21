#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "trie.h"
#include "sbolist.h"
#include <stdbool.h>
/*argp guide: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
#include <argp.h>
#include <argz.h>
#include <dirent.h>
#include "file_operations.h"
#include "heap.h"

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


//metodo che elabora le stringhe di un file
void analyze_file(const char *path)
{
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        perror("Could not open file");
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fptr) != -1)
    {
        char *word;
        char *save;

        if (*(line + strlen(line) - 1) == '\n')
            *(line + strlen(line) - 1) = '\0'; // Strips \n from line

        word = strtok_r(line, " ", &save);

        while (word != NULL)
        {
            bool is_valid = true;

            if (strlen(word) >= min)
            {
                if (alpha)
                {
                    for (int i = 0; i < strlen(word); i++)
                    {
                        if (!isalpha(*(word + i)))
                        {
                            is_valid = false;
                            break;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < strlen(word); i++)
                    {
                        if (!isalpha(*(word + i)) && !isdigit(*(word + i)))
                        {
                            is_valid = false;
                            break;
                        }
                    }
                }
            }
            else{
                is_valid = false;
            }

            if(is_valid){
                printf("%s\n", word);
            }

            word = strtok_r(NULL, " ", &save);
        }
    }
    fclose(fptr);
    printf("\n\n");
}


//metodo che elabora i file regolari in una directory
void analyze_directory(const char *path)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            char *filename = ent->d_name;
            if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0)
            {
                size_t path_size = (strlen(path) + strlen(filename) + 1);
                char *new_path = (char *)malloc(path_size * sizeof(char));
                check_heap(new_path);
                strcpy(new_path, path);
                //se non c'è il carattere '/', realloco e aggiungo
                if (*(new_path + strlen(new_path) - 1) != '/')
                {
                    new_path = (char *)realloc(new_path, (path_size + 1) * sizeof(char));
                    check_heap(new_path);
                    strcat(new_path, "/");
                }
                strcat(new_path, filename);
                //aggiungo carattere '/0'
                *(new_path + strlen(new_path)) = '\0';
                if (is_regular_file(new_path))
                {
                    analyze_file(new_path);
                }
                if (is_directory(new_path) && recursive)
                {
                    analyze_directory(new_path);
                }
                free(new_path);
            }
        }
    }
}


int main(void){

    trieNode* trie_root = create_trieNode();
    bool sorted = false;
    FILE* dest_fp;

    if (sorted==false) {
        print_trie( dest_fp, trie_root); 
    } else {
        occurrencyNode** sl_root; // sorted list occurrencyNodes pointer
        sl_root = sort_trie_by_occurrencies(trie_root);
        qsort (sl_root, sizeof(sl_root)/sizeof(occurrencyNode*),sizeof(occurrencyNode*),compare_occurrencyNodes);
        print_sorted_list(dest_fp, sl_root);
    }

    fclose(dest_fp);
    
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
                    analyze_file(argument);
                }
                else{
                    printf("Saltato link %s: per abilitare i link, aggiungere opzione -f o --follow", argument);
                }
            }
            else if(is_regular_file(argument)){
                analyze_file(argument);
            }
            else if(is_directory(argument)){
                if(recursive){
                    analyze_directory(argument);
                }
                else{
                    analyze_directory(argument);
                }
            }
        }
        printf("\n");
        free(arguments.argz);
    }
    return 0;
}

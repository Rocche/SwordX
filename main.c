#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
/*argp guide: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
#include <argp.h>
#include <argz.h>
#include <dirent.h>

#include "file_operations.h"
#include "utils.h"

#include "trie.h"
#include "sbolist.h"

//inizializzazione opzioni
bool recursive = false;
bool follow = false;
bool exclude = false;
bool alpha = false;
int min = 0;
bool sort_by_occurency = false;
bool log_flag = false;

/*word blacklist (--ignore)*/
char** word_blacklist = NULL;
size_t word_blacklist_size = 0;

/*file blacklist (--exclude)*/
char** file_blacklist = NULL;
size_t file_blacklist_size = 0;

/*root trie*/
trieNode* trie_root = NULL;
/*output*/
FILE* dest_fp = NULL;

/*inizializza automaticamente l'opzione --version*/
const char *argp_program_version = "version 1.0";
/*struct per i parametri*/
struct arguments
{
    char *argz;
    size_t argz_len;
};

/*crea la blacklist di parole da ignorare contenute in un file*/

char** update_word_blacklist(const char* path){

    FILE* fp = fopen(path, "r");
    if(fp == NULL){
        perror("Could not open the ignore file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1)
    {
        if (*(line + strlen(line) - 1) == '\n'){
            *(line + strlen(line) - 1) = '\0';
        }

        word_blacklist = (char**)realloc(word_blacklist, (word_blacklist_size + 1)*sizeof(char*));
        check_heap(word_blacklist);
        *(word_blacklist + word_blacklist_size) = (char*)malloc(strlen(line));
        check_heap(*(word_blacklist + word_blacklist_size));
        strcpy(*(word_blacklist + word_blacklist_size), line);
        word_blacklist_size++;
    }
    fclose(fp);
}

char** update_file_blacklist(const char* file){
    file_blacklist = (char**)realloc(file_blacklist, (file_blacklist_size + 1)*sizeof(char*));
    check_heap(file_blacklist);
    *(file_blacklist + file_blacklist_size) = (char*)malloc(strlen(file));
    check_heap(*(file_blacklist + file_blacklist_size));
    strcpy(*(file_blacklist + file_blacklist_size), file);
    file_blacklist_size++;
}

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
        update_file_blacklist(arg);
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
        update_word_blacklist(arg);
        break;
    case 's':
        sort_by_occurency = true;
        break;
    case 'o':
        //dest_fp = arg;
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
void analyze_file(const char *path, trieNode* trie_node)
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
                    if(!is_alphabetical_string(word) || is_in_blacklist(word, word_blacklist, word_blacklist_size)){
                        is_valid = false;
                    }
                }
                else
                {
                    if(!is_alphanumerical_string(word) || is_in_blacklist(word, word_blacklist, word_blacklist_size)){
                        is_valid = false;
                    }
                }
            }
            else{
                is_valid = false;
            }

            if(is_valid){
                //printf("%s\n", word);
                add_word(trie_root, word);
            }

            word = strtok_r(NULL, " ", &save);
        }
    }
    fclose(fptr);
    printf("\n\n");
}


//metodo che elabora i file regolari in una directory
void analyze_directory(const char *path, trieNode* trie_node)
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
                    /*controlla che il file non sia nella blacklist*/
                    if(!is_in_blacklist(new_path, file_blacklist, file_blacklist_size)){
                        analyze_file(new_path, trie_node);
                    }
                }
                if (is_directory(new_path) && recursive)
                {
                    analyze_directory(new_path, trie_node);
                }
                free(new_path);
            }
        }
    }
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
        {"output", 'o', "<file>", 0, "Specify the output file (if not set, it will be automatically \"output.txt\""},
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


        trie_root = create_trieNode();

        //printf("ciao\n");
        /*controlla gli argomenti uno per uno*/
        while ((argument = argz_next(arguments.argz, arguments.argz_len, previous)))
        {
            previous = argument;

            /*si controlla il tipo del file*/
            if(is_symbolic_link(argument)){
                if(follow){
                    analyze_file(argument, trie_root);
                }
                else{
                    printf("Saltato link %s: per abilitare i link, aggiungere opzione -f o --follow", argument);
                }
            }
            else if(is_regular_file(argument)){
                analyze_file(argument, trie_root);
            }
            else if(is_directory(argument)){
                analyze_directory(argument, trie_root);
            }
        }
        printf("\n");
        free(arguments.argz);
    }

    dest_fp = fopen("output.txt", "w");
    if(dest_fp == NULL){
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }
    if (!sort_by_occurency)
    {
        print_trie(dest_fp, trie_root);
    }
    else
    {
        occurrencyNode **sl_root = malloc(sizeof(occurrencyNode *));
        check_heap(sl_root);
        sort_trie_by_occurrencies(trie_root,sl_root);
        qsort(sl_root, sizeof(sl_root) / sizeof(occurrencyNode *), sizeof(occurrencyNode *), compare_occurrencyNodes);
        print_sorted_list(dest_fp, sl_root);
    }
    fclose(dest_fp);
    return 0;
}

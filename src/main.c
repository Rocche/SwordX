#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*argp guide: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
#include <argp.h>
#include <argz.h>
#include <dirent.h>
#include <time.h>
#include <glob.h>
#include <fnmatch.h>

#include "file_operations.h"
#include "memory_operations.h"
#include "trie.h"
#include "sbolist.h"

/*Options initialization*/
bool recursive = false;
bool follow = false;
bool exclude = false;
bool alpha = false;
int min = 0;
bool sort_by_occurency = false;
bool log_flag = false;

/*word blacklist (--ignore)*/
char **word_blacklist = NULL;
size_t word_blacklist_size = 0;

/*file blacklist (--exclude)*/
char **file_blacklist = NULL;
size_t file_blacklist_size = 0;

/*trie*/
trieNode *trie_root = NULL;

/*output (default swordx.out)*/
FILE *dest_fp = NULL;
char *output_path = "swordx.out";

/*log file*/
FILE *log_fp = NULL;

/*automatically initialize --version option*/
const char *argp_program_version = "version 1.0";

/*arguments structure (argz.h)*/
struct arguments
{
    char *argz;
    size_t argz_len;
};

/*wildcard*/
char** wildcards_list = NULL;
size_t wildcards_list_size = 0;

/*word blacklist --ignore*/
char **update_word_blacklist(const char *path)
{

    /*apertura del file*/
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Can not open ignored file %s", path);
        exit(EXIT_FAILURE);
    }

    /*inizializzazione file's lines*/
    char *line = NULL;
    size_t len = 0;

    /*check line by line*/
    while (getline(&line, &len, fp) != -1)
    {
        /*change \n to \0*/
        if (*(line + strlen(line) - 1) == '\n')
        {
            *(line + strlen(line) - 1) = '\0';
        }

        /*reallocates blacklist size*/
        word_blacklist = realloc_object(word_blacklist, (word_blacklist_size + 1) * sizeof(char *));
        /*adds line's string*/
        *(word_blacklist + word_blacklist_size) = malloc_object(*(word_blacklist + word_blacklist_size),strlen(line));
        strcpy(*(word_blacklist + word_blacklist_size), line);
        /*increase size counter*/
        word_blacklist_size++;
    }
    /*close file*/
    fclose(fp);
}

/*file blacklist --exclude*/
char **update_file_blacklist(const char *file)
{
    /*reallocates blacklist size*/
    file_blacklist = realloc_object(file_blacklist, (file_blacklist_size + 1) * sizeof(char *));
    /*adds file path to blacklist*/
    *(file_blacklist + file_blacklist_size) = malloc_object(*(file_blacklist + file_blacklist_size),strlen(file));
    strcpy(*(file_blacklist + file_blacklist_size), file);
    /*increase size counter*/
    file_blacklist_size++;
}

/*wildcards list --wildcard*/
char **update_wildcard_list(const char *wildcard){
    /*reallocates wildcards list size*/
    wildcards_list = realloc_object(wildcards_list, (wildcards_list_size + 1) * sizeof(char*));
    /*adds wildcard to list*/
    *(wildcards_list + wildcards_list_size) = malloc_object(*(wildcards_list + wildcards_list_size), strlen(wildcard));
    strcpy(*(wildcards_list + wildcards_list_size), wildcard);
    /*increases size counter*/
    wildcards_list_size++;
}
/*actions based on option*/
static int parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *a = state->input;
    switch (key)
    {
    case 'r':
        recursive = true;
        break;
    case 'e':
        /*add current arg to file_blacklist*/
        update_file_blacklist(arg);
        break;
    case 'f':
        follow = true;
        break;
    case 'a':
        alpha = true;
        break;
    case 'm':
        /*converts string arg to int*/
        min = atoi(arg);
        break;
    case 'i':
        /*adds file's words in word_blacklist*/
        update_word_blacklist(arg);
        break;
    case 's':
        sort_by_occurency = true;
        break;
    case 'o':
        /*change output file path*/
        output_path = arg;
        break;
    case 'l':
        log_flag = true;
        /*open log file*/
        log_fp = fopen(arg, "w");
        if (log_fp == NULL)
        {
            printf("Can not open log file %s", arg);
            exit(EXIT_FAILURE);
        }
        /*print first line*/
        fprintf(log_fp, "FILE NAME, COUNTED WORDS, IGNORED WORDS, PROCESS TIME\n\n");
        break;
    case 'w':
        /*adds wildcard to wildacrd_list*/
        update_wildcard_list(arg);
        printf("%s\n", arg);
        break;
    case ARGP_KEY_ARG:
        /*argument without option: file or directory to precess*/
        argz_add(&a->argz, &a->argz_len, arg);
        break;
    case ARGP_KEY_INIT:
        /*initializes argz*/
        a->argz = 0;
        a->argz_len = 0;
        break;
    case ARGP_KEY_END:
        /*if there is no argument, argp_failure*/
        if (argz_count(a->argz, a->argz_len) <= 0)
        {
            argp_failure(state, 1, 0, "Missing file to process, please check --help");
        }
        break;
    }
    return 0;
}

/*Method that processes a file*/
void analyze_file(const char *path, trieNode *trie_node)
{
    /*first check if file corresponds to wildcard (if wildcard_list is not empty)*/
    if(wildcards_list_size != 0){
        bool is_valid_file = false;
        /*check all wildcards*/
        for(int i = 0; i < wildcards_list_size; i++){
            /*if fnmatch returns 0, file is accepted, otherwise file has to be skipped*/
            if(fnmatch(wildcards_list[i], path, 0) == 0){
                is_valid_file = true;
                break;
            }
        }
        if(!is_valid_file){
            printf("Skipping %s: file doesn't correspond to given wildcard\n", path);
            return;
        }
    }
    /*log variables*/
    int cw = 0;
    int iw = 0;
    /*begin time*/
    clock_t begin = clock();

    /*open file*/
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("Can not open file %s", path);
        exit(EXIT_FAILURE);
    }

    /*string containing delimiters*/
    char *delimit = " \t\n\r\v\f,.;:-@";

    /*initializes file's lines*/
    char *line = NULL;
    size_t len = 0;

    /*checks line by line*/
    while (getline(&line, &len, fptr) != -1)
    {
        /*
        word: string to analyze
        save: used by strtok_r for saving context
        */
        char *word;
        char *save;

        /*change \n to \0*/
        if (*(line + strlen(line) - 1) == '\n')
            *(line + strlen(line) - 1) = '\0';

        /*strtok_r extract the first string contained in line separated by delimit (delimiters previously specified)*/
        word = strtok_r(line, delimit, &save);

        /*word will be NULL when there will be no more tokens*/
        while (word != NULL)
        {
            /*boolean for checking word's validity*/
            bool is_valid = true;

            /*check --min*/
            if (strlen(word) >= min)
            {
                /*check --alpha*/
                if (alpha)
                {
                    /*check word_blacklist*/
                    if (!is_alphabetical_string(word) || is_in_blacklist(word, word_blacklist, word_blacklist_size))
                    {
                        is_valid = false;
                    }
                }
                else
                {
                    /*check word_blacklist*/
                    if (!is_alphanumerical_string(word) || is_in_blacklist(word, word_blacklist, word_blacklist_size))
                    {
                        is_valid = false;
                    }
                }
            }
            else
            {
                is_valid = false;
            }

            /*if valid, increase counter cw and add word to trie structure*/
            if (is_valid)
            {
                cw++;
                add_word(trie_root, word);
            }
            /*if not valid, increase counter iw*/
            else
            {
                iw++;
            }
            /*extract next token (every call after the first, we set the line to NULL)*/
            word = strtok_r(NULL, delimit, &save);
        }
    }

    /*closes file*/
    fclose(fptr);
    /*takes end time*/
    clock_t end = clock();
    /*calculates process time*/
    double process_time = (double)(end - begin) / CLOCKS_PER_SEC;

    /*if --log set, print informations in log file*/
    if (log_flag)
    {
        fprintf(log_fp, "%s, %d, %d, %f\n", path, cw, iw, process_time);
    }

    printf("Analyzed %s\n", path);
}

/*Analyze files in a directory*/
void analyze_directory(const char *path, trieNode *trie_node)
{
    /*pointer to directory*/
    DIR *dir;
    /*dirent.h structure*/
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        /*take all files in the directory*/
        while ((ent = readdir(dir)) != NULL)
        {
            /*takes file name*/
            char *filename = ent->d_name;
            /*we need to exclude "." and ".." directories to avoid infinite loops*/
            if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0)
            {
                /*allocates memory space for file path*/
                size_t path_size = (strlen(path) + strlen(filename) + 1);
                char *new_path = malloc_object(new_path, path_size * sizeof(char));
                strcpy(new_path, path);

                /*if there is no '\' character, we add at the end of directory name*/
                if (*(new_path + strlen(new_path) - 1) != '/')
                {
                    new_path = realloc_object(new_path, (path_size + 1) * sizeof(char));
                    strcat(new_path, "/");
                }
                /*concatenates file name*/
                strcat(new_path, filename);
                /*adds terminal character*/
                *(new_path + strlen(new_path)) = '\0';

                /*
                if file is a link, checks if --follow is enabled and checks if link is in file_blacklist*/
                if (is_symbolic_link(new_path))
                {
                    if (follow && !is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        analyze_file(new_path, trie_node);
                    }
                    else if (is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        printf("Skipped link %s: specified as file to be ignored\n", new_path);
                    }
                    else
                    {
                        printf("Skipped link %s: add -f or --follow option to analyze links too\n", new_path);
                    }
                }
                /*if regular file, checks if it is in file_blacklist*/
                if (is_regular_file(new_path))
                {
                    if (!is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        analyze_file(new_path, trie_node);
                    }
                    else
                    {
                        printf("Skipped file %s: specified as file to be ignored\n", new_path);
                    }
                }
                /*if directory, checks if --recursive is enabled and, in that case, recursively call current function*/
                if (is_directory(new_path) && recursive)
                {
                    analyze_directory(new_path, trie_node);
                }
                /*free new_path memory*/
                free(new_path);
            }
        }
    }
}

/*main function*/
int main(int argc, char **argv)
{
    /*
    options descriptions
    they will be displayed in case of --help
    */
    struct argp_option options[] = {
        {"recursive", 'r', 0, 0, "Check for subdirectories too"},
        {"exclude", 'e', "<file>", 0, "The specified file won't be considered"},
        {"follow", 'f', 0, 0, "Follow links too"},
        {"alpha", 'a', 0, 0, "Consider only words composed by alphabetical characters"},
        {"min", 'm', "<num>", 0, "Consider only words with at least <num> characters"},
        {"ignore", 'i', "<file>", 0, "Ignore words contained in <file> (a row is considered as a word)"},
        {"sortbyoccurrency", 's', 0, 0, "The analysis sorts words by occurency"},
        {"output", 'o', "<file>", 0, "Specify the output file (if not set, it will be automatically \"swordx.out\""},
        {"log", 'l', "<file>", 0, "At the end create a log file containing stats foreach file processed"},
        {"wildcard", 'w', "<wildcard>", 0, "Select only files corresponding to <wildcard> (REMEMBER: special characters requires '\\' escape characer)"},
        {0}};

    /*arguments structure (argz.h)*/
    struct arguments arguments;

    /*argp structure*/
    struct argp argp = {options, parse_opt, "<input1> <input2> ... <inputn>", "Count words occurencies in specified files or directories and save the reuslt in a file (swordx.out default)"};

    /*if line command is correct*/
    if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0)
    {
        /*save previous argument*/
        const char *previous = NULL;
        char *argument;

        /*initialize trie structure*/
        trie_root = create_trieNode();

        /*check arguments one by one*/
        while ((argument = argz_next(arguments.argz, arguments.argz_len, previous)))
        {
            /*update previous*/
            previous = argument;

            /*check file type*/
            if (is_symbolic_link(argument))
            {
                /*checks --follow and file_blacklist*/
                if (follow && !is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    analyze_file(argument, trie_root);
                }
                else if (is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    printf("Skipped link %s: specified as file to be ignored\n", argument);
                }
                else
                {
                    printf("Skipped link %s: add -f or --follow option to analyze links too\n", argument);
                }
            }
            else if (is_regular_file(argument))
            {
                /*checks file_blacklist*/
                if (!is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    analyze_file(argument, trie_root);
                }
                else
                {
                    printf("Skipped file %s: specified as file to be ignored\n\n", argument);
                }
            }
            else if (is_directory(argument))
            {
                analyze_directory(argument, trie_root);
            }
            /*if path doesn't exist, send error*/
            else{
                printf("ERROR: %s doesn't exist\n", argument);
                exit(EXIT_FAILURE);
            }
        }
        /*free argz memory*/
        free(arguments.argz);
    }

    /*open output file*/
    dest_fp = fopen(output_path, "w");
    if (dest_fp == NULL)
    {
        printf("Can not open file %s\n", output_path);
        exit(EXIT_FAILURE);
    }
    /*if not enabled --sortbyoccurrency, print trie in alphabetical order*/
    if (!sort_by_occurency)
    {
        print_trie(dest_fp, trie_root);
    }
    /*otherwise print in occurrency order*/
    else
    {
        /*initialize sorted list's root node*/
        sl_root *sl_root = create_sl_root();
        /*inserts trie structure's words in sorted list*/
        sl_root = sort_trie_by_occurrencies(trie_root, sl_root);
        /*orders the list*/
        qsort(sl_root->oc_nodes, sl_root->elements, sizeof(occurrencyNode), compare_occurrencyNodes);
        /*prints the list in output file*/
        print_sorted_list(dest_fp, sl_root);

        /*free sorted list memory space*/
        destroy_sorted_list(sl_root);
    }
    /*close output file*/
    fclose(dest_fp);

    /*free trie's memory*/
    destroy_trie(trie_root);

    /*if --log is set, we have to close log file*/
    if (log_flag)
    {
        fclose(log_fp);
    }

    return 0;
}
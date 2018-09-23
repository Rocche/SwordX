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


int main(void)
{
    trieNode *trie_root = create_trieNode();
    bool sorted = false;
    FILE *dest_fp;

    if (sorted == false)
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
/*argp guide: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
#include <argp.h>
#include <argz.h>
#include <dirent.h>
#include <time.h>

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
char **word_blacklist = NULL;
size_t word_blacklist_size = 0;

/*file blacklist (--exclude)*/
char **file_blacklist = NULL;
size_t file_blacklist_size = 0;

/*trie*/
trieNode *trie_root = NULL;

/*output default swordx.out (altro se specificato come parametro)*/
FILE *dest_fp = NULL;
char *output_path = "swordx.out";

/*log file*/
FILE *log_fp = NULL;

/*inizializza automaticamente l'opzione --version*/
const char *argp_program_version = "version 1.0";

/*struct per i parametri*/
struct arguments
{
    char *argz;
    size_t argz_len;
};

/*blacklist di parole da ignorare contenute in un file per l'opzione --ignore*/
char **update_word_blacklist(const char *path)
{

    /*apertura del file*/
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("Could not open the ignore file");
        exit(EXIT_FAILURE);
    }

    /*inizializzazione linee file*/
    char *line = NULL;
    size_t len = 0;

    /*controllo linea per linea fino alla fine del file*/
    while (getline(&line, &len, fp) != -1)
    {
        /*sostituisco \n con \0*/
        if (*(line + strlen(line) - 1) == '\n')
        {
            *(line + strlen(line) - 1) = '\0';
        }

        /*realloc della lista con una stringa in più*/
        word_blacklist = (char **)realloc(word_blacklist, (word_blacklist_size + 1) * sizeof(char *));
        check_heap(word_blacklist);
        /*aggiungo la stringa contenuta nella linea*/
        *(word_blacklist + word_blacklist_size) = (char *)malloc(strlen(line));
        check_heap(*(word_blacklist + word_blacklist_size));
        strcpy(*(word_blacklist + word_blacklist_size), line);
        /*aumento la dimensione della blacklist*/
        word_blacklist_size++;
    }
    /*chiusura del file*/
    fclose(fp);
}

/*blacklist contenente i file da escludere per l'opzione --exclude*/
char **update_file_blacklist(const char *file)
{
    /*realloc della lista aumentando di una stringa*/
    file_blacklist = (char **)realloc(file_blacklist, (file_blacklist_size + 1) * sizeof(char *));
    check_heap(file_blacklist);
    /*aggiungo il path del file*/
    *(file_blacklist + file_blacklist_size) = (char *)malloc(strlen(file));
    check_heap(*(file_blacklist + file_blacklist_size));
    strcpy(*(file_blacklist + file_blacklist_size), file);
    /*aumento la dimensione della blacklist*/
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
        /*aggiungo percorso del file (arg corrente) alla file_blacklist*/
        update_file_blacklist(arg);
        break;
    case 'f':
        follow = true;
        break;
    case 'a':
        alpha = true;
        break;
    case 'm':
        /*converto l'arg in intero*/
        min = atoi(arg);
        break;
    case 'i':
        /*aggiungo le parole del file con percorso arg alla word_blacklist*/
        update_word_blacklist(arg);
        break;
    case 's':
        sort_by_occurency = true;
        break;
    case 'o':
        /*il file di output viene cambiato in quello specificato*/
        output_path = arg;
        break;
    case 'l':
        log_flag = true;
        /*apertura del file di log con percorso arg*/
        log_fp = fopen(arg, "w");
        if (log_fp == NULL)
        {
            perror("Could not open log file");
            exit(EXIT_FAILURE);
        }
        break;
    case ARGP_KEY_ARG:
        /*argomento non accompagnato da opzioni: file o directory da processare*/
        argz_add(&a->argz, &a->argz_len, arg);
        break;
    case ARGP_KEY_INIT:
        /*inizializzazione di argz*/
        a->argz = 0;
        a->argz_len = 0;
        break;
    case ARGP_KEY_END:
        /*se non si è incontrato alcun argomento riguardanti file da processare, si esce con argp_failure*/
        if (argz_count(a->argz, a->argz_len) <= 0)
        {
            argp_failure(state, 1, 0, "Missing file to process, please check --help");
        }
        break;
    }
    return 0;
}

/*metodo che elabora le stringhe di un file*/
void analyze_file(const char *path, trieNode *trie_node)
{
    /*variabili per il file di log*/
    int cw = 0;
    int iw = 0;
    /*si inizia l'analisi del file: salvo il tempo corrente come tempo di inizio*/
    clock_t begin = clock();

    /*apertura del file*/
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    /*stringa contenente i delimitatori delle parole*/
    char *delimit = " \t\n\r\v\f,.;:-@";

    /*inizializzazione linee del file*/
    char *line = NULL;
    size_t len = 0;

    /*si controlla una riga alla volta fino a fine file*/
    while (getline(&line, &len, fptr) != -1)
    {
        /*
        word: stringa da analizzare
        save: usata da strtok_r per salvare il contesto
        */
        char *word;
        char *save;

        /*sostituisco \n con \0*/
        if (*(line + strlen(line) - 1) == '\n')
            *(line + strlen(line) - 1) = '\0';

        /*strtok_r estrae la prima stringa di line separata da delimit (delimitatori specificati precedentemente)*/
        word = strtok_r(line, delimit, &save);

        /*word sarà NULL quando non ci saranno più token da estrarre nella linea corrente*/
        while (word != NULL)
        {
            /*booleano per informare se una stringa è valida oppure no*/
            bool is_valid = true;

            /*controllo dell'opzione --min*/
            if (strlen(word) >= min)
            {
                /*controllo dell'opzione --alpha*/
                if (alpha)
                {
                    /*si controlla se la stringa sia contenuta nella blacklist*/
                    if (!is_alphabetical_string(word) || is_in_blacklist(word, word_blacklist, word_blacklist_size))
                    {
                        is_valid = false;
                    }
                }
                else
                {
                    /*si controlla se la stringa sia contenuta nella blacklist*/
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

            /*se la stringa è valida, si aumenta il contatore cw per il file di log e si aggiunge la parola al trie*/
            if (is_valid)
            {
                cw++;
                add_word(trie_root, word);
            }
            /*altrimenti si incrementa il contatore iw*/
            else
            {
                iw++;
            }
            /*si estrae il prossimo token (in ogni chiamata successiva alla prima, strtok_r richiede NULL se si vuole specificare la stessa linea)*/
            word = strtok_r(NULL, delimit, &save);
        }
    }

    /*finito di analizzare il file, si chiude*/
    fclose(fptr);
    /*fine analisi file: salvo il tempo corrente come tempo di fine*/
    clock_t end = clock();
    /*calcolo il tempo di processamento del file*/
    double process_time = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("%s, %d, %d, %f\n", path, cw, iw, process_time);

    /*se è stata specificata l'opzione --log, si stampa la linea con le informazioni raccolte dall'analisi*/
    if (log_flag)
    {
        fprintf(log_fp, "%s, %d, %d, %f\n", path, cw, iw, process_time);
    }
}

//metodo che elabora i file in una directory
void analyze_directory(const char *path, trieNode *trie_node)
{
    /*puntatore alla directory*/
    DIR *dir;
    /*struttura della libreria dirent.h*/
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        /*si continua finchè non si finiscono il file nella directory*/
        while ((ent = readdir(dir)) != NULL)
        {
            /*si prende il nome del file*/
            char *filename = ent->d_name;
            /*bisogna escludere le directory "." e ".." per evitare cicli infiniti*/
            if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0)
            {
                /*alloco spazio in memoria per il percors del file*/
                size_t path_size = (strlen(path) + strlen(filename) + 1);
                char *new_path = (char *)malloc(path_size * sizeof(char));
                check_heap(new_path);
                strcpy(new_path, path);

                /*se non è presente il carattere '/' alla fine del percorso della directory corrente lo aggiungo*/
                if (*(new_path + strlen(new_path) - 1) != '/')
                {
                    new_path = (char *)realloc(new_path, (path_size + 1) * sizeof(char));
                    check_heap(new_path);
                    strcat(new_path, "/");
                }
                /*concateno il nome del file*/
                strcat(new_path, filename);
                /*aggiungo il carattere terminale*/
                *(new_path + strlen(new_path)) = '\0';

                /*
                se il file è un link, si controlla che sia abilitata l'opzione --follow
                si controlla anche che il link non sia nella blacklist
                */
                if (is_symbolic_link(new_path))
                {
                    if (follow && !is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        analyze_file(new_path, trie_node);
                    }
                    else if (is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        printf("Saltato link %s: specificato come file da ignorare\n", new_path);
                    }
                    else
                    {
                        printf("Saltato link %s: per abilitare i link, aggiungere opzione -f o --follow\n", new_path);
                    }
                }
                /*in caso di file regolare, si controlla che il file non sia nella blacklist*/
                if (is_regular_file(new_path))
                {
                    if (!is_in_blacklist(new_path, file_blacklist, file_blacklist_size))
                    {
                        analyze_file(new_path, trie_node);
                    }
                    else
                    {
                        printf("Saltato file %s: specificato come file da ignorare\n", new_path);
                    }
                }
                /*in caso di directory, si richiama la funzione corrente ricorsivamente in caso di opzione --recursive*/
                if (is_directory(new_path) && recursive)
                {
                    analyze_directory(new_path, trie_node);
                }
                /*si libera l'allocazione di new_path*/
                free(new_path);
            }
        }
    }
}

/*main function*/
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
        {"output", 'o', "<file>", 0, "Specify the output file (if not set, it will be automatically \"swordx.out\""},
        {"log", 'l', "<file>", 0, "At the end create a log file containing stats foreach file processed"},
        {0}};

    /*struttura di arguments*/
    struct arguments arguments;

    /*struttura argp*/
    struct argp argp = {options, parse_opt, "<input1> <input2> ... <inputn>", "Count words occurencies in specified files or directories and save the reuslt in a file (swordx.out default)"};

    //print_logo();
    /*se il comando è corretto*/
    if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0)
    {
        /*salva l'argomento precedente*/
        const char *previous = NULL;
        char *argument;

        /*si inizializza il trie*/
        trie_root = create_trieNode();

        /*controlla gli argomenti uno per uno*/
        while ((argument = argz_next(arguments.argz, arguments.argz_len, previous)))
        {
            /*aggiornamento del precedente*/
            previous = argument;

            /*si controlla il tipo del file*/

            if (is_symbolic_link(argument))
            {
                /*controlli sull'opzione follow e file_blacklist*/
                if (follow && !is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    analyze_file(argument, trie_root);
                }
                else if (is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    printf("Saltato link %s: specificato come file da ignorare\n", argument);
                }
                else
                {
                    printf("Saltato link %s: per abilitare i link, aggiungere opzione -f o --follow\n", argument);
                }
            }
            else if (is_regular_file(argument))
            {
                /*controlli sulla file_blacklist*/
                if (!is_in_blacklist(argument, file_blacklist, file_blacklist_size))
                {
                    analyze_file(argument, trie_root);
                }
                else
                {
                    printf("Saltato file %s: specificato come file da ignorare\n", argument);
                }
            }
            else if (is_directory(argument))
            {
                analyze_directory(argument, trie_root);
            }
        }
        printf("\n");
        /*si libera la memoria allocata da argz*/
        free(arguments.argz);
    }

    /*apro il file di output*/
    dest_fp = fopen(output_path, "w");
    if (dest_fp == NULL)
    {
        perror("Cannot open file\n");
        exit(EXIT_FAILURE);
    }
    /*se non è attivata l'opzione --sortbyoccurrency, stampa il trie nel file di destinazione*/
    if (!sort_by_occurency)
    {
        print_trie(dest_fp, trie_root);
    }
    /*altrimenti ordina per occorrenza*/
    else
    {
        /*si crea il nodo root della sorted list*/
        sl_root *sl_root = create_sl_root();
        /*metodo che inserisce le parole del trie nella sorted list*/
        sl_root = sort_trie_by_occurrencies(trie_root, sl_root);
        /*si ordina per occorrenza*/
        qsort(sl_root->oc_nodes, sl_root->elements, sizeof(occurrencyNode), compare_occurrencyNodes);
        /*stampa nel file di destinazione*/
        print_sorted_list(dest_fp, sl_root);

        destroy_sorted_list(sl_root);
    }
    /*chiusura del file*/
    fclose(dest_fp);

    /*free trie's memory*/
    destroy_trie(trie_root);

    /*se è stata specificata l'opzione --log, bisogna chiudere il file già aperto in precedenza*/
    if (log_flag)
    {
        fclose(log_fp);
    }

    return 0;
}
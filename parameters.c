#include <stdio.h>
#include "parameters.h"

void show_help(){
    printf("Usage: swordx [options] [inputs]\n");
    printf("Search for word's occurencies in specified files\n\n");
    printf("OPTIONS:\n");
    printf("\t-h, --help: display this help and exit\n");
    printf("\t-r, --recursive: check for subdirectories too\n");
    printf("\t-e <file>, --exclude <file>: don't consider the specified file (only if you are working with directories)\n");
    printf("\t-a, --alpha: consider only words containing alphanumeric characters");
    printf("\t-m <num>, --min <num>: consider only words with the specified minimum characters\n");
    printf("\t-i <file>, --ignore <file>: the specified file must contain words that you want to exclude\n");
    printf("\t-s, --sortbyoccurency: words are sorted by occurency (not in alphabetical order)\n");
    printf("\t-l <file>, --log <file>: generate the specified log file where, foreach file analized, prints a record formed by <name> cw iw time:\n");
    printf("\t\t<name>: name of the file\n");
    printf("\t\tcw>: number of registered words\n");
    printf("\t\tiw: number of ignored words\n");
    printf("\t\ttime: elaboration's burst time\n");
    printf("\t-o <file>, --output <file>: data will be saved in the output file specified\n\n");
    printf("INPUTS:\n");
    printf("\t<path/file>: specifies the file you want to analyze\n");
    printf("\t<path/directory>: elaborates regular files contained in the specified directory\n");
    printf("\t<path/file1> <path/file2> <path/file3>...: specifies multiple files to analyze\n\n");
    printf("EXAMPLES:\n");
    printf("\tswordx Documents/myFolder/pippo.txt\tcount occurencies in pippo.txt and print the result in alphabetical order\n");
    printf("\tswordx Documents/myFolder/ -r -a -o Documents/Results/output.txt\tcount occurencies in all regular files in folder Documents, with subdirectories too, and print the result in alphabetical order\n\n");
}
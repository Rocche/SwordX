#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "trie.h"
#include "sbolist.h"

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
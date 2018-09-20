#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "trie.h"
#include "sbolist.h"

int main(void){

    trieNode* trie_root; // trie very first node
    char* trie_word; // word built visiting the trie node after node
        bool sorted;
        occurrencyNode** sl_root; // sorted list occurrencyNodes pointer
    trie_root = create_trieNode(trie_root);
    trie_word = allocate_str(trie_word,__LINE__);
    sl_root = create_sl_root(sl_root);

        FILE* dest_fp; // input file
        dest_fp = fopen("/home/ale/Desktop/Files/output","w");
        int max;
        max=10;
        trie_word = reallocate_str(trie_word,max,__LINE__);
        
        add_word(trie_root,"zattera\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"zattera\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"zattera\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"076\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"076\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"12\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"AlBa\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"ALBATROS\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"ALBATROS\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"ALBATROS\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"albERo\0", trie_word, 0);
        trie_word= clear_str(trie_word);

        add_word(trie_root,"albeREllo\0", trie_word, 0);
        trie_word= clear_str(trie_word);
        add_word(trie_root,"BAnana\0", trie_word, 0);
        trie_word= clear_str(trie_word);

    print_trie( dest_fp, trie_root, trie_word, 0 );
    
        fclose(dest_fp);

    return 0;

}
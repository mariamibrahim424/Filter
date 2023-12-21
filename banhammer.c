#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define WORD      "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"
#define BUFF_SIZE 1024
#define OPTIONS   "h:f:m"

int main(int argc, char **argv) {
    bool mtf = false;
    uint32_t ht_size = 10000;
    uint32_t bf_size = 1048576;
    int opt = 0;
    // loop through command line options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            ht_size = atoi(optarg); // specify size of hash table
            if (ht_size == 0) {
                fprintf(stderr, "Invalid hash table size.\n");
                return 1;
            }
            break;
        case 'f':
            bf_size = atoi(optarg); // specify size of bloom filter
            if (bf_size == 0) {
                fprintf(stderr, "Invalid Bloom Filter size.\n");
                return 1;
            }
            break;
        case 'm': mtf = true; break; // enable move-to-front option
        }
    }
    HashTable *ht = ht_create(ht_size, mtf);
    if (!ht) {
        fprintf(stderr, "Failed to create hash table.\n");
        return 1;
    }
    BloomFilter *bf = bf_create(bf_size);
    if (!bf) {
        fprintf(stderr, "Failed to create bloom filter.\n");
        return 1;
    }
    LinkedList *used_badspeak = ll_create(mtf); // ll used to keep track of badspeak words inputted
    LinkedList *used_oldspeak = ll_create(mtf); // ll used to keep track of oldspeak words inputted

    // read in words from badspeak.txt and insert them into bloom filter and hash table
    char badspeak_input[BUFF_SIZE];
    FILE *badspeak = fopen("badspeak.txt", "r");
    while (fscanf(badspeak, "%s\n", badspeak_input) != -1) {
        bf_insert(bf, badspeak_input);
        ht_insert(ht, badspeak_input, NULL);
    }
    // read in oldspeak and newspeak pairs from newspeak.txt and insert them bloom
    // filter and hash table
    char oldspeak_input[BUFF_SIZE];
    char newspeak_input[BUFF_SIZE];
    FILE *newspeak = fopen("newspeak.txt", "r");
    while (fscanf(newspeak, "%s %s\n", oldspeak_input, newspeak_input) != -1) {
        bf_insert(bf, oldspeak_input);
        ht_insert(ht, oldspeak_input, newspeak_input);
    }
    FILE *infile = stdin;
    regex_t re; // regex type re
    // compile regex
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, " Failed to compile regex .\n"); // throw an error if compilation fails
        return 1;
    }

    // read in words from stdin using regex
    char *word = NULL;
    while ((word = next_word(infile, &re))) {
        // convert uppercase letters to lowercase
        for (uint32_t i = 0; word[i] != '\0'; i += 1) {
            word[i] = tolower(word[i]);
        }
        // check if the word is in the bloom filter
        // if it is then, the hash table should also be checked
        bf_probe(bf, word);
        if (bf_probe(bf, word)) {
            ht_lookup(ht, word);
        } else {
            continue;
        }
        // if the word is in both the bloom filter and the hash table
        // check if it is an oldspeak or badspeak word
        if ((bf_probe(bf, word) == true) && (ht_lookup(ht, word) != NULL)) {
            // if the word does not have a newspeak translation, it is a badspeak word
            if (ht_lookup(ht, word)->newspeak == NULL) {
                ll_insert(used_badspeak, word, NULL);
            }
            // if the word has a newspeak translation, it is an oldspeak word
            if (ht_lookup(ht, word)->newspeak != NULL) {
                ll_insert(used_oldspeak, word, ht_lookup(ht, word)->newspeak);
            }
        }
    }

    // the user used both badspeak and oldspeak words
    if (((ll_length(used_badspeak) != 0)) && (ll_length(used_oldspeak) != 0)) {
        printf("Dear Comrade,\n\n");
        printf("You have chosen to use degenerate words that may cause hurt\nfeelings or cause "
               "your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct "
               "your wrongthink and\npreserve community consensus we will be sending you to "
               "joycamp\nadministered by Medellin's Miniluv. Beware of the hippos.\n\n");
        printf("Your errors:\n\n");
        ll_print(used_badspeak);
        printf("\n");
        printf("Think of these words on your vacation!\n\n");
        ll_print(used_oldspeak);
    }
    // the user used only badspeak words
    if (((ll_length(used_badspeak) != 0)) && (ll_length(used_oldspeak) == 0)) {
        printf("Dear Comrade,\n\n");
        printf("You have chosen to use degenerate words that may cause hurt\nfeelings or cause "
               "your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct "
               "your wrongthink and\npreserve community consensus we will be sending you to "
               "joycamp\nadministered by Medellin's Miniluv. Beware of the hippos.\n\n");
        printf("Your errors:\n\n");
        ll_print(used_badspeak);
    }
    // the user used only oldspeak words
    if (((ll_length(used_badspeak) == 0)) && (ll_length(used_oldspeak) != 0)) {
        printf("Dear Comrade,\n\n");
        printf("Submitting your text helps to preserve feelings and prevent\nbadthink. Some of the "
               "words that you used are not goodspeak.\nThe list shows how to turn the oldspeak "
               "words into newspeak.\n\n");
        ll_print(used_oldspeak);
    }
    ht_delete(&ht);
    ll_delete(&used_badspeak);
    ll_delete(&used_oldspeak);
    fclose(infile);
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    fclose(badspeak);
    fclose(newspeak);
    return 0;
}

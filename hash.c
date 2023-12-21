#include "hash.h"

#include "speck.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

// creates a hash table and returns a pointer to that hash table
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        // when used in hash function will return an index in the ht
        ht->salt[0] = 0x85ae998311115ae3; // Il nome della rosa
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size; // size of ht
        ht->mtf = mtf; //move-to-front technique
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// frees the hash table
// which frees each linked list
// then the hash table
void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < (*ht)->size; i += 1) {
        if ((*ht)->lists[i] != NULL) {
            ll_delete(&((*ht)->lists[i]));
        }
    }
    free((*ht)->lists);
    free(*ht);
    (*ht) = NULL;
    return;
}

// returns the size of the hash table
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// searches for a linked list with a node that contains oldspeak
// if it is found a pointer to the node is returned, else NULL is returned
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    // hashing oldsoeak with the salt will return an index
    // where the oldspeak value should be inserted
    // limit the index to the size of the hash table
    uint32_t hash_code_index = (hash(ht->salt, oldspeak)) % (ht->size);
    return ll_lookup(ht->lists[hash_code_index], oldspeak);
}

// inserts elements into the hash table
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t hash_code_index = (hash(ht->salt, oldspeak)) % (ht->size);
    // if there does not already exist a linked list at the index
    // create a linked list before inserting contents
    if (ht->lists[hash_code_index] == NULL) {
        ht->lists[hash_code_index] = ll_create(ht->mtf);
        ll_insert(ht->lists[hash_code_index], oldspeak, newspeak);
    }
    // if there exists a linked list at the index
    // insert the contents in the existing linked list
    if (ht->lists[hash_code_index] != NULL) {
        ll_insert(ht->lists[hash_code_index], oldspeak, newspeak);
    }
    return;
}

// prints out contents of a hash table
// which are the contents of each linked list
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i += 1) {
        printf("ht[%u] ", i);
        printf("\n");
        if (ht->lists[i] != NULL) {
            ll_print(ht->lists[i]);
        }
        printf("\n");
    }
    return;
}

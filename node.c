#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// creates a node and returns a pointer to that node
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = ((Node *) calloc(1, sizeof(Node)));
    // before allocating memory for oldspeak and newspeak pointers in the node
    // ensure that newspeak and oldspeak are words(not NULL)
    if ((oldspeak != NULL) && (newspeak != NULL)) {
        n->newspeak = (char *) calloc((strlen(newspeak)) + 1, sizeof(char)); // +1 for '\0'
        n->oldspeak = (char *) calloc((strlen(oldspeak)) + 1, sizeof(char));
        if ((n->newspeak) && (n->oldspeak)) {
            strcpy(n->newspeak, newspeak);
            strcpy(n->oldspeak, oldspeak);
        }
    }
    // if the word is a badspeak, only allocate memory for badspeak(oldspeak)
    // because it does not have a newspeak translation
    if ((oldspeak != NULL) && (newspeak == NULL)) {
        n->oldspeak = (char *) calloc((strlen(oldspeak)) + 1, sizeof(char));
        if (n->oldspeak) {
            strcpy(n->oldspeak, oldspeak);
        }
    }
    // a node originally won't have neighbors
    n->next = NULL;
    n->prev = NULL;

    return n;
}

// frees a node
void node_delete(Node **n) {
    // a node will always have an oldspeak
    // but it might not have a newspeak translation
    if (*n) {
        free((*n)->oldspeak);
        if ((*n)->newspeak != NULL) {
            free((*n)->newspeak);
        }
        free(*n);
        (*n) = NULL;
    }
    return;
}

// prints contents of a node
void node_print(Node *n) {
    if ((n->oldspeak != NULL) && (n->newspeak != NULL)) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if ((n->oldspeak != NULL) && (n->newspeak == NULL)) {
        printf("%s\n", n->oldspeak);
    }
    return;
}

#include "ll.h"

#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node
    Node *tail; // Tail sentinel node
    bool mtf;
};

// creates a linked list and returns a pointer to that linked list
// the mtf parameter will move frequently used words in the beginning of the list
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = ((LinkedList *) calloc(1, sizeof(LinkedList)));
    if (ll) {
        ll->length = 0;
        // the sentinel nodes don't contain oldspeak/badspeak
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);
        // when list is empty, the sentinel nodes point at each other
        // and don't point at any other nodes
        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;
        ll->mtf = mtf;
        ll->head->prev = NULL;
        ll->tail->next = NULL;
    }
    return ll;
}

// frees the linked list
// which frees each node
// then the ll
void ll_delete(LinkedList **ll) {
    while ((*ll)->head != NULL) {
        Node *next = (*ll)->head->next;
        node_delete(&(*ll)->head);
        ((*ll)->head) = next;
    }
    free(*ll);
    (*ll) = NULL;
    return;
}

// returns the length of the linked list (excluding sentinel nodes)
uint32_t ll_length(LinkedList *ll) {
    uint32_t length = 0;
    for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
        length += 1;
    }
    return length;
}

// searches for an oldspeak word in the linked list
// if it is found, a pointer to the node where oldspeak is located is returned
// othwise null is returned
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    if (ll) {
        for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
            if (strcmp(n->oldspeak, oldspeak) == 0) {
                // if mtf is true, the node will be moved to the front of list
                if (ll->mtf) {
                    n->prev->next = n->next; // node before n points to node after n
                    n->next->prev = n->prev; // node after n points to node before n
                    n->next = ll->head->next; // node after n is node that head was pointing to
                    n->prev = ll->head; // node before n should be head
                    ll->head->next->prev = n; // node after head points to n
                    ll->head->next = n; // node after head should be n
                    return n;
                }
                return n;
            }
        }
    }
    return NULL;
}

// creates and insert a node with oldspeak and newspeak
// if it is not already in the linked list
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if ((ll_lookup(ll, oldspeak)) == NULL) {
        Node *n = node_create(oldspeak, newspeak);
        n->next = ll->head->next;
        n->prev = ll->head;
        ll->head->next->prev = n;
        ll->head->next = n;
        ll->length += 1;
    }
    return;
}

// prints contents of linked list
void ll_print(LinkedList *ll) {
    for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
        if (n != NULL) {
            node_print(n);
        }
    }
    return;
}

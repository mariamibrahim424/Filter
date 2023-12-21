#include "bv.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// takes in number of bits and returns bytes(there are 8 bits in a byte)
// if the bits are not divisble by 8, one has to take the ceiling because an extra byte will be
// needed
uint32_t bytes(uint32_t bits) {
    if (bits % 8 == 0) {
        return (bits / 8);
    } else {
        return (bits / 8 + 1);
    }
}

// creates a bit vector and returns a pointer to that bit vector
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) calloc(1, sizeof(BitVector));
    if (bv) {
        bv->vector = (uint8_t *) calloc(bytes(length), sizeof(uint8_t));
        if (bv->vector) {
            bv->length = length;
            return bv;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

// frees the bit vector
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    (*bv) = NULL;
    return;
}
// return length of bit vector
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// create a mask with 1 at the specified bit by shifting and then "anding" that will
// set the specified bit to 1
void bv_set_bit(BitVector *bv, uint32_t i) {
    uint32_t mask = 1 << i % 8;
    bv->vector[i / 8] = bv->vector[i / 8] | mask;
    return;
}

// create a mask with 0 at the specified bit by shifting and then "anding" that will
// turn the specified bit into 0
void bv_clr_bit(BitVector *bv, uint32_t i) {
    uint32_t mask = ~(1 << i % 8);
    bv->vector[i / 8] = bv->vector[i / 8] & mask;
    return;
}

// create a mask with 1 at the specified bit by shifting and then "anding" that will give
// the bit and to get 1 or 0 one has to shift it back
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    uint32_t mask = 1 << i % 8;
    uint32_t result = bv->vector[i / 8] & mask;
    result = result >> i % 8;
    return result;
}

// prints the bit vector
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i += 1) {
        printf("%c ", '0' + bv_get_bit(bv, i));
        printf("\n");
    }
    printf("\n");
    return;
}

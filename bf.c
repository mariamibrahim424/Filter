#include "bf.h"

#include "bv.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt .
    uint64_t secondary[2]; // Secondary hash function salt .
    uint64_t tertiary[2]; // Tertiary hash function salt .
    BitVector *filter;
};

// creates a bloom filter and returns a pointer to that bloom filter
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        // Fear & Loathing in Las Vegas
        bf->primary[0] = 0x02d232593fbe42ff;
        bf->primary[1] = 0x3775cfbf0794f152;
        // A Moveable Feast
        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;
        // The Cremation of Sam McGee
        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;
        bf->filter = bv_create(size); // the bloom filter is the bit vecctor
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// frees the bloom filter
void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    (*bf) = NULL;
}

// returns the length of the bloom filter
uint32_t bf_length(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// to insert an element in the bloom filter
// the element has to be hashed with the 3 salts wihch return 3 indices
// the 3 bits at the 3 indices are then set
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t first = (hash(bf->primary, oldspeak)) % (bf_length(bf));
    uint32_t second = (hash(bf->secondary, oldspeak)) % (bf_length(bf));
    uint32_t third = (hash(bf->tertiary, oldspeak)) % (bf_length(bf));
    bv_set_bit(bf->filter, first);
    bv_set_bit(bf->filter, second);
    bv_set_bit(bf->filter, third);
    return;
}
// checks if an element might be in the bloom filter
// returns true if it is,
// false otherwise
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t first = (hash(bf->primary, oldspeak)) % (bf_length(bf));
    uint32_t second = (hash(bf->secondary, oldspeak)) % (bf_length(bf));
    uint32_t third = (hash(bf->tertiary, oldspeak)) % (bf_length(bf));
    // if all 3 bits are set then the element is most likely in the bloom filter
    if ((bv_get_bit(bf->filter, first)) && (bv_get_bit(bf->filter, second))
        && (bv_get_bit(bf->filter, third))) {
        return true;
    } else {
        return false;
    }
}

// prints out the bloom filter which is just the bit vector
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}

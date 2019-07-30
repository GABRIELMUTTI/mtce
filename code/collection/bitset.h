#ifndef COL_BITSET_H
#define COL_BITSET_H

#include <stdint.h>

struct bitset {
    uint8_t *bitset;
    unsigned int size;
};

void bitset_init(struct bitset *bitset);
void bitset_free(struct bitset *bitset);

void bitset_set(struct bitset *bitset, unsigned int bit, unsigned int value);
void bitset_set_bits(struct bitset *bitset, unsigned int bits[], unsigned int size);
unsigned int bitset_get(struct bitset *bitset, unsigned int bit);

void bitset_resize(struct bitset *bitset, unsigned int num_bits);

#endif /* BITSET_H */

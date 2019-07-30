#include "bitset.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

void bitset_init(struct bitset *bitset)
{
	bitset->size = 0;
	bitset->bitset = NULL;

}

inline void bitset_free(struct bitset *bitset)
{
	free(bitset->bitset);
}

void bitset_set(struct bitset *bitset, unsigned int bit, unsigned int value)
{
	unsigned int byte = floor(bit / sizeof(uint8_t));
	bitset->bitset[byte] = (bitset->bitset[byte] & ~(1 << bit)) | (value << bit);
}

void bitset_set_bits(struct bitset *bitset, unsigned int bits[], unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		bitset_set(bitset, bits[i], 1);
}

unsigned int bitset_get(struct bitset *bitset, unsigned int bit) {
	unsigned int byte = floor(bit / sizeof(uint8_t));
	return bitset->bitset[byte] & (1 << bit);
}

void bitset_resize(struct bitset *bitset, unsigned int num_bits)
{
	bitset->bitset = realloc(bitset->bitset, num_bits);
	bitset->size = (num_bits / 8) + 1;
	memset(bitset->bitset, 0, bitset->size);
}

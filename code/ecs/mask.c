#include "mask.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

void mtecs_mask_init(struct mtecs_mask *mask, unsigned int num_bits)
{
    mask->size = (num_bits / 8) + 1;
    mask->mask = malloc(sizeof(uint8_t) * mask->size);
    memset(mask->mask, 0, 2);
}

inline void mtecs_mask_free(struct mtecs_mask *mask)
{
    free(mask->mask);
}

void mtecs_mask_set(struct mtecs_mask *mask, unsigned int bit, unsigned int value)
{
    unsigned int byte = floor(bit / sizeof(uint8_t));
    mask->mask[byte] = (mask->mask[byte] & ~(1 << bit)) | (value << bit);
}

unsigned int mtecs_mask_get(struct mtecs_mask *mask, unsigned int bit) {
    unsigned int byte = floor(bit / sizeof(uint8_t));
    return mask->mask[byte] & (1 << bit);
}


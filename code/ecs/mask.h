#ifndef MTECS_MASK_H
#define MTECS_MASK_H

#include <stdint.h>

struct mtecs_mask {
    uint8_t *mask;
    unsigned int size;
};

void mtecs_mask_init(struct mtecs_mask *mask, unsigned int num_bits);
void mtecs_mask_free(struct mtecs_mask *mask);

void mtecs_mask_set(struct mtecs_mask *mask, unsigned int bit, unsigned int value);
unsigned int mtecs_mask_get(struct mtecs_mask *mask, unsigned int bit);

#endif /* MTECS_MASK_H */

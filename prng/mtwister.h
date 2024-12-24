#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include <stdint.h>

void init_genrand(uint32_t seed);
uint32_t genrand_int32(void);

#endif
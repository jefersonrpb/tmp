#include <stdint.h>
#include <stdlib.h>
#define get16bits(d) (*((const uint16_t *) (d)))

uint32_t create_hash(const char * data, int len);


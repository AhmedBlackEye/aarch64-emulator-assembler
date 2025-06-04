#ifndef ENCODE_LOAD_STORE_H
#define ENCODE_LOAD_STORE_H

#include <stdint.h>
#include <stdbool.h>

uint32_t encode_load(const char *args[], int size);

uint32_t encode_store(const char *args[], int size);

#endif 

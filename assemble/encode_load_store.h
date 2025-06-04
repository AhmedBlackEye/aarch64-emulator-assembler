#ifndef ENCODE_LOAD_STORE_H
#define ENCODE_LOAD_STORE_H

#include <stdint.h>
#include <stdbool.h>

uint32_t encode_load(char *args[], int size);

uint32_t encode_store(char *args[], int size);

#endif 

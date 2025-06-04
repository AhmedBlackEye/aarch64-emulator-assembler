#ifndef ENCODE_LOAD_STORE_H
#define ENCODE_LOAD_STORE_H

#include <stdint.h>
#include <stdbool.h>

/*
 * @brief Encodes a load instruction into a 32 bit binary word.
 */
uint32_t encode_load(const char *args[], int size);

/*
 * @brief Encodes a store instruction into a 32 bit binary word.
 */
uint32_t encode_store(const char *args[], int size);

#endif 

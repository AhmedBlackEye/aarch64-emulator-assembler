#ifndef LOAD_STORE_ASM_H
#define LOAD_STORE_ASM_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Encodes a single data transfer (load/store) instruction in ARMv8 assembly.
 *
 * @param args     Array of string arguments to the instruction.
 * @param size     Number of arguments provided.
 * @param is_load  true for load instructions, false for store.
 * @return         32-bit encoded instruction.
 */
uint32_t encode_single_data_tranfer(const char *args[], uint8_t size, bool is_load);

#endif

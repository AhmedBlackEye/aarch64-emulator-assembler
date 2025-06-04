#ifndef ENCODE_DP_IMM_H
#define ENCODE_DP_IMM_H

#include <stdint.h>

// Move the glboal or something similar
typedef enum {
    STANDARD, 
    RD_ZR,   
    RN_ZR   
} INSTRUCTION_TYPE;

uint32_t encode_movn  (const char **tokens, int size);
uint32_t encode_movz  (const char **tokens, int size);
uint32_t encode_movk  (const char **tokens, int size);
uint32_t encode_arith_imm(const char **tokens, int size, uint8_t opc, INSTRUCTION_TYPE type);

#endif
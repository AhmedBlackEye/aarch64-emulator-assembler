#ifndef ENCODE_DP_IMM_H
#define ENCODE_DP_IMM_H

#include <stdint.h>

uint32_t encode_add   (const char **tokens, int size);
uint32_t encode_adds  (const char **tokens, int size);
uint32_t encode_sub   (const char **tokens, int size);
uint32_t encode_subs  (const char **tokens, int size);
uint32_t encode_cmp   (const char **tokens, int size);
uint32_t encode_cmn   (const char **tokens, int size);
uint32_t encode_neg   (const char **tokens, int size);
uint32_t encode_movn  (const char **tokens, int size);
uint32_t encode_movz  (const char **tokens, int size);
uint32_t encode_movk  (const char **tokens, int size);
uint32_t encode_arith_imm(const char **tokens, int size, uint8_t opc);

#endif
#ifndef ENCODE_DP_IMM_H
#define ENCODE_DP_IMM_H

#include <stdint.h>

uint32_t encode_add   (char **tokens, int size);
uint32_t encode_adds  (char **tokens, int size);
uint32_t encode_sub   (char **tokens, int size);
uint32_t encode_subs  (char **tokens, int size);
uint32_t encode_cmp   (char **tokens, int size);
uint32_t encode_cmn   (char **tokens, int size);
uint32_t encode_neg   (char **tokens, int size);
uint32_t encode_negs  (char **tokens, int size);
uint32_t encode_movn  (char **tokens, int size);
uint32_t encode_movz  (char **tokens, int size);
uint32_t encode_movk  (char **tokens, int size);
uint32_t encode_arith_imm(const char **tokens, int size, uint8_t opc);

#endif
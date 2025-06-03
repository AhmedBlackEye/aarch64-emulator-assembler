#ifndef ENCODE_DP_REG_H
#define ENCODE_DP_REG_H

#include <stdint.h>

uint32_t encode_and  (char **tokens, int size);
uint32_t encode_bic  (char **tokens, int size);
uint32_t encode_orr  (char **tokens, int size);
uint32_t encode_orn  (char **tokens, int size);
uint32_t encode_eor  (char **tokens, int size);
uint32_t encode_eon  (char **tokens, int size);
uint32_t encode_ands (char **tokens, int size);
uint32_t encode_bics (char **tokens, int size);
uint32_t encode_tst  (char **tokens, int size);
uint32_t encode_mvn  (char **tokens, int size);
uint32_t encode_mov  (char **tokens, int size);
uint32_t encode_madd(char **tokens, int size);
uint32_t encode_msub(char **tokens, int size);
uint32_t encode_mul (char **tokens, int size);
uint32_t encode_mneg(char **tokens, int size);
uint32_t encode_arith_reg(char **tokens, int size, uint8_t opc);

#endif
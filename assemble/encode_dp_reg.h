#ifndef ENCODE_DP_REG_H
#define ENCODE_DP_REG_H

#include <stdint.h>
#include "global.h"

uint32_t encode_and   (const char **tokens, int size);
uint32_t encode_bic   (const char **tokens, int size);
uint32_t encode_orr   (const char **tokens, int size);
uint32_t encode_orn   (const char **tokens, int size);
uint32_t encode_eor   (const char **tokens, int size);
uint32_t encode_eon   (const char **tokens, int size);
uint32_t encode_ands  (const char **tokens, int size);
uint32_t encode_bics  (const char **tokens, int size);
uint32_t encode_tst   (const char **tokens, int size);
uint32_t encode_mvn   (const char **tokens, int size);
uint32_t encode_mov   (const char **tokens, int size);
uint32_t encode_madd  (const char **tokens, int size);
uint32_t encode_msub  (const char **tokens, int size);
uint32_t encode_mul   (const char **tokens, int size);
uint32_t encode_mneg  (const char **tokens, int size);
uint32_t encode_arith_reg(const char **tokens, int size, uint8_t opc, INSTRUCTION_TYPE type);

#endif
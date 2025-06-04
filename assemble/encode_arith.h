#ifndef ENCODE_ARITH_H
#define ENCODE_ARITH_H

#include <stdint.h>

uint32_t encode_add   (const char **tokens, int size);
uint32_t encode_adds  (const char **tokens, int size);
uint32_t encode_sub   (const char **tokens, int size);
uint32_t encode_subs  (const char **tokens, int size);
uint32_t encode_cmp   (const char **tokens, int size);
uint32_t encode_cmn   (const char **tokens, int size);
uint32_t encode_neg   (const char **tokens, int size);
uint32_t encode_negs  (const char **tokens, int size);

#endif
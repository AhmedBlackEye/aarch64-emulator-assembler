#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>

/**
 * Branch instruction encoders that match the encoder_table interface
 */
uint32_t encode_b(const char **tokens, int size);      // b <literal>
uint32_t encode_br(const char **tokens, int size);     // br xn

// Conditional branches
uint32_t encode_b_eq(const char **tokens, int size);   // b.eq <literal>
uint32_t encode_b_ne(const char **tokens, int size);   // b.ne <literal>
uint32_t encode_b_ge(const char **tokens, int size);   // b.ge <literal>
uint32_t encode_b_lt(const char **tokens, int size);   // b.lt <literal>
uint32_t encode_b_gt(const char **tokens, int size);   // b.gt <literal>
uint32_t encode_b_le(const char **tokens, int size);   // b.le <literal>
uint32_t encode_b_al(const char **tokens, int size);   // b.al <literal>

#endif
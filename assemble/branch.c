#include <string.h>
#include <stdio.h>
#include <stdbool.h> 
#include <stdint.h>
#include <stdlib.h>
#include "global.h"
#include "debug.h"
#include "sym_table.h"

// CONDITION CODE VALUES
#define EQ 0x0
#define NE 0x1
#define GE 0xA
#define LT 0xB
#define GT 0xC
#define LE 0xD
#define AL 0xE

#define MASK 0xFFFFFFFF
#define MAX_26 33554432
#define MAX_19 262143
#define ZERO_REG 0x1F

#define B_BITS 0x05
#define BR_BITS 0xD61F0
#define BCOND_BITS 0x54

static inline uint32_t mask_bits(int num_bits) {
    if (num_bits <= 0) return 0;
    if (num_bits >= 32) return MASK;
    return (1U << num_bits ) -1;
}

static uint32_t get_condition_code(const char *mnemonic) {
    const char *dot = strchr(mnemonic, '.');

    const char *condition = dot + 1;

    if STR_EQUAL(condition, "eq") return EQ;
    if STR_EQUAL(condition, "ne") return NE;
    if STR_EQUAL(condition, "ge") return GE;
    if STR_EQUAL(condition, "lt") return LT;
    if STR_EQUAL(condition, "gt") return GT;
    if STR_EQUAL(condition, "le") return LE;
    if STR_EQUAL(condition, "al") return AL; 

    PANIC("invalid condition code: %s", condition);
    return 0; 
}

uint32_t encode_b(const char **tokens, int size) {
    PANIC_IF(size != 1, "Size of b <literal> instruction must be 1. Size was: %d", size);
    
    uint32_t target_address = (uint32_t) symtab_lookup(tokens[0]);
    int64_t offset = (int64_t) target_address - (int64_t) curr_instr_addr;
    int64_t simm26_val = offset / 4;

    PANIC_IF(simm26_val < -MAX_26 || simm26_val > MAX_26, "Unconditional branch offset %ld out of range", simm26_val);

    return (B_BITS << 26) | (simm26_val & mask_bits(26)); 
}

uint32_t encode_br(const char **tokens, int size) {
    PANIC_IF(size != 1, "Size of br <register> instruction must be 1. Size was: %d", size);
    PANIC_IF(tokens[0][0] != 'x', "Expected a regiser for br <register> but got %s", tokens[0]);

    uint32_t target_register = (uint32_t)atoi(tokens[0] + 1);

    PANIC_IF(target_register > 30, "Error: Invalid register for branch: X%u", target_register);
    PANIC_IF(target_register == ZERO_REG, "Error: Zero register access not handled");

    return (BR_BITS << 12) | (target_register << 5); 
}

static uint32_t encode_conditional_branch_generic(const char **tokens, int size, const char *mnemonic) {
    PANIC_IF(size != 1, "Conditional branch expects 1 operand, got %d", size);
    
    uint32_t target_address = (uint32_t)symtab_lookup(tokens[0]);
    int64_t offset = (int64_t)target_address - (int64_t)curr_instr_addr;
    
    int64_t simm19_val = offset / 4;
    uint32_t cond = get_condition_code(mnemonic);

    PANIC_IF(simm19_val < -MAX_19 || simm19_val > MAX_19, "Conditional branch offset %ld out of range", simm19_val);
             
    return (BCOND_BITS << 24) | ((simm19_val & mask_bits(19)) << 5) | cond;
}


uint32_t encode_b_eq(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.eq");
}

uint32_t encode_b_ne(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.ne");
}

uint32_t encode_b_ge(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.ge");
}

uint32_t encode_b_lt(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.lt");
}

uint32_t encode_b_gt(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.gt");
}

uint32_t encode_b_le(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.le");
}

uint32_t encode_b_al(const char **tokens, int size) {
    return encode_conditional_branch_generic(tokens, size, "b.al");
}


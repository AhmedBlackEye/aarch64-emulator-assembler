#include <string.h>
#include <stdio.h>
#include <stdbool.h> 
#include <stdint.h>
#include <stdlib.h>
#include "global.h"
#include "debug.h"
#include "sym_table.h"

static inline uint32_t mask_bits(int num_bits) {
    if (num_bits <= 0) return 0;
    if (num_bits >= 32) return 0xFFFFFFFF;
    return (1U << num_bits ) -1;
}

typedef enum Branch_Conditions {
    BRANCH_UNCONDITIONAL, // b <literal>
    BRANCH_REGISTER, // br xn
    BRANCH_CONDITIONAL //b.cond <literal>
} branch_type_t; 

static bool check_valid_condition_code(const char *cond);
static uint32_t get_condition_code(const char *mnemonic);


branch_type_t detect_branch_type (const char *mnemonic) {
    PANIC_IF ((!mnemonic || mnemonic[0] != 'b'), "Error: Mnemonic \"%s\" unknown, branch not found", mnemonic);

    switch (mnemonic[1]) {
        case '\0': // Unconditional branch: b <literal>
            return BRANCH_UNCONDITIONAL;
        case 'r': // Register branch: br xn
            PANIC_IF(mnemonic[2] != '\0', "Error: Invalid register branch %s", mnemonic);
            return BRANCH_REGISTER;
        case '.':
            {
                const char *condition = &mnemonic[2];
                PANIC_IF(!check_valid_condition_code(condition), "Error: Invalid condition code %s", mnemonic);
                return BRANCH_CONDITIONAL;
            }
        default: 
            PANIC("Error: Unknown mnemonic condition code: %s", mnemonic);

    }
}


static uint32_t get_condition_code(const char *mnemonic) {
    const char *dot = strchr(mnemonic, '.');

    const char *condition = dot + 1;

    if STR_EQUAL(condition, "eq") return 0x0;
    if STR_EQUAL(condition, "ne") return 0x1;
    if STR_EQUAL(condition, "ge") return 0xA;
    if STR_EQUAL(condition, "lt") return 0xB;
    if STR_EQUAL(condition, "gt") return 0xC;
    if STR_EQUAL(condition, "le") return 0xD;
    if STR_EQUAL(condition, "al") return 0xE; 

    PANIC("invalid condition code: %s", condition);
    return 0; 
}

static bool check_valid_condition_code(const char *cond) {
    const char *valid_codes[] = {
        "eq", "ne", "ge", "lt", "gt", "le", "al"
    };
    for (int i = 0; valid_codes[i] != NULL; i++) {
        if (STR_EQUAL(cond, valid_codes[i])) {
            return true;
        }
    }
    return false;
}

uint32_t encode_b(const char **tokens, int size) {
    PANIC_IF(size != 1, "Size of b <literal> instruction must be 1. Size was: %d", size);
    
    uint32_t target_address = (uint32_t) symtab_lookup(tokens[0]);
    int64_t offset = (int64_t) target_address - (int64_t) curr_instr_addr;
    int64_t simm26_val = offset / 4;

     PANIC_IF(simm26_val < -33554432 || simm26_val > 33554431, "Unconditional branch offset %ld out of range", simm26_val);

    // Combine to make unconditional branch encoding: 000101 + simm26
    return (0x05 << 26) | (simm26_val & mask_bits(26)); 
}


uint32_t encode_br(const char **tokens, int size) {
    PANIC_IF(size != 1, "Size of br <register> instruction must be 1. Size was: %d", size);
    PANIC_IF(tokens[0][0] != 'x', "Expected a regiser for br <register> but got %s", tokens[0]);

    uint32_t target_register = (uint32_t)atoi(tokens[0] + 1);

    PANIC_IF(target_register > 30, "Error: Invalid register for branch: X%u", target_register);
    PANIC_IF(target_register == 0x1F, "Error: Zero register access not handled");

    // Register branch encoding: 1101011000011111000000 + xn (5 bits) + 00000
    // 1101 0110 0001 1111 0000 00[xn5][xn4] [xn3][xn2][xn1]0 0000
    return (0xD61F0 << 12) | (target_register << 5); 
}

// Generic function for conditional branches: b.cond <literal>
static uint32_t encode_conditional_branch_generic(const char **tokens, int size, const char *mnemonic) {
    PANIC_IF(size != 1, "Conditional branch expects 1 operand, got %d", size);
    
    // Branch target is always a label in typical ARMv8 assembly
    uint32_t target_address = (uint32_t)symtab_lookup(tokens[0]);
    int64_t offset = (int64_t)target_address - (int64_t)curr_instr_addr;
    
    int64_t simm19_val = offset / 4;
    uint32_t cond = get_condition_code(mnemonic);
    // Check range for 19-bit signed immediate
    PANIC_IF(simm19_val < -262144 || simm19_val > 262143, "Conditional branch offset %ld out of range", simm19_val);
             
    // Conditional branch follows encoding: 0101 0100 [simm19]0 [cond]
    return (0x54 << 24) | ((simm19_val & mask_bits(19)) << 5) | cond;
}

// Encode branch conditionals.
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


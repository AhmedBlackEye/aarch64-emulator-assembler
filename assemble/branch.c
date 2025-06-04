#include <string.h>
#include <stdio.h>
#include <stdbool.h> 
#include <stdint.h>
#include <assemble/global.h>
#include <common/debug.h>

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
            const char *condition = &mnemonic[2];
            PANIC_IF(!check_valid_condition_code(condition), "Error: Invalid condition code %s", mnemonic);
            return BRANCH_CONDITIONAL;
        default: 
            PANIC("Error: Unknown mnemonic condition code: %s", condition);

    }
}


static uint32_t get_condition_code(const char *mnemonic) {
    const char *dot = strchr(mnemonic, '.');

    const char *condition = dot + 1;

    if IS_EQUAL(condition, "eq") return 0x0;
    if IS_EQUAL(condition, "ne") return 0x1;
    if IS_EQUAL(condition, "ge") return 0xA;
    if IS_EQUAL(condition, "lt") return 0xB;
    if IS_EQUAL(condition, "gt") return 0xC;
    if IS_EQUAL(condition, "le") return 0xD;
    if IS_EQUAL(condition, "al") return 0xE; 
}

static bool check_valid_condition_code(const char *cond) {
    const char *valid_codes[] = {
        "eq", "ne", "ge", "lt", "gt", "le", "al"
    };
    for (int i = 0; valid_codes[i] != NULL; i++) {
        if (IS_EQUAL(cond, valid_codes[i])) {
            return true;
        }
    }
    return false;
}

uint32_t encode_unconditional_branch(int64_t offset) {
    int64_t simm26_val = offset / 4;
    // Combine to make unconditional branch encoding: 000101 + simm26
    return (0x05 << 26) | (simm26_val & mask_bits(26)); 
}

uint32_t encode_register_branch(uint32_t xn_reg) {
    PANIC_IF(xn_reg > 30, "Error: Invalid register for branch: X%u", xn_reg);
    PANIC_IF(xn_reg == 0x1F, "Error: Zero register access not handled");
    // Register branch encoding: 1101011000011111000000 + xn (5 bits) + 00000
    // 1101 0110 0001 1111 0000 00[xn5][xn4] [xn3][xn2][xn1]0 0000
    return (0xD61F0 << 12) | (xn_reg << 5); 
}

uint32_t encode_conditional_branch(int64_t offset, const char *mnemonic) {
    int64_t simm19_val = offset / 4;
    uint32_t cond = get_condition_code(mnemonic);
    // Conditional branch follows encoding: 0101 0100 [simm19]0 [cond]
    return (0x54 << 24) | ((simm19_val & mask_bits(19)) << 5) | cond;
}

uint32_t assemble_branch_instruction(const char *mnemonic, const char **args, uint32_t current_address, uint32_t target_address) {
    branch_type_t branch_type = detect_branch_type(mnemonic);
    switch (branch_type) {
        case BRANCH_UNCONDITIONAL:
            // Offset is calculated from target - current.
            int64_t offset = (int64_t)target_address - (int64_t)current_address;
            return encode_unconditional_branch(offset);
        case BRANCH_REGISTER:
            // Parse operands[0] to get the reg num: e.g. "x5" = 5 
            uint32_t reg_num = args[0] + 1;
            return encode_register_branch(reg_num);
        case BRANCH_CONDITIONAL:
            uint32_t offset = (int64_t)target_address - (int64_t)current_address;
            return encode_conditional_branch(offset, mnemonic);
    }
}
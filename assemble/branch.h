#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
/**
 * Translate branch instructions into an assembly encoding
 * @param mnemonic The branch instruction type
 * @param operands The operands of the branch instruction (literal, register number.)
 * @param current_address The current address of the PC
 * @param target_address The target address for the branch instruction
 */
uint32_t assemble_branch_instruction(const char *mnemonic, const char **operands, uint32_t current_address, uint32_t target_address);

#endif
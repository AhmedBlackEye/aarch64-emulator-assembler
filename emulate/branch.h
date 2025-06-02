#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
/**
 * Executes a branch instruction based on the instruction encoding.
 *
 * @param instruction The 32-bit instruction to execute
 * @return 0 on success, or terminates program on error
 */
int execute_branch_instruction(uint32_t instruction);

#endif // BRANCH_H

#ifndef HANDLE_INSTR_H
#define HANDLE_INSTR_H

#include <stdint.h>

/**
 * @brief Decodes a 32-bit instruction using its 4-bit op0 field and executes the instruction.
 */
void handle_instr(uint32_t instr);

#endif

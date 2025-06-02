#ifndef DP_IMMEDIATE_H
#define DP_IMMEDIATE_H

#include <stdint.h>

/**
 * @brief Decodes the instruction and dispatches to either arithmetic or wide move
 * execution functions.
 */
void handle_dp_imm(uint32_t instr);

#endif

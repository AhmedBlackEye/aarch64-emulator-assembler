#ifndef DP_REG_H
#define DP_REG_H

#include <stdint.h>

/**
 * Handles Data Processing Register instructions
 * @param instruction The 32-bit ARM instruction to decode and handle.
 */
extern void handle_dp_reg(uint32_t instr);

#endif // DP_REG_H
#ifndef DP_SHARED_H
#define DP_SHARED_H

#include <stdint.h>
#include <stdbool.h>

#define OPC_ADD 0x0
#define OPC_ADDS 0x1
#define OPC_SUB 0x2
#define OPC_SUBS 0x3

/**
 * @brief Updates processor status flags (NZCV) after an arithmetic operation.
 *
 * This function sets the Negative (N), Zero (Z), Carry (C), and Overflow (V)
 * flags in the processor state based on the operands and result of an
 * addition or subtraction operation.
 */
extern void update_arith_flags(uint64_t opr1, uint64_t opr2, uint64_t result, bool is_addition, bool is64);

/**
 * @brief Computes the result of an arithmetic operation (add/sub/adds/subs).
 *
 * If the operation sets flags (ADDS/SUBS), the condition flags are updated.
 */
extern uint64_t compute_arithmetic(uint64_t Rn, uint64_t Op2, uint8_t opc, bool is64);

#endif // DP_SHARED_H

#ifndef DP_SHARED_H
#define DP_SHARED_H

#include <stdint.h>
#include <stdbool.h>

#define OPC_ADD 0b00
#define OPC_ADDS 0b01
#define OPC_SUB 0b10
#define OPC_SUBS 0b11

extern void update_arith_flags(uint64_t opr1, uint64_t opr2, uint64_t result, bool is_addition, bool is64);
extern uint64_t compute_arithmetic(uint64_t Rn, uint64_t Op2, uint8_t opc, bool is64);

#endif // DP_SHARED_H
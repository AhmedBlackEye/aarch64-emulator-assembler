#include <stdint.h>
#include <stdlib.h>

#include "handle_instr.h"
#include "debug.h"
#include "dp_reg.h"
#include "load_store.h"
#include "branch.h"
#include "dp_immediate.h"

// Mask
#define MASK_4_BITS 0xF

// Bit Position
#define OP0_POS 25

// OP0 Values
#define OP0_DP_IMM_1 0x8
#define OP0_DP_IMM_2 0x9
#define OP0_DP_REG_1 0x5
#define OP0_DP_REG_2 0xD
#define OP0_LOAD_1 0x4
#define OP0_LOAD_2 0x6
#define OP0_LOAD_3 0xC
#define OP0_LOAD_4 0xE
#define OP0_BRANCH_1 0xA
#define OP0_BRANCH_2 0xB

void handle_instr(uint32_t instr)
{
    uint32_t op0 = (instr >> OP0_POS) & MASK_4_BITS;

    switch (op0)
    {
    case OP0_DP_IMM_1:
    case OP0_DP_IMM_2:
        handle_dp_imm(instr);
        break;

    case OP0_DP_REG_1:
    case OP0_DP_REG_2:
        handle_dp_reg(instr);
        break;

    case OP0_LOAD_1:
    case OP0_LOAD_2:
    case OP0_LOAD_3:
    case OP0_LOAD_4:
        handle_load_store(instr);
        break;

    case OP0_BRANCH_1:
    case OP0_BRANCH_2:
        execute_branch_instruction(instr);
        break;

    default:
        PANIC("Unknown op0 value: 0x%x\n", op0);
    }
}

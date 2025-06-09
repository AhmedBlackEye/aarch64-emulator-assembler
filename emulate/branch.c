#include <stdint.h>
#include <stdbool.h>

#include "branch.h"
#include "global.h"
#include "debug.h"
#include "shared.h"

// Branch instruction type constants
#define UNCONDITIONAL 0x05
#define REGISTER 0x35
#define CONDITIONAL 0x15

#define COND_EQ 0x0
#define COND_NE 0x1
#define COND_GE 0xA
#define COND_LT 0xB
#define COND_GT 0xC
#define COND_LE 0xD
#define COND_AL 0xE

// Declarations for static functions
static int handle_unconditional_branch(uint32_t instruction);
static int handle_register_branch(uint32_t instruction);
static int handle_conditional_branch(uint32_t instruction);
static bool check_condition(uint32_t cond);

int execute_branch_instruction(uint32_t instruction)
{
    uint32_t op0 = extract_bits(instruction, 26, 31);

    switch (op0)
    {
    case UNCONDITIONAL:
        return handle_unconditional_branch(instruction);
    case REGISTER:
        return handle_register_branch(instruction);
    case CONDITIONAL:
        return handle_conditional_branch(instruction);
    default:
        PANIC("Unknown branch instruction format with op0=0x%02X", op0);
    }
}

static int handle_unconditional_branch(uint32_t instruction)
{
    uint32_t simm26 = extract_bits(instruction, 0, 25);
    int64_t offset = sign_extend(simm26, 26) * 4;

    // Update PC =  PC + offset
    pc = (pc - INSTR_SIZE) + offset;
    return 0;
}

static int handle_register_branch(uint32_t instruction)
{
    uint32_t xn = extract_bits(instruction, 5, 9);

    PANIC_IF(xn == 31, "Branch to zero register not supported");

    // Update PC = Xn
    pc = read_reg(xn, true);
    return 0;
}

static int handle_conditional_branch(uint32_t instruction)
{
    uint32_t cond = extract_bits(instruction, 0, 3);
    uint32_t simm19 = extract_bits(instruction, 5, 23);

    bool condition_met = check_condition(cond);

    if (condition_met)
    {
        int64_t offset = sign_extend(simm19, 19) * 4;
        // Update PC = PC + offset
        pc = (pc - INSTR_SIZE) + offset;
    }

    return 0;
}

static bool check_condition(uint32_t cond)
{
    switch (cond)
    {
    case COND_EQ:
        return pstate.Z;
    case COND_NE:
        return !pstate.Z;
    case COND_GE:
        return pstate.N == pstate.V;
    case COND_LT:
        return pstate.N != pstate.V;
    case COND_GT:
        return (!pstate.Z && (pstate.N == pstate.V));
    case COND_LE:
        return !(!pstate.Z && (pstate.N == pstate.V));
    case COND_AL:
        return true;
    default:
        PANIC("Unsupported condition code: 0x%X", cond);
    }
}

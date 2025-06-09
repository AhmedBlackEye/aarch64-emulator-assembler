#include <stdint.h>
#include <stdbool.h>

#include "dp_reg.h"
#include "debug.h"
#include "global.h"
#include "dp_shared.h"
#include "shared.h"

#define MASK_32_BITS 0xFFFFFFFFULL
#define MASK_64_BITS ~0ULL

#define LSL_MASK 0x0
#define LSR_MASK 0x1
#define ASR_MASK 0x2
#define ROR_MASK 0x3

#define OPC_AND_BIC 0x0
#define OPC_ORR_ORN 0x1
#define OPC_EOR_EON 0x2
#define OPC_ANDS_BICS 0x3

#define OP_FIELD_MULTIPLY 0x8

static uint64_t shift_reg(uint64_t val, uint8_t amount, uint8_t shift_type, bool is64);
static uint64_t execute_multiply(uint64_t Rn, uint64_t Rm, uint64_t Ra, bool negate, bool is64);

static uint64_t shift_reg(uint64_t val, uint8_t amount, uint8_t shift_type, bool is64)
{
    const uint64_t n = is64 ? 64 : 32;
    const uint64_t mask = is64 ? MASK_64_BITS : MASK_32_BITS;

    amount &= (n - 1);
    val &= mask;

    uint64_t res;
    switch (shift_type)
    {
    case LSL_MASK:
        res = val << amount;
        return res & mask;
    case LSR_MASK:
        res = val >> amount;
        return res & mask;
    case ASR_MASK:
        res = (is64 ? (int64_t)val : (int32_t)val) >> amount;
        return res & mask;
    case ROR_MASK:
        res = (val >> amount) | (val << ((n - amount) & (n - 1)));
        return res & mask;
    default:
        DEV_ASSERT(false, "Invalid shift type %d", shift_type);
        return 0;
    }
}

static uint64_t execute_multiply(uint64_t Rn, uint64_t Rm, uint64_t Ra, bool negate, bool is64)
{
    uint64_t product = Rn * Rm;
    uint64_t result = negate ? (Ra - product) : (Ra + product);
    return is64 ? result : (uint32_t)result;
}

static uint64_t execute_logical(uint64_t Rn, uint64_t Op2, uint8_t opc, bool is64)
{
    switch (opc)
    {
    case OPC_AND_BIC:
        return Rn & Op2;
    case OPC_ORR_ORN:
        return Rn | Op2;
    case OPC_EOR_EON:
        return Rn ^ Op2;
    case OPC_ANDS_BICS:
    {
        uint64_t res = Rn & Op2;

        // update flags
        pstate.N = (res >> (is64 ? 63 : 31)) & 1;
        pstate.Z = (res == 0);
        pstate.C = 0;
        pstate.V = 0;
        return res;
    }
    default:
        DEV_ASSERT(false, "Invalid logical opc: %d", opc);
        return 0;
    }
}

void handle_dp_reg(uint32_t instr)
{
    uint8_t is64 = extract_bits(instr, 31, 31);
    uint8_t opc = extract_bits(instr, 29, 30);
    uint8_t M = extract_bits(instr, 28, 28);
    uint8_t op_field = extract_bits(instr, 21, 24);
    uint8_t N_neg_op2_flag = extract_bits(instr, 21, 21);
    uint8_t shift = extract_bits(instr, 22, 23);
    uint8_t imm6 = extract_bits(instr, 10, 15);
    uint8_t rm = extract_bits(instr, 16, 20);
    uint8_t ra = extract_bits(instr, 10, 14);
    uint8_t rn = extract_bits(instr, 5, 9);
    uint8_t rd = extract_bits(instr, 0, 4);

    uint64_t Rn = read_reg(rn, is64);
    uint64_t Rm = read_reg(rm, is64);
    uint64_t Ra = read_reg(ra, is64);
    if (!is64)
        Ra = (uint32_t)Ra;

    bool is_arith = !M && !N_neg_op2_flag && extract_bits(instr, 24, 24);
    bool is_logic = !M && !extract_bits(instr, 24, 24);
    bool is_multiply = M && op_field == OP_FIELD_MULTIPLY;

    PANIC_IF(!is_arith && !is_logic && !is_multiply,
             "Invalid instruction: M=%u, op_field=0x%X, instr=0x%08X\n",
             M, op_field, instr);

    uint64_t result;

    if (is_multiply)
    {
        bool negate_product = extract_bits(instr, 15, 15);
        result = execute_multiply(Rn, Rm, Ra, negate_product, is64);
        write_reg(rd, result, is64);
        return;
    }

    uint64_t Op2 = shift_reg(Rm, imm6, shift, is64);
    if (N_neg_op2_flag)
    {
        Op2 = is64 ? ~Op2 : ~((uint32_t)Op2);
    }

    if (is_arith)
        result = compute_arithmetic(Rn, Op2, opc, is64);
    else
        result = execute_logical(Rn, Op2, opc, is64);

    if (!is64)
        result = (uint32_t)result;
    write_reg(rd, result, is64);
}
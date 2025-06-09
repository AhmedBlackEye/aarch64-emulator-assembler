#include <stdint.h>

#include "global.h"
#include "dp_immediate.h"
#include "dp_shared.h"
#include "shared.h"

// Wide Move Opcodes
#define OPC_MOVN 0x0
#define OPC_MOVZ 0x2
#define OPC_MOVK 0x3

// Type of Operation
#define OPI_ARITH 0x2
#define OPI_WMOVE 0x5

static void execute_arithmetic(uint32_t instr);
static void execute_wmove(uint32_t instr);

void handle_dp_imm(uint32_t instr)
{
    uint8_t opi = extract_bits(instr, 23, 25);

    switch (opi)
    {
    case OPI_ARITH:
        execute_arithmetic(instr);
        break;
    case OPI_WMOVE:
        execute_wmove(instr);
        break;
    }
}

static void execute_arithmetic(uint32_t instr)
{
    uint8_t is64 = extract_bits(instr, 31, 31);
    uint8_t sh = extract_bits(instr, 22, 22);
    uint16_t imm12 = extract_bits(instr, 10, 21);
    uint8_t rn = extract_bits(instr, 5, 9);
    uint8_t rd = extract_bits(instr, 0, 4);

    uint64_t op1 = read_reg(rn, is64);
    uint64_t op2 = (sh == 1) ? (((uint64_t)imm12) << 12) : imm12;

    uint8_t opc = extract_bits(instr, 29, 30);
    uint64_t result = compute_arithmetic(op1, op2, opc, is64);
    write_reg(rd, result, is64);
}

static void execute_wmove(uint32_t instr)
{
    uint8_t sf = extract_bits(instr, 31, 31);
    uint8_t opc = extract_bits(instr, 29, 30);
    uint8_t hw = extract_bits(instr, 21, 22);
    uint16_t imm16 = extract_bits(instr, 5, 20);
    uint8_t rd = extract_bits(instr, 0, 4);

    uint64_t result_or_op;
    switch (opc)
    {
    case OPC_MOVN:
        result_or_op = ~((uint64_t)imm16 << (hw * 16));
        break;
    case OPC_MOVZ:
        result_or_op = (uint64_t)imm16 << (hw * 16);
        break;
    case OPC_MOVK:
    {
        uint64_t zero_mask = ~((uint64_t)0xFFFF << (hw * 16));
        uint64_t register_contents = read_reg(rd, sf);
        uint64_t insert_value = ((uint64_t)imm16) << (hw * 16);
        result_or_op = (register_contents & zero_mask) | insert_value;
        break;
    }
    }
    write_reg(rd, result_or_op, sf);
}

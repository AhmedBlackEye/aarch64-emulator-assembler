#include <stdint.h>
#include "global.h"
#include "debug.h"

static void handle_load_literal(uint32_t instruction)
{
    bool sf = extract_bits(instruction, 30, 30);
    uint8_t rt = extract_bits(instruction, 0, 4);
    int32_t simm19 = extract_bits(instruction, 5, 23);

    int64_t offset = sign_extend(simm19, 19) * 4;
    uint64_t address = (pc - INSTR_SIZE) + offset;

    write_reg(rt, read_mem(address, sf), sf);
}

static void handle_single_data_transfer(uint32_t instruction)
{
    bool sf = extract_bits(instruction, 30, 30);
    bool u = extract_bits(instruction, 24, 24);
    bool l = extract_bits(instruction, 22, 22);
    uint8_t xn = extract_bits(instruction, 5, 9);
    uint8_t rt = extract_bits(instruction, 0, 4);
    uint64_t base_addr = read_reg(xn, true);

    uint64_t address;
    // Unsigned Immediate Offset
    if (u)
    {
        uint32_t imm12 = extract_bits(instruction, 10, 21);
        uint64_t uoffset = imm12 * (sf ? 8 : 4);
        address = base_addr + uoffset;
    }
    else
    {
        bool isRegOffset = extract_bits(instruction, 21, 21);
        bool I = extract_bits(instruction, 11, 11);
        int64_t simm9 = sign_extend(extract_bits(instruction, 12, 20), 9);
        PANIC_IF(
            !isRegOffset && (simm9 < -256 || simm9 > 255),
            "simm9 value is out of range");

        if (I && !isRegOffset)
        { // Pre-Indexed
            address = base_addr + simm9;
            write_reg(xn, address, true);
        }
        else if (!I && !isRegOffset)
        { // Post-Indexed
            address = base_addr;
            write_reg(xn, base_addr + simm9, true);
        }
        else
        { // Register Offset
            int8_t xm = extract_bits(instruction, 16, 20);
            address = base_addr + read_reg(xm, true);
        }
    }

    if (l)
    { // Load
        write_reg(rt, read_mem(address, sf), sf);
    }
    else
    { // Store
        write_mem(address, read_reg(rt, sf), sf);
    }
}

void choose_sdt_or_ll(uint32_t instruction)
{
    bool sdt_or_ll = extract_bits(instruction, 31, 31);
    if (sdt_or_ll)
    {
        handle_single_data_transfer(instruction);
    }
    else
    {
        handle_load_literal(instruction);
    }
}

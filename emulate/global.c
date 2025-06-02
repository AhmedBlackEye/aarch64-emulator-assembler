#include <stdint.h>
#include <stdbool.h>

#include "global.h"
#include "debug.h"

static uint32_t read_mem32(uint32_t addr);
static uint64_t read_mem64(uint32_t addr);
static void write_mem32(uint32_t addr, uint32_t value);
static void write_mem64(uint32_t addr, uint64_t value);

static uint64_t xregs[NUM_REG] = {0};

inline uint32_t extract_bits(uint32_t instruction, int start, int end)
{
    int width = end - start + 1;
    uint32_t mask = (1U << width) - 1;
    return (instruction >> start) & mask;
}

uint64_t sign_extend(uint32_t value, int bits)
{
    DEV_ASSERT(bits > 0 && bits <= 32, "Invalid bit width for sign extension: %d", bits);

    uint32_t sign_bit = 1U << (bits - 1);
    if (value & sign_bit)
    {
        uint64_t mask = ~((1ULL << bits) - 1);
        return value | mask;
    }
    else
    {
        return value;
    }
}

void write_reg(uint8_t reg_num, uint64_t value, bool is64)
{
    DEV_ASSERT(reg_num < NUM_REG, "Invalid register reg_num=%d", reg_num);
    if (reg_num == ZERO_REG)
        return;
    xregs[reg_num] = is64 ? value : (uint32_t)value;
}

uint64_t read_reg(uint8_t reg_num, bool is64)
{
    DEV_ASSERT(reg_num < NUM_REG, "Invalid register reg_num=%d", reg_num);
    return is64 ? xregs[reg_num] : (uint32_t)xregs[reg_num];
}

uint64_t pc = 0;

PState pstate = {.N = false, .Z = true, .C = false, .V = false};

uint8_t memory[MAX_MEMORY_SIZE] = {0};

static uint32_t read_mem32(uint32_t addr)
{
    PANIC_IF(addr > MAX_MEMORY_SIZE - sizeof(uint32_t), "Memory read out of bounds");
    return (uint32_t)memory[addr] |
           ((uint32_t)memory[addr + 1] << 8) |
           ((uint32_t)memory[addr + 2] << 16) |
           ((uint32_t)memory[addr + 3] << 24);
}

static uint64_t read_mem64(uint32_t addr)
{
    PANIC_IF(addr > MAX_MEMORY_SIZE - sizeof(uint64_t), "Memory read out of bounds");
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        result |= ((uint64_t)memory[addr + i]) << (i * 8);
    }
    return result;
}

uint64_t read_mem(uint32_t addr, bool is64)
{
    return is64 ? read_mem64(addr) : read_mem32(addr);
}

static void write_mem32(uint32_t addr, uint32_t value)
{
    PANIC_IF(addr > MAX_MEMORY_SIZE - sizeof(uint32_t), "Memory write out of bounds");
    memory[addr] = (uint8_t)(value & 0xFF);
    memory[addr + 1] = (uint8_t)((value >> 8) & 0xFF);
    memory[addr + 2] = (uint8_t)((value >> 16) & 0xFF);
    memory[addr + 3] = (uint8_t)((value >> 24) & 0xFF);
}

static void write_mem64(uint32_t addr, uint64_t value)
{
    PANIC_IF(addr > MAX_MEMORY_SIZE - sizeof(uint64_t), "Memory write out of bounds");
    for (int i = 0; i < 8; ++i)
    {
        memory[addr + i] = (uint8_t)((value >> (i * 8)) & 0xFF);
    }
}

void write_mem(uint32_t addr, uint64_t value, bool is64)
{
    is64 ? write_mem64(addr, value) : write_mem32(addr, value);
}

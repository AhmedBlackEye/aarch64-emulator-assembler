#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

// Constants
#define HALT_INSTR 0x8A000000
#define NUM_REG 32
#define MAX_MEMORY_SIZE (2 << 20)
#define INSTR_SIZE 4
#define ZERO_REG 31

extern uint32_t extract_bits(uint32_t instruction, int start, int end);
extern uint64_t sign_extend(uint32_t value, int bits);
extern uint64_t read_reg(uint8_t reg_num, bool is64);
extern void write_reg(uint8_t reg_num, uint64_t value, bool is64);

extern void step_pc();

// Program Counter
extern uint64_t pc;
extern uint8_t memory[MAX_MEMORY_SIZE];
/**
 * Reads a 32-bit or 64-bit little-endian value from memory at the given address.
 * Panics if the access would exceed memory bounds.
 */
extern uint64_t read_mem(uint32_t addr, bool is64);

/**
 * Writes a 32-bit and 64-bit little-endian values to memory at the given address.
 * Panics if the access would exceed memory bounds.
 */
extern void write_mem(uint32_t addr, uint64_t value, bool is64);

typedef struct
{
    bool N; // Negative flag
    bool Z; // Zero flag
    bool C; // Carry flag
    bool V; // Overflow flag
} PState;

extern PState pstate;

#endif

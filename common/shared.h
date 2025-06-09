#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

/// Extracts a range of bits from a 32-bit instruction.
static inline uint32_t extract_bits(uint32_t instruction, int start, int end) {
    int width = end - start + 1;
    uint32_t mask = (1U << width) - 1;
    return (instruction >> start) & mask;
}

/// @brief Sign-extends a value with a specified bit width to 64 bits.
/// @param value The value to sign-extend.
/// @param bits The original bit width of the value (must be 1 to 32).
/// @return The sign-extended 64-bit value.
extern uint64_t sign_extend(uint32_t value, int bits);

#endif

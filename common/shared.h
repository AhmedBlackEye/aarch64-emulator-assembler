// Included common definations
#include <stdint.h>
#include <common/debug.h>

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
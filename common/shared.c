#include <stdint.h>

#include "debug.h"

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


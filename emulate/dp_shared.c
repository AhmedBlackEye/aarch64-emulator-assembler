#include "dp_shared.h"
#include "debug.h"

#define MASK_32_BITS 0xFFFFFFFFULL

void update_arith_flags(uint64_t opr1, uint64_t opr2, uint64_t result, bool is_addition, bool is64)
{
    uint64_t n = 63;
    if (!is64)
    {
        opr1 &= MASK_32_BITS;
        opr2 &= MASK_32_BITS;
        result &= MASK_32_BITS;
    }

    pstate.N = (result >> (is64 ? 63 : 31)) & 1;
    pstate.Z = (result == 0);

    if (is_addition)
    {
        pstate.C = result < opr1;
        pstate.V = ((~(opr1 ^ opr2) & (opr1 ^ result)) >> n) & 1;
    }
    else
    {
        pstate.C = opr1 >= opr2;
        pstate.V = (((opr1 ^ opr2) & (opr1 ^ result)) >> n) & 1;
    }
}

uint64_t compute_arithmetic(uint64_t Rn, uint64_t Op2, uint8_t opc, bool is64)
{
    uint64_t res;
    switch (opc)
    {
    case OPC_ADD:
        return Rn + Op2;
    case OPC_ADDS:
        res = Rn + Op2;
        update_arith_flags(Rn, Op2, res, true, is64);
        return res;
    case OPC_SUB:
        return Rn - Op2;
    case OPC_SUBS:
        res = Rn - Op2;
        update_arith_flags(Rn, Op2, res, false, is64);
        return res;
    default:
        DEV_ASSERT(false, "Invalid arithmetic opc: %d", opc);
        return 0;
    }
}
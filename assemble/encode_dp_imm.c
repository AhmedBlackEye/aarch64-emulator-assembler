#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "encode_dp_imm.h"
#include "global.h"
#include "debug.h"
#include "encode_dp_reg.h"
#include "global.h"

static uint32_t encode_wmove(const char **tokens,
    int size,
    uint8_t  opc
);

uint32_t encode_movn(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 0);
}

uint32_t encode_movz(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 2);
}

uint32_t encode_movk(const char ** tokens, int size) {
    return encode_wmove(tokens, size, 3);
}

uint32_t encode_arith_imm(const char **tokens,
    int size,
    uint8_t  opc,
    INSTRUCTION_TYPE type
) {
    PANIC_IF(!(size == 3 || size == 5 || size == 2 || size == 4),
        "encode_arith_imm: unexpected number of tokens: %d", size);

    uint8_t sh = 0;
    if ((size == 5 && STR_EQUAL(tokens[4], "#12")) || 
        (size == 4 && STR_EQUAL(tokens[3], "#12"))) {
        sh = 1;
    }

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint16_t imm_12 = (uint16_t) strtol(tokens[(type == STANDARD) ? 2 : 1] + 1, NULL, 0);

    uint8_t rd, rn;
    if (type == RD_ZR) {
        rd = 31;
        rn = parse_reg(tokens[0]);
    } else if (type == RN_ZR) {
        rd = parse_reg(tokens[0]);
        rn = 31;
    } else {
        rd = parse_reg(tokens[0]);
        rn = parse_reg(tokens[1]);
    }

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0x4 << 26;
    instr |= 0x2 << 23; 
    instr |= sh << 22;
    instr |= imm_12 << 10;
    instr |= rn << 5;
    instr |= rd;
    return instr;
}

static uint32_t encode_wmove(const char **tokens,
    int size,
    uint8_t  opc
) {
    PANIC_IF(!(size == 2 || size == 4),
        "encode_wmove: unexpected number of tokens: %d", size);

    uint8_t hw = 0;
    if (size == 4) {
        hw = (uint8_t)(strtol(tokens[3] + 1, NULL, 0) / 16);
    }

    uint8_t sf = (tokens[0][0] == 'x') ? 1 : 0;

    uint16_t imm_16 = (uint16_t) strtol(tokens[1] + 1, NULL, 0);

    uint32_t instr = 0;
    instr |= sf << 31;
    instr |= opc << 29;
    instr |= 0x4 << 26;
    instr |= 0x5 << 23;
    instr |= hw << 21;
    instr |= imm_16 << 5;
    instr |= parse_reg(tokens[0]);
    return instr;
}

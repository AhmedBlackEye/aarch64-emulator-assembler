#include "encode_dp_imm.h"
#include "encode_dp_reg.h"

uint32_t encode_add(const char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 0);
    }
    else {
        return encode_arith_reg(tokens, size, 0);
    }
}

uint32_t encode_adds(const char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 1);
    }
    else {
        return encode_arith_reg(tokens, size, 1);
    }
}

uint32_t encode_sub(const char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 2);
    }
    else {
        return encode_arith_reg(tokens, size, 2);
    }
}

uint32_t encode_subs(const char **tokens, int size) {
    if (tokens[2][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}

uint32_t encode_cmp(const char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}

uint32_t encode_cmn(const char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 1);
    }
    else {
        return encode_arith_reg(tokens, size, 1);
    }
}

uint32_t encode_neg(const char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 2);
    }
    else {
        return encode_arith_reg(tokens, size, 2);
    }
}

uint32_t encode_negs(const char **tokens, int size) {
    if (tokens[1][0] == '#') {
        return encode_arith_imm(tokens, size, 3);
    }
    else {
        return encode_arith_reg(tokens, size, 3);
    }
}
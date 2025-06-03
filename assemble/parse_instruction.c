#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "encoder_table.h"

#define MAX_TOKENS 5

uint32_t parse_instr(char *line) {

    char *saveptr = NULL;
    char *mnemonic = strtok_r(line, " ", &saveptr);
    char *tokens[MAX_TOKENS];
    int  total = 0;
    char *t = strtok_r(NULL, " ", &saveptr);
    while (t != NULL) {
        tokens[total++] = t;
        t = strtok_r(NULL, " ", &saveptr);
    }
    encode_function fn = lookup_encoder(mnemonic);
    return fn(tokens, total);
}
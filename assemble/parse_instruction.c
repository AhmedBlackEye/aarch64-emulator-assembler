#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "encoder_table.h"

#define MAX_TOKENS 5

uint32_t parse_instr(char *line) {

    char *saveptr = NULL;
    char *mnemonic = strtok_r(line, ", ", &saveptr);
    PANIC_IF(mnemonic == NULL,"parse_instr: no mnemonic read");
    char *tokens[MAX_TOKENS];
    int  num_tokens = 0;
    char *t = strtok_r(NULL, ", ", &saveptr);
    while (t != NULL) {
        tokens[num_tokens] = t;
        num_tokens++;
        t = strtok_r(NULL, ", ", &saveptr);
    }
    PANIC_IF( num_tokens < 2, "parse_instr: only 1 token but at least 2 are needed, the token is %s", tokens[0]);
    encode_function fn = lookup_encoder(mnemonic);
    return fn(tokens, num_tokens);
}
#include <stdint.h>
#include <string.h> 
#include <stdlib.h>

uint64_t curr_instr_addr = 0;

uint8_t parse_reg(const char *reg) {
    return (strcmp(reg, "xzr") == 0 || strcmp(reg, "wzr") == 0 ? 31 : strtol(reg + 1, NULL, 0));
}
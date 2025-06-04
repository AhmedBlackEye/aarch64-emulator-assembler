#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "debug.h"
#include "sym_table.h"
#include "global.h"
#include "shared.h"

static uint32_t encode_load_literal(const char *args[], int size) {
    DEV_ASSERT(size == 2, "Expected arguements should be 2, but got %d", size);
    
    bool sf = (args[0][0] == 'x');
    uint8_t rt = atoi(args[0] + 1);
    uint32_t target_address;
    if (args[1][0] == '#') {
        target_address = strtol(args[1] + 1, NULL, 0);
    } else {
        target_address = symtab_lookup(args[1]);
    }

    int32_t offset = (target_address - curr_instr_addr) / 4;

    PANIC_IF(offset < -262144 || offset > 262143, "offset out of range");

    uint32_t instr = 0;
    instr |= sf << 30;
    instr |= 0x18 << 24;
    instr |= extract_bits(offset, 0, 18) << 5;
    instr |= rt << 0;

    return instr;
}

static uint32_t encode_unsigned_offset(const char *args[], int size, bool is_load) {
    DEV_ASSERT(size == 2, "Expected 2 tokens, got %d", size);
    
    bool sf = (args[0][0] == 'x');
    bool U = 1;
    uint8_t rt = atoi(args[0] + 1);
    
    uint8_t xn;
    int tmp_xn = -1; 
    int imm = 0;
    if (sscanf(args[1], "[x%d, #%d]", &tmp_xn, &imm) == 2) {
        xn = (uint8_t)tmp_xn;
    } else if (sscanf(args[1], "[x%d]", &tmp_xn) == 1) {
        xn = (uint8_t)tmp_xn;
    } else {
        PANIC("Parsing failed in Unsigned offset");
    } 

    int scale = sf ? 8 : 4;
    int16_t imm12 = imm / scale;
       
    uint32_t instr = 0;
    instr |= 0x1 << 31;
    instr |= sf << 30;
    instr |= 0x1C << 25;
    instr |= U << 24;
    instr |= 0 << 23;
    instr |= is_load << 22;
    instr |= imm12 << 10;
    instr |= xn << 5;
    instr |= rt << 0;

    return instr;
}

static uint32_t encode_pre_index(const char *args[], int size, bool is_load) {
    DEV_ASSERT(size == 2, "Expected arguments should be 2, but got %d", size);
    
    uint32_t rt = atoi(args[0] + 1);
    bool sf = (args[0][0] == 'x');
    PANIC_IF(rt > 31, "Invalid target register number: %u", rt);
    
    uint8_t xn;
    int16_t simm9; 
    int tmp_xn, tmp_simm9;
    sscanf(args[1], "[x%d, #%d]!", &tmp_xn, &tmp_simm9);
    xn = (uint8_t)tmp_xn;
    simm9 = (int16_t)tmp_simm9;

    PANIC_IF(simm9 < -256 || simm9 > 255, "Pre-index immediate %d out of range [-256, 255]",
             simm9);
    PANIC_IF(xn > 31, "Invalid base register: %u", xn); 

    uint32_t instr = 0;
    instr |= rt << 0;
    instr |= xn << 5;
    instr |= 0x3 << 10;
    instr |= simm9 << 12;
    instr |= is_load << 22;
    instr |= 0x7 << 27;
    instr |= sf << 30;
    instr |= 1 << 31;
    return instr;
}

static uint32_t encode_post_index(const char *args[], int size, bool is_load) {
    DEV_ASSERT(size == 3, "Expected arguments should be 3, but got %d", size);

    uint32_t rt = atoi(args[0] + 1);
    bool sf = (args[0][0] == 'x');
    PANIC_IF(rt > 31, "Invalid target register number: %u", rt);

    uint8_t xn;
    int tmp_xn;
    int16_t simm9 = atoi(args[2] + 1);
    sscanf(args[1], "[%d]", &tmp_xn);
    xn = (uint8_t)tmp_xn;

    PANIC_IF(simm9 < -256 || simm9 > 255, "Pre-index immediate %d out of range [-256, 255]",
             simm9);
    PANIC_IF(xn > 31, "Invalid base register: %u", xn);
    
    uint32_t instr = 0;
    instr |= rt << 0;
    instr |= xn << 5;
    instr |= 1 << 10;
    instr |= simm9 << 12;
    instr |= is_load << 22;
    instr |= 0x7 << 27;
    instr |= sf << 30;
    instr |= 1 << 31;
    return instr;
}

static uint32_t encode_register_offset(const char *args[], int size, bool is_load) {
    DEV_ASSERT(size == 2, "Expected arguements should be 2, but got %d", size);

    uint32_t rt = atoi(args[0] + 1);
    bool sf = (args[0][0] == 'x');        
    
    uint8_t xn;
    uint8_t xm;
    int tmp_xn, tmp_xm;
    sscanf(args[1], "[%*c%d, %*c%d]", &tmp_xn, &tmp_xm);
    xn = (uint8_t)tmp_xn;
    xm = (uint8_t)tmp_xm;

    uint32_t instr = 0;
    instr |= rt << 0;
    instr |= xn << 5;
    instr |= 0x1A << 10;
    instr |= xm << 16;
    instr |= 1 << 21;
    instr |= is_load << 22;
    instr |= 0x7 << 27;
    instr |= sf << 30;
    instr |= 1 << 31;

    return instr;
}  

static uint32_t encode_load_store(const char *args[], int size, bool is_load) {
    DEV_ASSERT(size == 2 || size == 3, "Expected arguements should be 2 or 3, but got %d", size);
       
    if (is_load && args[1][0] != '[') { // Literal Load
        return encode_load_literal(args, size);
    } else if (strstr(args[1], "]!")) { // Pre-indexed
        return encode_pre_index(args, size, is_load);
    } else if (size == 3) { // Post-indexed
        return encode_post_index(args, size, is_load);
    } else if (strchr(args[1], ',') && !strchr(args[1], '#')) { // Register offset
        return encode_register_offset(args, size, is_load);
    } else if (args[1][0] == '[') { // Unsigned offset
        return encode_unsigned_offset(args, size, is_load);
    }

    PANIC("Invalid addressing mode: %s", args[1]);
    return 0;
}

uint32_t encode_load(const char *args[], int size) {
    return encode_load_store(args, size, true);
}

uint32_t encode_store(const char *args[], int size) {
    return encode_load_store(args, size, false);
}

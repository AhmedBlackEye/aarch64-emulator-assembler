#ifndef GLOBAL_H 
#define GLOBAL_H 

#include "string.h"
#include "stdint.h"

/**
 * Determines if two strings are equal to one another
 * 
 * @return true on success and false on error. 
 */
#define STR_EQUAL(str1, str2) (strcmp(str1, str2) == 0)


extern uint64_t curr_instr_addr;

#endif 
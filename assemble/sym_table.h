#include <stdint.h>

#include "debug.h"
#include "global.h"
#include "bintree.h"
#include "dynstr.h"

/* Add new symbol to table */
void symtab_define(char *label, uint64_t mem_addr);

/* Lookup symbol address by label */
uint64_t symtab_lookup(const char *label);

/* Optional cleanup function */
void symtab_cleanup(void);
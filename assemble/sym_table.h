#include <stdint.h>

typedef struct {
  char *label;
  uint64_t mem_addr;
} SymTabElem;


void symtab_define(char *label, uint64_t mem_addr);
uint64_t symtab_lookup(const char *label);
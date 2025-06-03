#include <stdint.h>


#include "debug.h"
#include "sym_table.h"


#define SYMBOL_TABLE_SIZE 1000


static SymTabElem sym_table[SYMBOL_TABLE_SIZE];
static size_t size = 0;

void symtab_define(char *label, uint64_t mem_addr) {
  PANIC_IF(size >= SYMBOL_TABLE_SIZE, "Symbol table is full. Unable to add new element");

  sym_table[size++] = (SymTabElem){label, mem_addr}; 
}

uint64_t symtab_lookup(char *label) {
  for(int i = 0; i < size; i++) {
    if(STR_EQUAL(sym_table[i].label, label)) return sym_table[i].mem_addr;
  }
  PANIC("Label=\"%s\" is not found.", label);
}
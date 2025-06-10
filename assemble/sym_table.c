#include <stdint.h>
#include <string.h>

#include "debug.h"
#include "sym_table.h"
#include "global.h"
#include "bintree.h"
#include "dynstr.h"

#include "bintree.h"
#include "debug.h"
#include "dynstr.h"

typedef struct SymTabElem
{
  DynStr label;      // Dynamic string for symbol label
  uint64_t mem_addr; // Memory address associated with symbol
} SymTabElem;

int symtabelem_cmp(GVal v1, GVal v2)
{
  SymTabElem *e1 = (SymTabElem *)v1;
  SymTabElem *e2 = (SymTabElem *)v2;

  return dynstr_cmp(e1->label, e2->label);
}

static void symtabelem_print(GVal val)
{
  SymTabElem *e = (SymTabElem *)val;
  printf("SymTabElem<%s|%lx>", dynstr_data(e->label), e->mem_addr);
}

static void symtabelem_free(GVal val)
{
  SymTabElem *e = (SymTabElem *)val;
  dynstr_free(e->label);
  free(e);
}

static BinTree sym_table = NULL;

static void symtab_init(void)
{
  if (!sym_table)
  {
    sym_table = create_tree(symtabelem_cmp, symtabelem_print, symtabelem_free);
    PANIC_IF(!sym_table, "Symbol table creation failed");
  }
}

void symtab_define(char *label, uint64_t mem_addr)
{
  symtab_init();

  SymTabElem *elem = malloc(sizeof(SymTabElem));
  PANIC_IF(!elem, "Memory allocation failed for symbol: %s", label);

  elem->label = dynstr_create((char *)label);
  PANIC_IF(!elem->label, "DynStr creation failed for: %s", label);

  elem->mem_addr = mem_addr;

  bintree_add(sym_table, elem);
}

uint64_t symtab_lookup(const char *label)
{
  PANIC_IF(!sym_table, "Symbol table not initialized");

  SymTabElem temp_key = {.label = dynstr_create((char *)label)};
  PANIC_IF(!temp_key.label, "DynStr creation failed for: %s", label);
  SymTabElem *found = bintree_lookup(sym_table, &temp_key);
  dynstr_free(temp_key.label);

  PANIC_IF(!found, "Label=\"%s\" not found", label);
  return found->mem_addr;
}

void symtab_cleanup(void)
{
  if (sym_table)
  {
    destroy_tree(sym_table);
    sym_table = NULL;
  }
}
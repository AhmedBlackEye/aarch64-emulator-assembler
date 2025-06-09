#include <stdint.h>
#include <stdlib.h>

#include "dynstr.h"

typedef struct Node *Node;

extern Node make_node(char *str);

typedef struct BinTreeStr *BinTreeStr;


extern BinTreeStr make_bintreestr();

extern void destroy_bintreestr(BinTreeStr t);

extern void bintreestr_add(BinTreeStr t, char *str);
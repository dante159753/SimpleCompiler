#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"

void insert_id(char* name, TokenType tokentype, void* id_val);

int get_id(char* name, ExprValue ** idvalue);

#endif

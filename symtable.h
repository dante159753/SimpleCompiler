#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"

void insert_id(char* name, TokenType tokentype, void* id_val);

int find_id(char* name);

int get_id(char* name, ExprValue ** idvalue);

void print_symbol_table();

#endif

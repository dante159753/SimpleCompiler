#include "globals.h"
#include "util.h"
#include "symtable.h"

#define TABLE_SIZE 313 /* size of the hash table */

#define SHIFT 4 /* used for hash function */

int hash(char * key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % TABLE_SIZE;
		++i;
	}
	return temp;
}

/* entry for symbol table */
typedef struct sym_entry
{
	char* name;
	ExprValue* value;
	struct sym_entry * next;
} SymbolEntry;

static SymbolEntry* hashTable[TABLE_SIZE];

void insert_id(char* name, TokenType tokentype, void* id_val)
{
	int key = hash(name);
	SymbolEntry* entry = hashTable[key];
	while((entry != NULL) && (strcmp(name, entry->name) != 0))
	{
		entry = entry->next;
	}
	if (entry == NULL)
	{
		entry = (SymbolEntry*)malloc(sizeof(SymbolEntry));
		entry->name = copy_str(name);
		ExprValue* idvalue = (ExprValue*) malloc(sizeof(ExprValue));
		if(tokentype == INTNUM)
		{
			idvalue->exprtype = INT_EXPR;
			idvalue->value.int_val = *(int*)id_val;
		}
		else
		{
			idvalue->exprtype = REAL_EXPR;
			idvalue->value.real_val = *(double*)id_val;
		}
		entry->value = idvalue;
		entry->next = hashTable[key];
		hashTable[key] = entry;
	}
	else
	{
		/* id has already in the table, do nothing */
	}
}

int get_id(char* name, ExprValue ** idvalue)
{
	int key = hash(name);
	SymbolEntry* entry = hashTable[key];
	while((entry != NULL) && (strcmp(name, entry->name) != 0))
	{
		entry = entry->next;
	}
	if(entry == NULL)
	{
		/* not found */
		return -1;
	}
	else
	{
		*idvalue = entry->value;
		return 1;
	}
}

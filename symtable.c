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

/**
 * @param name name of id.
 * @param tokentype tokentype of the tree node.
 * @param id_val pointer to the value of the id
 */
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

		if(tokentype == INT)
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

/**
 * Get id's value by name.
 * @param name Name to find.
 * @param idvalue Value struct of the id.
 * @return 1 means the given name is found, -1 means not found.
 */
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

/**
 * Determain wheather the name is in the symtable.
 * @param name Name to find.
 * @return 1 means the given name is found, -1 means not found.
 */
int find_id(char* name)
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
		return 1;
	}
}

/**
 * Pirnt the value of a symbol table entry.
 * @param entry The symbol table entry to print.
 */
void print_entry(SymbolEntry* entry)
{
	printf("%s: ", entry->name);
	if(entry->value->exprtype == INT_EXPR)
	{
		printf("%d\n", entry->value->value.int_val);
	}
	else
	{
		printf("%.3lf\n", entry->value->value.real_val);
	}
}

/**
 * Print current value of all entries in symbol table.
 */
void print_symbol_table()
{
	SymbolEntry* current;
	int i;
	for(i=0; i<TABLE_SIZE; i++)
	{
		current = hashTable[i];
		while(current != NULL)
		{
			print_entry(current);
			current = current->next;
		}
	}
}

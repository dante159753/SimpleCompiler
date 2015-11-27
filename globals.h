#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define MAX_CHILDREN 10

typedef enum
{
	/* flag token */
	AT_EOF,ERROR,
	/* reserved words */
	IF,THEN,ELSE,WHILE,INT,REAL,
	/* multicharacter tokens */
	ID,INTNUM,REALNUM,
	/* special symbols */
	ASSIGN,EQ,NEQ,LT,GT,LET,GET,PLUS,MINUS,TIMES,OVER,
	LPAREN,RPAREN,LBRACKET,RBRACKET,SEMI
} TokenType;

typedef enum
{
	PROG=1, DECLS, DECL, STMT, IFSTMT, ASSIGNSTMT, COMPOUNDSTMT, WHILESTMT, STMTS, BOOLEXPR, BOOLOP,
	ARITHEXPR, ARITHEXPR_P, MULTIEXPR, MULTIEXPR_P, SIMPLEEXPR
} NontermType;

typedef struct 
{
	TokenType type;
	union{
		char* error_str;/* for ERROR */
		char* name;/* for ID */
		int int_val;/* for INTNUM */
		double real_val;/* for REALNUM */
	}value;
	int lineno;
	int linepos;
} Token;

typedef enum
{
	TERMINAL,
	NONTERMINAL,
	ERRORNODE
} NodeType;

typedef struct treenode
{
	NodeType node_type;
	union{
		TokenType term;
		NontermType nonterm;
	}type;
	struct treenode* child[MAX_CHILDREN];
	struct treenode* sibling;
	int n_child;
	int lineno;
	int linepos;
	union{
		char* name;/* for ID */
		int int_val;/* for INTNUM */
		double real_val;/* for REALNUM */
	}value;
} TreeNode;

#endif

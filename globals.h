#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define MAX_CHILDREN 3

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

typedef struct treenode
{
	struct treenode* child[MAX_CHILDREN];
	struct treenode* sibling;
	int lineno;
	int linepos;
	union{
		char* name;/* for ID */
		int int_val;/* for INTNUM */
		double real_val;/* for REALNUM */
	}value;
} TreeNode;

#endif

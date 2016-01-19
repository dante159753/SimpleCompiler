#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define MAX_CHILDREN 10

/**
 * Indicate token type in lexical analysis.
 */
typedef enum
{
	ERROR, SEMI, INT, ID, ASSIGN, INTNUM, 
	REAL, REALNUM, IF, LPAREN, RPAREN, THEN, 
	ELSE, LBRACKET, RBRACKET, WHILE, LT, GT, 
	LET, GET, EQ, NEQ, PLUS, MINUS, 
	TIMES, OVER, AT_EOF
} TokenType;

/**
 * Indicate nonterminals in syntactical analysis;
 */
typedef enum
{
	PROG=1, DECLS, DECL, STMT, IFSTMT, ASSIGNSTMT, COMPOUNDSTMT, WHILESTMT, STMTS, BOOLEXPR, BOOLOP,
	ARITHEXPR, ARITHEXPR_P, MULTIEXPR, MULTIEXPR_P, SIMPLEEXPR
} NontermType;

/**
 * Contains concreate token infomation.
 */
typedef struct
{
	TokenType type;
	union{
		char* error_str;/*< for ERROR */
		char* name;/*< for ID */
		int int_val;/*< for INTNUM */
		double real_val;/*< for REALNUM */
	}value;
	int lineno;
	int linepos;
} Token;

/**
 * Type of syntax tree's node.
 */
typedef enum
{
	TERMINAL,
	NONTERMINAL,
	ERRORNODE
} NodeType;

/**
 * Node of syntax tree.
 */
typedef struct treenode
{
	NodeType node_type;
	union{
		TokenType term;
		NontermType nonterm;
	}type;
	struct treenode* child[MAX_CHILDREN];
	struct treenode* sibling;
	int n_child;/*< Number of children. */
	int lineno;
	int linepos;
	int leng;/*< If this node is length of token. */
	union{
		char* name;/* for ID */
		int int_val;/* for INTNUM */
		double real_val;/* for REALNUM */
	}value;
} TreeNode;

/**
 * Expression's type used in executing.
 */
typedef enum
{
	INT_EXPR,
	REAL_EXPR
} ExprType;

/**
 * Expression's value used in executing.
 */
typedef struct
{
	ExprType exprtype;
	union{
		int int_val;
		double real_val;
	}value;
} ExprValue;

#endif

#include "util.h"

char * copy_str(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    printf("Out of memory error");
  else strcpy(t,s);
  return t;
}

TokenType get_token_bystr(char* s)
{
	if (strlen(s) == 1){
		switch(s[0]){
			case '+':
				return PLUS;
				break;
			case '-':
				return MINUS;
				break;
			case '*':
				return TIMES;
				break;
			case '/':
				return OVER;
				break;
			case '>':
				return GT;
				break;
			case '<':
				return LT;
				break;
			case '=':
				return ASSIGN;
				break;
			case '{':
				return LBRACKET;
				break;
			case '}':
				return RBRACKET;
				break;
			case '(':
				return LPAREN;
				break;
			case ')':
				return RPAREN;
				break;
			case ';':
				return SEMI;
				break;
			default:
				return ERROR;
		}
	}
	else {
		if (strcmp(s, "if") == 0){
			return IF;
		}
		else if (strcmp(s, "then") == 0){
			return THEN;
		}
		else if (strcmp(s, "else") == 0){
			return ELSE;
		}
		else if (strcmp(s, "while") == 0){
			return WHILE;
		}
		else if (strcmp(s, "int") == 0){
			return INT;
		}
		else if (strcmp(s, "real") == 0){
			return REAL;
		}
		else if (strcmp(s, ">=") == 0){
			return GET;
		}
		else if (strcmp(s, "<=") == 0){
			return LET;
		}
		else if (strcmp(s, "!=") == 0){
			return NEQ;
		}
		else if (strcmp(s, "==") == 0){
			return EQ;
		}
		else {
			return ERROR;
		}
	}
}

TreeNode* create_node(){
	TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
	if (t == NULL){
		printf("error, out of memory!\n");
	}
	else{
		int i;
		for (i=0; i<MAX_CHILDREN; i++){
			new_node -> child[i] = NULL;
		}
		new_node -> sibling = NULL;
		new_node -> lineno = 0;
		new_node -> linepos = 0;
	}
	return new_node;
}

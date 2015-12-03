#include "util.h"

char * nonterminal_names[40] = {
	"empty", "program", "decls", "decls", "decl", "decl", "stmt", "stmt", "stmt", "stmt", 
	"ifstmt", "assignstmt", "compoundstmt", "whilestmt", "stmts", "stmts",
	"boolexpr", "boolop", "boolop", "boolop", "boolop", "boolop", "boolop", 
	"arithexpr", "arithexprprime", "arithexprprime", "arithexprprime", "multiexpr", 
	"multiexprprime", "multiexprprime", "multiexprprime", 
	"simpleexpr", "simpleexpr", "simpleexpr", "simpleexpr"
};

extern char * terminal_names[];

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

TreeNode* create_node(NodeType type, int child_number){
	TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
	if (new_node == NULL){
		printf("error, out of memory!\n");
	}
	else{
		int i;
		for (i=0; i<MAX_CHILDREN; i++){
			new_node -> child[i] = NULL;
		}
		new_node -> node_type = type;
		new_node -> sibling = NULL;
		new_node -> lineno = 0;
		new_node -> linepos = 0;
		new_node -> n_child = child_number;
	}
	return new_node;
}

TreeNode* error_node(){
	static TreeNode* t = NULL;
	if (t==NULL){
		t = (TreeNode*)malloc(sizeof(TreeNode));
		if (t == NULL){
			printf("error, out of memory!\n");
		}

		int i;
		for (i=0; i<MAX_CHILDREN; i++){
			t -> child[i] = NULL;
		}
		t -> node_type = ERRORNODE;
		t -> sibling = NULL;
		t -> lineno = 0;
		t -> linepos = 0;
		t -> n_child = 0;
	}
	return t;
}

void print_node(TreeNode* node){
	printf("(");
	if (node->node_type == ERRORNODE){
		printf("error node");
	}
	else if (node->node_type == TERMINAL){
		printf("%s", terminal_names[node->type.term]);
		switch (node->type.term){
			case ID:
				printf(", %s", node->value.name);
				break;
			case INTNUM:
				printf(", %d", node->value.int_val);
				break;
			case REALNUM:
				printf(", %.3lf", node->value.real_val);
				break;
			default:
				break;
		}
	}
	else if (node->node_type == NONTERMINAL){
		printf("%s", nonterminal_names[node->type.nonterm]);
	}
	printf(")");
}

void print_tree(TreeNode* root){
	// special flag node
	TreeNode* endline_node = create_node(0, 0);
	TreeNode* l_paren_node = create_node(0, 0);
	TreeNode* r_paren_node = create_node(0, 0);

	TreeNode* node_list[300];
	TreeNode* t;
	int first = 0, last = 0, max_list = 298;
	node_list[last++] = root;
	node_list[last++] = endline_node;
	while(last != (first+1) % max_list){
		//printf("first: %d, last: %d, top_addr:%p\n", first, last, node_list[first]);
		t = node_list[first];
		if (t == l_paren_node){
			printf(" [");
			first = (first+1) % max_list;
		}
		else if (t == r_paren_node){
			printf("] ");
			first = (first+1) % max_list;
		}
		else if (t == endline_node){
			printf("\n");
			node_list[last] = endline_node;
			last = (last+1) % max_list;
			first = (first+1) % max_list;
		}
		else {
			if (t->n_child > 0){
				printf("<");
				print_node(t);
				printf(">");
				first = (first+1) % max_list;

				node_list[last] = l_paren_node;
				last = (last+1) % max_list;
				int i;
				for (i=0; i<t->n_child; i++){
					node_list[last] = t->child[i];
					last = (last+1) % max_list;
				}
				node_list[last] = r_paren_node;
				last = (last+1) % max_list;
			}
			else {
				print_node(t);
				first = (first+1) % max_list;
			}
			/* free test
			if(t->node_type != ERRORNODE){
				free(t); // free tree node
			}
			*/
		}
	}
	printf("\n");
	free(endline_node);
	free(l_paren_node);
	free(r_paren_node);
}

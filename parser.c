#include "globals.h"
#include "util.h"

#define N_TERMINAL 27
#define START_NONTERMINAL 1
#define MAX_RULE_ORDER 34

extern int yylex();
extern Token next_token;

TokenType terminals[50] = {
	ERROR, SEMI, INT, ID, ASSIGN, INTNUM, 
	REAL, REALNUM, IF, LPAREN, RPAREN, THEN, 
	ELSE, LBRACKET, RBRACKET, WHILE, LT, GT, 
	LET, GET, EQ, NEQ, PLUS, MINUS, 
	TIMES, OVER, AT_EOF
};

char * terminal_names[40] = {
	"error", ";", "int", "id", "=", "intvalue", 
	"real", "realvalue", "if", "(", ")", "then",
	"else", "{", "}", "while", "<", ">",
	"<=", ">=", "==", "!=", "+", "-",
	"*", "/", "$"
};

int parsing_table[50][50] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,36,1,36,36,36,1,36,36,36,36,36,36,1,36,36,36,36,36,36,36,36,36,36,36,36,35}, // program 1
	{0,36,2,36,36,36,2,36,36,36,36,36,36,3,36,36,36,36,36,36,36,36,36,36,36,36,35}, // decls 2
	{0,35,4,36,36,36,5,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,35}, // decl 3
	{0,36,36,7,36,36,36,36,6,36,36,36,35,8,35,9,36,36,36,36,36,36,36,36,36,36,35}, // stmt 4
	{0,36,36,35,36,36,36,36,10,36,36,36,35,35,35,35,36,36,36,36,36,36,36,36,36,36,35}, // ifstmt 5
	{0,36,36,11,36,36,36,36,35,36,36,36,35,35,35,35,36,36,36,36,36,36,36,36,36,36,35}, // assgstmt 6
	{0,36,36,35,36,36,36,36,35,36,36,36,35,12,35,35,36,36,36,36,36,36,36,36,36,36,35}, // compoundstmt 7
	{0,36,36,35,36,36,36,36,35,36,36,36,35,35,35,13,36,36,36,36,36,36,36,36,36,36,35}, // whilestmt 8
	{0,36,36,14,36,36,36,36,14,36,36,36,36,14,15,14,36,36,36,36,36,36,36,36,36,36,35}, // stmts 9
	{0,36,36,16,36,16,36,16,36,16,35,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,35}, // boolexpr 10
	{0,36,36,35,36,35,36,35,36,35,36,36,36,36,36,36,17,18,19,20,21,22,36,36,36,36,35}, // boolop 11
	{0,35,36,23,36,23,36,23,36,23,35,36,36,36,36,36,35,35,35,35,35,35,36,36,36,36,35}, // arithexpr 12
	{0,26,36,36,36,36,36,36,36,36,26,36,36,36,36,36,26,26,26,26,26,26,24,25,36,36,35}, // arithexprprime 13
	{0,35,36,27,36,27,36,27,36,27,35,36,36,36,36,36,35,35,35,35,35,35,35,35,36,36,35}, // multexpr 14
	{0,30,36,36,36,36,36,36,36,36,30,36,36,36,36,36,30,30,30,30,30,30,30,30,28,29,35}, // multexprprime 15
	{0,35,36,31,36,32,36,33,36,34,35,36,36,36,36,36,35,35,35,35,35,35,35,35,35,35,35} // simpleexpr 16
};

/* 
replace rule, "0" indicate the end of a rule 
positive numbers indicate the non-terminals
negetive numbers indicate the terminals
*/
int push_rules[50][15] = {
	{0}, // 0
	{7,2,0}, // 1
	{2,-1,3,0}, // 2
	{0}, // 3
	{-5,-4,-3,-2,0}, // 4
	{-7,-4,-3,-6,0}, // 5
	{5,0}, // 6
	{6,0}, // 7
	{7,0}, // 8
	{8,0}, // 9
	{4,-12,4,-11,-10,10,-9,-8,0}, // 10
	{-1,12,-4,-3,0}, // 11
	{-14,9,-13,0}, // 12
	{4,-10,10,-9,-15,0}, // 13
	{9,4,0}, // 14
	{0}, // 15
	{12,11,12,0}, // 16
	{-16,0}, // 17
	{-17,0}, // 18
	{-18,0}, // 19
	{-19,0}, // 20
	{-20,0}, // 21
	{-21,0}, // 22
	{13,14,0}, // 23
	{13,14,-22,0}, // 24
	{13,14,-23,0}, // 25
	{0}, // 26
	{15,16,0}, // 27
	{15,16,-24,0}, // 28
	{15,16,-25,0}, // 29
	{0}, // 30
	{-3,0}, // 31
	{-5,0}, // 32
	{-7,0}, // 33
	{-10,12,-9,0} // 34
};

int node_child_number[40] = {
	0, 2, 3, 0, 4, 4, //0-5
	1, 1, 1, 1, 8, //6-10
	4, 3, 5, 2, 0, //11-15
	3, 1, 1, 1, 1, //16-20
	1, 1, 2, 3, 3, //21-25
	0, 2, 3, 3, 0, //26-30
	1, 1, 1, 3 //31-34
};

int node_finished(TreeNode* t){
	int n_fin = 0;
	while(t->child[n_fin] != NULL){
		n_fin++;
	}
	return n_fin == t->n_child;
}

TreeNode* node_stack[200];
int node_stack_top = -1;

void check_node_stack(){
	while(node_stack_top >= 0){
		TreeNode* t = node_stack[node_stack_top];
		if (!node_finished(t)){
			break;
		}

		if (node_stack_top != 0){
			TreeNode* f = node_stack[node_stack_top-1];
			int next_child = 0;
			while(f->child[next_child]!=NULL){
				next_child++;
			}

			f->child[next_child] = t;
		}

		node_stack_top--;
	}
}

TreeNode* push_node(NodeType nodetype, int order){
	TreeNode* t;
	if (nodetype == ERRORNODE){
		t = error_node();
	}
	else if (nodetype == NONTERMINAL){
		t = create_node(nodetype, node_child_number[order]);
		t->type.nonterm = order;
	}
	else if (nodetype == TERMINAL){
		t = create_node(nodetype, 0);
		t->type.term = order;
	}
	node_stack_top++;
	node_stack[node_stack_top] = t;

	check_node_stack();

	return t;
}

int terminal_order(TokenType tokenType){
	int i;
	for (i = 1; i <= N_TERMINAL; i++){
		if (tokenType == terminals[i]){
			return i;
		}
	}
	return -1; // error
}

void missed_error_message(int terminal){
	printf("error: missed \"%s\" at %d:%d\n", terminal_names[terminal], next_token.lineno, next_token.linepos);
}

void scan(int nonterminal){
	int terminal = terminal_order(next_token.type);
	while(parsing_table[nonterminal][terminal] == MAX_RULE_ORDER + 2){
		yylex();
		//printf("token:%s, at %d:%d\n", terminal_names[terminal_order(next_token.type)], next_token.lineno, next_token.linepos);
		terminal = terminal_order(next_token.type);

		if (next_token.type == AT_EOF){
			return;
		}
	}
}

TreeNode* parse(){
	int stack[500];
	int top = 0; // stack's top

	stack[top] = START_NONTERMINAL; // push start symbol into the stack
	TreeNode* tree_root = push_node(NONTERMINAL, START_NONTERMINAL); // push root node to node stack

	int terminal = 0;
	int is_valid = 1;
	yylex();
	//printf("token:%s, at %d:%d\n", terminal_names[terminal_order(next_token.type)], next_token.lineno, next_token.linepos);
	
	while(top >= 0){
		//printf("top:%d, stack[top]:%d\n", top, stack[top]);

		terminal = terminal_order(next_token.type);

		// terminal
		if (stack[top] < 0){
			if (abs(stack[top]) == terminal){
				//printf("matched:%s, at %d:%d\n", terminal_names[terminal], next_token.lineno, next_token.linepos);
				// matched, pop
				top--;
				push_node(TERMINAL, next_token.type);
				// get next token
				yylex();
				//printf("token:%s, at %d:%d\n", terminal_names[terminal_order(next_token.type)], next_token.lineno, next_token.linepos);
			}
			else {
				missed_error_message(abs(stack[top]));
				top--;
				push_node(ERRORNODE, 0);
				is_valid = 0;
			}
		}
		// non-terminal
		else if (stack[top] > 0){
			int rule_order = parsing_table[stack[top]][terminal];

			// pop
			if (rule_order == MAX_RULE_ORDER + 1){
				printf("error at %d:%d, do pop\n", next_token.lineno, next_token.linepos);
				top--; // pop
				push_node(ERRORNODE, 0);
				is_valid = 0;
			} 
			// scan
			else if (rule_order == MAX_RULE_ORDER + 2){
				printf("error at %d:%d, do scan\n", next_token.lineno, next_token.linepos);
				scan(stack[top]);
				is_valid = 0;
			}
			// valid
			else {
				//printf("match rule %d\n", rule_order);
				int * rule;
				rule = push_rules[rule_order];

				top--; // pop
				push_node(NONTERMINAL, rule_order);
				int i = 0;
				while(rule[i] != 0){
					top++;
					stack[top] = rule[i];
					i++;
				}
			}
		}
		else{
			// should not get here
		}
	}
	// success
	if (is_valid && (next_token.type == AT_EOF)){
		printf("valid!\n");
	}
	// input is not empty
	if (next_token.type != AT_EOF){
		printf("error at %d:%d\n", next_token.lineno, next_token.linepos);
	}

	return tree_root;
}

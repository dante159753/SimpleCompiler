#include "interpreter.h"

void execute_node(TreeNode* node)
{
	if(node->node_type == NONTERMINAL)
	{
		switch(node->type.nonterm)
		{
			case PROG:
				
				break;
			case DECLS:
				
				break;
			case DECL:
				
				break;
			case STMT:
				
				break;
			case IFSTMT:
				
				break;
			case ASSIGNSTMT:
				
				break;
			case COMPOUNDSTMT:
				
				break;
			case WHILESTMT:
				
				break;
			case STMTS:
				
				break;
			case BOOLEXPR:
				
				break;
			case BOOLOP:
				
				break;
			case ARITHEXPR:
				
				break;
			case ARITHEXPR_P:
				
				break;
			case MULTIEXPR:
				
				break;
			case MULTIEXPR_P:
				
				break;
			case SIMPLEEXPR:
				
				break;
		}
	}
	else if(node->node_type == TERMINAL)
	{
		switch(node->type.term)
		{
			
		}
	}
}

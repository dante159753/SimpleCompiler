#include "interpreter.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

int onerror = 0;

int check_variable(TreeNode* node)
{
	if(node->node_type == NONTERMINAL)
	{
		if(node->type.nonterm == DECL)
		{
			TokenType id_type = node->child[0]->type.term;
			char* name = node->child[1]->value.name;
			if(find_id(name) == 1)
			{
				TreeNode* name_node = node->child[1];
				printf("error at %d,%d: Redefined identifer %s\n", 
					name_node->lineno, 
					name_node->linepos, 
					name_node->value.name
					);
				onerror = 1;
			}
			void* value = (void*)(&(node->value));
			insert_id(name, id_type, value);
		}
		else
		{
			int i;
			for(i=0; i<node->n_child; i++)
			{
				check_variable(node->child[i]);
			}
		}
	}
	else if (node->node_type == TERMINAL)
	{
		// this id is not child of decl, so it must be a child of simpleexpr or assignment
		if(node->type.term == ID)
		{
			char* name = node->value.name;
			if(find_id(name) == -1)
			{
				printf("error at %d,%d: Undecleared identifer %s\n", 
					node->lineno,
					node->linepos,
					node->value.name
					);
				onerror = 1;
			}
		}
	}
}

ExprValue* do_cal(ExprValue* left, ExprValue* right, TokenType op)
{
	ExprValue* cal_result = (ExprValue*) malloc(sizeof(ExprValue));

	// use double to calculate
	double l_val = left->exprtype == INT_EXPR? left->value.int_val : left->value.real_val;
	double r_val = right->exprtype == INT_EXPR? right->value.int_val : right->value.real_val;
	double result_val;

	switch(op)
	{
		case PLUS:
			result_val = l_val + r_val;
			break;
		case MINUS:
			result_val = l_val - r_val;
			break;
		case TIMES:
			result_val = l_val * r_val;
			break;
		case OVER:
			result_val = l_val / r_val;
			break;
	}

	// determain type of result
	if(left->exprtype != right->exprtype)
	{
		cal_result->exprtype = REAL_EXPR;
		cal_result->value.real_val = result_val;
	}
	else if(left->exprtype == INT_EXPR)
	{
		cal_result->exprtype = INT_EXPR;
		cal_result->value.int_val = result_val;
	}
	else if(left->exprtype == REAL_EXPR)
	{
		cal_result->exprtype = REAL_EXPR;
		cal_result->value.real_val = result_val;
	}

	return cal_result;
}

ExprValue* eval_expr(TreeNode* node, ExprValue* pre_value)
{
	if(node->node_type == NONTERMINAL)
	{
		switch(node->type.nonterm)
		{
			case ARITHEXPR:
			{
				ExprValue* multi_expr_result;
				multi_expr_result = eval_expr(node->child[0], NULL);
				return eval_expr(node->child[1], multi_expr_result);

				break;
			}
			case ARITHEXPR_P:
			{
				if(node->n_child == 0)
				{
					// node is empty
					return pre_value;
				}
				else
				{
					ExprValue* multi_expr_result = eval_expr(node->child[1], NULL);
					ExprValue* op_result = do_cal(pre_value, multi_expr_result, node->child[0]->type.term);
					return eval_expr(node->child[2], op_result);
				}

				break;
			}
			case MULTIEXPR:
			{
				ExprValue* simple_expr_result = eval_expr(node->child[0], NULL);
				return eval_expr(node->child[1], simple_expr_result);

				break;
			}
			case MULTIEXPR_P:
			{
				if(node->n_child == 0)
				{
					// node is empty
					return pre_value;
				}
				else
				{
					ExprValue* simple_expr_result = eval_expr(node->child[1], NULL);
					ExprValue* op_result = do_cal(pre_value, simple_expr_result, node->child[0]->type.term);
					return eval_expr(node->child[2], op_result);
				}

				break;
			}
			case SIMPLEEXPR:
			{
				if(node->n_child == 1)
				{
					ExprValue* ret_val;
					switch(node->child[0]->type.term)
					{
						case ID:
						{
							int found = get_id(node->child[0]->value.name, &ret_val);

							break;
						}
						case INTNUM:
						{
							ret_val = (ExprValue*) malloc(sizeof(ExprValue));
							ret_val->exprtype = INT_EXPR;
							ret_val->value.int_val = node->child[0]->value.int_val;

							break;
						}
						case REALNUM:
						{
							ret_val = (ExprValue*) malloc(sizeof(ExprValue));
							ret_val->exprtype = REAL_EXPR;
							ret_val->value.real_val = node->child[0]->value.real_val;

							break;
						}
					}
					return ret_val;
				}
				else
				{
					return eval_expr(node->child[1], NULL);
				}
				break;
			}
		}
	}
}

ExprValue* cal_expr(TreeNode* node)
{
	ExprValue* empty_node = (ExprValue*) malloc(sizeof(ExprValue));
	empty_node->exprtype = INT_EXPR;
	empty_node->value.int_val = 0;

	return eval_expr(node, empty_node);
}

int execute_boolexpr(TreeNode* node)
{
	ExprValue* result_l = cal_expr(node->child[0]);
	ExprValue* result_r = cal_expr(node->child[2]);

	double l_val = result_l->exprtype == INT_EXPR? result_l->value.int_val : result_l->value.real_val;
	double r_val = result_r->exprtype == INT_EXPR? result_r->value.int_val : result_r->value.real_val;

	switch(node->child[1]->type.term)
	{
		case LT:
			return l_val < r_val;
			break;
		case GT:
			return l_val > r_val;
			break;
		case LET:
			return l_val <= r_val;
			break;
		case GET:
			return l_val >= r_val;
			break;
		case EQ:
			return l_val == r_val;
			break;
		case NEQ:
			return l_val != r_val;
			break;
		default:
			printf("unknown boolop!\n");
			return 0;
	}
}

void execute_node(TreeNode* node)
{
	if(node->node_type == NONTERMINAL)
	{
		int i;
		switch(node->type.nonterm)
		{
			case PROG:
				for(i=0; i<node->n_child; i++)
				{
					execute_node(node->child[i]);
				}
				break;
			case DECLS:
				// no need to check, has been checked by `check_variable'
				break;
			case DECL:
				// no need to check, has been checked by `check_variable'
				break;
			case STMT:
				execute_node(node->child[0]);
				break;
			case IFSTMT:
				if(execute_boolexpr(node->child[2]))
				{
					execute_node(node->child[5]);
				}
				else
				{
					execute_node(node->child[7]);
				}
				break;
			case ASSIGNSTMT:
			{
				TreeNode* name_node = node->child[0];
				ExprValue* return_val = cal_expr(node->child[2]);
				ExprValue* id_val;
				get_id(name_node->value.name, &id_val);

				if(id_val->exprtype == INT_EXPR && return_val->exprtype == REAL_EXPR)
				{
					// warning
					printf("warning at %d,%d: Assign real value to int variable %s\n", 
						name_node->lineno,
						name_node->linepos,
						name_node->value.name
						);
					id_val->value.int_val = return_val->value.real_val;
				}
				else if(id_val->exprtype == REAL_EXPR && return_val->exprtype == REAL_EXPR)
				{
					id_val->value.real_val = return_val->value.real_val;
				}
				else if(id_val->exprtype == INT_EXPR && return_val->exprtype == INT_EXPR)
				{
					id_val->value.int_val = return_val->value.int_val;
				}
				else if(id_val->exprtype == REAL_EXPR && return_val->exprtype == INT_EXPR)
				{
					id_val->value.real_val = return_val->value.int_val;
				}

				break;
			}
			case COMPOUNDSTMT:
				execute_node(node->child[1]);
				break;
			case WHILESTMT:
				while(execute_boolexpr(node->child[2]))
				{
					execute_node(node->child[4]);
				}

				break;
			case STMTS:
				if(node->n_child != 0)
				{
					execute_node(node->child[0]);
					execute_node(node->child[1]);
				}
				break;
			case BOOLEXPR:
			case BOOLOP:
			case ARITHEXPR:
			case ARITHEXPR_P:
			case MULTIEXPR:
			case MULTIEXPR_P:
			case SIMPLEEXPR:
				printf("should not execute this!\n");
				break;
		}
	}
	else if(node->node_type == TERMINAL)
	{
		// should not enter here
	}
}


void analyze(TreeNode* node)
{
	// init program state
	onerror = 0;
	check_variable(node);
	if(!onerror)
	{
		execute_node(node);

		print_symbol_table();
	}
	else
	{
		printf("There are errors in program, execution terminated.\n");
	}
}

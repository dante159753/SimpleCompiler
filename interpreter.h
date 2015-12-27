#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include "globals.h"

ExprValue* cal_expr(TreeNode* node);

void build_table(TreeNode* declnode);

void execute_node(TreeNode* node);

#endif

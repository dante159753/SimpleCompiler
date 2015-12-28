#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include "globals.h"

int check_variable(TreeNode* node);

void execute_node(TreeNode* node);

void analyze(TreeNode* node);

#endif

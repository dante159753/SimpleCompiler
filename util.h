#ifndef _UTIL_H_
#define _UTIL_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

char* copy_str(char *);

TokenType get_token_bystr(char*);

TreeNode* create_node(NodeType, int);

TreeNode* error_node();

void print_tree(TreeNode*);

void print_node(TreeNode*);

void print_tree_json(TreeNode*, FILE*);

#endif

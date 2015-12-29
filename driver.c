#include "util.h"
#include "globals.h"
#include "interpreter.h"

Token next_token;
extern int yylex();
extern char * yytext;
extern TreeNode * parse();

void lexical_analyze()
{
	yylex();
	while(next_token.type!=AT_EOF){
		/*
		printf("(%s, %d, %d, %d)", yytext, next_token.type, next_token.lineno, next_token.linepos);
		if(next_token.type == ID){
			printf("ID:%s,", next_token.value.name);
		}
		if(next_token.type == INTNUM){
			printf("INTNUM:%d,", next_token.value.int_val);
		}
		if(next_token.type == REALNUM){
			printf("REALNUM:%.4lf,", next_token.value.real_val);
		}
		*/
		
		printf("(%s, %d, %d, %d)", yytext, next_token.type, next_token.lineno, next_token.linepos);
		if(next_token.type == ERROR)
		{
			printf("unrecognized token:%s, at %d:%d\n", yytext, next_token.lineno, next_token.linepos);
		}

		yylex();
	}

}

int main(int argc, char ** argv){
	int mode = 0, i = 1;
	for(i=1; i<argc; i++)
	{
		if(strcmp(argv[i], "-scanning") == 0)
		{
			mode = 1;
		}
		else if(strcmp(argv[i], "-parsing") == 0)
		{
			mode = 2;
		}
	}

	if(mode == 1) // only lexical analyze
	{
		lexical_analyze();
		return 0;
	}

	if(mode == 2) // only syntactic analyze
	{
		TreeNode* root = parse();
	
		FILE* fp;
		fp = fopen("pasing_tree.json", "w+");
		print_tree_json(root, fp);
		fclose(fp);

		return 0;
	}

	// otherwise, do everything
	TreeNode* root = parse();

	FILE* fp;
	fp = fopen("pasing_tree.json", "w+");
	print_tree_json(root, fp);
	fclose(fp);

	analyze(root);

	return 0;
}

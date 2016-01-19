#include "util.h"
#include "globals.h"
#include "interpreter.h"

Token next_token;
extern int yylex();
extern char * yytext;
extern int yyleng;
extern TreeNode * parse();
extern int is_valid;

int is_for_ui = 0; /**< Switch for highlighting json. >*/
int mode = 0; /**< Executing mode, default to executing. >*/

/**
 * Do lexical analysis with respect to the given args.
 * If only lexical analysis, call this.
 */
void lexical_analyze()
{
	yylex();

	if(is_for_ui)
		fprintf(stderr, "[");
	while(next_token.type!=AT_EOF){
		if(is_for_ui && next_token.type != ERROR)
		{
			fprintf(stderr, "{'type': 0, 'token': %d, 'lineno': %d, 'linepos': %d, 'len': %d}", 
				next_token.type,
				next_token.lineno,
				next_token.linepos,
				yyleng
				);
		}
		else if(next_token.type == ERROR)
		{
			if(is_for_ui)
			{
				fprintf(stderr, "{'type': 0, 'token': %d, 'lineno': %d, 'linepos': %d, 'len': %d}",
					next_token.type,
					next_token.lineno,
					next_token.linepos,
					yyleng
					);
			}
			printf("unrecognized token:%s, at %d:%d\n", yytext, next_token.lineno, next_token.linepos);
		}

		yylex();
		if(is_for_ui && next_token.type!=AT_EOF)
		{
			fprintf(stderr, ", ");
		}
	}
	if(is_for_ui)
		fprintf(stderr, "]\n");

}

/**
 * Console args available: -ui -scanning -parsing -analyze.
 * -ui: print json to stderr for code highlighting.
 * -scanning: only do lexical analysis.
 * -parsing: only do lexical and syntactical analysis.
 * -analyze: do all analysis.
 */
int main(int argc, char ** argv){
	int i = 1;
	int gene_tree = 0;
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
		else if(strcmp(argv[i], "-analyze") == 0)
		{
			mode = 3;
		}
		else if(strcmp(argv[i], "-ui") == 0)
		{
			is_for_ui = 1;
		}
		else if(strcmp(argv[i], "-tree") == 0)
		{
			gene_tree = 1;
		}
	}

	if(mode == 1) // only lexical analysis
	{
		lexical_analyze();
		return 0;
	}

	// mode = 2 or 0, do sytactical analysis
	if(is_for_ui)
	{
		fprintf(stderr, "[{'type': 5}"); // add a empty dict
	}
	TreeNode* root = parse();
	if(is_for_ui)
	{
		fprintf(stderr, "]\n");
	}

	if(gene_tree)
	{
		FILE* fp;
		fp = fopen("pasing_tree.json", "w+");
		print_tree_json(root, fp);
		fclose(fp);
	}

	// mode = 0
	if(mode == 0 || mode == 3)
	{
		if(is_valid)
		{
			analyze(root);
		}
	}

	return 0;
}

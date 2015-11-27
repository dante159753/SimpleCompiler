#include "util.h"
#include "globals.h"

Token next_token;
extern int yylex();
extern char * yytext;
extern TreeNode * parse();

int main(){
	/*
	yylex();
	while(next_token.type!=AT_EOF){
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

		yylex();
	}
	*/

	parse();
	return 0;
}

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "util.h"

extern Token next_token;
#define ECHO {next_token.type=ERROR;next_token.lineno=line_number;next_token.linepos=line_pos;line_pos+=yyleng; return next_token.type;}
int line_pos = 1;
int line_number = 1;
%}
blank [ \t]
comments \/\/.*(\n|$)
keywords int|real|if|then|else|while
letter [A-Za-z]
digit [0-9]
intnumber {digit}+
exponent E("+"|"-")?{digit}+
fraction .{digit}+
realnumber ({digit}+{exponent})|({digit}+{fraction}{exponent}?)
operators "+"|"-"|"/"|"*"|"=="|"="|"<="|">="|"!="|"<"|">"
delimiters "("|")"|"{"|"}"|";"
identifiers {letter}({letter}|{digit})*
%%
{blank} line_pos += yyleng;
{keywords} {next_token.type=get_token_bystr(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
{identifiers} {next_token.type=ID; next_token.value.name=copy_str(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
{intnumber} {next_token.type=INTNUM; next_token.value.int_val=atoi(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
{realnumber} {next_token.type=REALNUM; next_token.value.real_val=atof(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
{comments} {line_pos=1;line_number++;}
{operators} {next_token.type=get_token_bystr(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
{delimiters} {next_token.type=get_token_bystr(yytext); next_token.lineno=line_number; next_token.linepos=line_pos; line_pos += yyleng; return next_token.type;}
\n {line_pos = 1;line_number++;}
<<EOF>> {next_token.type=AT_EOF;return -1;}
%%

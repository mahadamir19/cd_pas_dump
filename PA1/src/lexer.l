%{
#include <stdio.h>
#include <string.h>
#include "tokens.h"

int yylineno; 
%}

%option yylineno
%option noyywrap

/* Regex Definitions */

/* lexer rules section */ 
/* define your regex rules to match the tokens here (an example has been provided)*/
%%
"if"                { return T_IF; }


%%
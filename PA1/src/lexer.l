%{
#include <stdio.h>
#include "tokens.h"
%}

%option yylineno
%option noyywrap

%%
[ \t\n\r]+          ; /* Ignore spaces/tabs */


"//".*     ;

"if"            { return T_IF; }
"int" {return T_INT;}
"float" {return T_FLOAT;}
"bool" {return T_BOOL;}

"("     {return T_LPAREN;}
")"     {return T_RPAREN;}
"{"     {return T_LBRACE;}
"}"     {return T_RBRACE;}
"=" {return T_ASSIGN;}
";" {return T_SEMI;}
"main" {return T_MAIN;}

[0-9]+ {return T_INT_LIT;}
[0-9]+.[0-9]+ {return T_FLOAT_LIT;}
"true"      {return T_BOOL_LIT;}
[A-Za-z_]+  {return T_ID;}

.               { return T_ERROR; }
%%
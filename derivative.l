%{
#include"derivative.h"
#include"derivative.tab.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%
"+"|"-"|"*"|"/"|"("|")"|"|"|"^"     { return yytext[0]; }
"sin"|"Sin"   { yylval.fn = B_sin ; return FUNC ;}
"cos"|"Cos"   { yylval.fn = B_cos ; return FUNC ;}
[xX]    { return VAR  ;}
[0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?    {yylval.d = atof(yytext); return NUMBER;}
[ \t] /* nothing */
\n      {return EOL;}
.       { yyerror("unrecognize pattern , %c is not recognized!\n", *yytext);}
%%



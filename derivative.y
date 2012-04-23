%{
#include<stdlib.h>
#include<stdio.h>
#include"derivative.h"
%}

%union{
    struct ast *a;
    double d;
    char var;
    int fn ; 
}

%token <d> NUMBER
%token <var> VAR
%token <fn> FUNC
%token EOL

%left '+' '-'
%left '*' '/'
%right '^'
%nonassoc '|' UMINUS

%type <a> exp 
%start callist
%%

exp : exp '+' exp { $$ = newast('+',$1,$3);}
    | exp '-' exp { $$ = newast('-',$1,$3);}
    | exp '*' exp { $$ = newast('*',$1,$3);}
    | exp '/' exp { $$ = newast('/',$1,$3);}
    | '(' exp ')' { $$ = $2;}
    | exp '^' exp { $$ = newast('^',$1, $3); }
    | '-' exp %prec UMINUS { $$ = newast('M',$2,NULL);}
    | '|' exp     { $$ = newast('|',$2,NULL);}
    | NUMBER       { $$ = newnum ($1);}
    | VAR           { $$ = newvar($1);}
    | FUNC '(' exp ')' {  $$ = newfunc($1,$3);  }
    ;

callist:
        | callist EOL  { printf("->$$");}
        | callist exp EOL{            printf("->$$");showtree(dev($2));    free($2);printf("\n->$$"); } 
        ;
%%
void yyerror(char *s,...)
{
    fprintf(stderr,"%d:error",yylineno);
    fprintf(stderr,"\n");
}

int main()
{
    printf("->$$");
    return yyparse();
}

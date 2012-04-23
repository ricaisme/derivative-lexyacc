/*
 * =====================================================================================
 *
 *       Filename:  derivative.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/14/2011 01:06:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

enum bifs{
    B_sin = 1,
    B_cos
};

/*build in function s such as sin cos */

struct fncall{
    int nodetype;
    struct ast *l;
    enum bifs functype;
};


struct numval{
    int nodetype;
    double number;
};

struct varval{
    int nodetype;
    char *variable;
};


struct ast *newast(int nodetype , struct ast *l, struct ast *r);
struct ast *newnum(double value);
struct ast *newfunc(int functype , struct ast *l);
struct ast *newvar();

void showtree(struct ast *a);
struct ast *dev(struct ast * a);

void treefree(struct ast *);

extern int yylineno;
void yyerror(char *s,...);

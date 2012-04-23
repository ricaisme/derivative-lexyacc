/*
 * =====================================================================================
 *
 *       Filename:  der_func.c
 *
 *    Description:  THE main function file about derivative
 *
 *        Version:  1.0
 *        Created:  04/15/2011 02:56:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mi HU  
 *        Company:  ESIEE PARIS 
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<math.h>
#include  "derivative.h"

struct ast *newast(int nodetype , struct ast *l, struct ast *r)
{
    struct ast  *a = malloc(sizeof(struct ast));
    if(!a)
    {
        yyerror("error malloc ast *a!\n");
        exit(0);
    }

    a->nodetype = nodetype;
    a->l = l ;
    a->r = r;

    return a;

}

struct ast *newnum(double value)
{
    struct numval *a = malloc(sizeof(struct numval));
    if(!a)
    {
        yyerror("error malloc numval *a!\n");
        exit(0);
    }

    a->nodetype = 'K';
    a->number = value;
    return (struct ast *)a;
}

struct ast *newfunc(int functype , struct ast *l)
{
    struct fncall *a = malloc(sizeof(struct fncall));
    if(!a)
    {
        yyerror("error malloc newfunc *a!\n");
        exit(0);
    }
    a->nodetype = 'F';
    a->l = l;
    a->functype = functype;
    return (struct ast *) a ;

}

struct ast *newvar()
{

    struct varval *a = malloc(sizeof(struct varval));
    if(!a)
    {
        yyerror("error malloc newvar  *a!\n");
        exit(0);
    }
    a->nodetype = 'V';
    a->variable  = "x";
    return (struct ast *)a;
}

void treefree( struct ast *a)
{
    switch(a->nodetype)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case 'F':
            {
            treefree(a->l);
            treefree(a->r);
            }break;

        case 'K':
        case 'V':
        case 'M':
            {
            
            }break;
    }
    free(a);
}


struct ast *check_fn( struct fncall *f)
{
    struct ast *fun_res = malloc(sizeof(struct ast ));
    if(!fun_res)
    {
        yyerror("error malloc fun_res  !\n");
        exit(0);
    }

    enum bifs functype = f->functype; 
    
    switch(functype)
    {
        case B_sin:
        {
           fun_res->nodetype='*';
           fun_res->l=(struct ast *)newfunc(B_cos,f->l);
           fun_res->r=(struct ast *)(dev(f->l));
        } break;
        case B_cos:
        {
            fun_res->nodetype='*';
            fun_res->l=(struct ast *)(newfunc(B_sin,f->l));
            fun_res->r= newast('*',newnum(-1),(struct ast *)(dev(f->l)));
        } break;
        default:
        break;
    }
    return (struct ast *)fun_res;          
}

// to deal with number w
struct ast *devk(struct numval *k)
{
    struct numval *resk = malloc(sizeof(struct numval));
    if(!resk)
    {
        yyerror("error malloc newvar  *resk!\n");
        exit(0);
    }
    
    resk->nodetype = 'K';
    resk->number = 0;
    return (struct ast *)resk;
}

// to deal with variable x 
struct ast *devv(struct varval *v)
{
    struct numval *resv = malloc(sizeof(struct numval));
    if(!resv)
    {
        yyerror("error malloc newvar  *resv!\n");
        exit(0);
    }
    resv->nodetype = 'K';
    resv->number = 1;
    return (struct ast*)resv;
}

// to deal with u^3 eauqtion 
struct ast *deve(struct numval *e)
{

    struct numval *rese = malloc(sizeof(struct numval));
    if(!rese)
    {
        yyerror("error malloc newvar  *rese!\n");
        exit(0);
    }
    int eres = e->number;
    rese->nodetype = 'K';
    rese->number = eres-1 ;
    return (struct ast *)rese;

}

// show result 
void showk( struct numval *k)
{
    int nvtint = k->number;
    printf("%d",nvtint);
}

void showv( struct varval *v)
{
    printf("%s",v->variable);
}

// a prototype to printf the function symbol "sin" and "cos"
void showf(struct fncall *f)
{
    enum bifs functype = f->functype;

    switch(functype)
    {
        case B_sin:
            {
                printf("Sin");
            }break;
        case B_cos:
            {
                printf("Cos");
            }break;
        default:
            break;
    }

}
// chkzero need to use it 
int chkval(struct numval * a)
{
    int v = a->number;
    if(v==0)
    {
        return 0;
    }
    else if(v==1)
    {
        return 1;
    }
    else 
    {
        return 3;
    }
}

// a simple function to chk the number node is zero or not 
// that's a function to simplify the result 
int chkzero(struct ast *a)
{

    if(a->nodetype=='K')
    {
        int v = chkval((struct numval *)a);
        if(v==0)
        {
            return 0;
        }
        else if ( v==1)
        {
            return 1;
        }
    }

    else 
    {
        return 2;
    }

}
// function to chknode to trace the tree node type 
void chknode(struct ast *a)
{
    if(a->nodetype=='K')
    {
        printf("K\n");
    }
    else if(a->nodetype=='V')
    {
        printf("V\n");
    
    }
    else if(a->nodetype='+')
    {
        printf("+\n");
    }
}

struct ast *dev( struct ast *a)
{

    struct ast *result = malloc(sizeof(struct ast));
    if(!result)
    {
        yyerror("internal error , NULL ! \n");
        return ;
    }
    switch(a->nodetype)
    {

        case 'K': { 
                      result=devk((struct numval*)a);
                  }break;
        case 'V': {
                      result=devv((struct varval*)a);
                  }break;
                  
        case 'M':{
                      result->nodetype = 'M';
                      result->l=(struct ast *)(dev(a->l));
                 }break;
        
        case '+': { result->nodetype = '+';
                      result->l=(struct ast *)(dev(a->l));
                      result->r=(struct ast *)(dev(a->r));
                  } break;
        case '-':{
                     result->nodetype = '-';
                     result->l=(struct ast *)(dev(a->l));
                      result->r=(struct ast *)(dev(a->r));
                 
                 }break;
        case '*' :{
                      result->nodetype = '+';
                      result->l= newast('*', (struct ast *)(dev(a->l)), (struct ast *)(a->r));
                      result->r= newast('*', (struct ast *)(a->l),  (struct ast *)(dev(a->r)));
                  } break;

        case '/':{
                      result->nodetype = '/';
                      result->l=newast('-', newast('*',(struct ast *)(dev(a->l)),(struct ast*)(a->r)), 
                                                    newast('*',(struct ast *)(a->l),(struct ast*)(dev(a->r))));
                      result->r=newast('^',(struct ast *)(a->r),(struct ast *)(newnum(2)));
                 
                 }break;
        case  '^':{
                      result->nodetype = '*';
                      result->l=newast('^',newast('*',(struct ast *)(a->r),(struct ast *)(a->l)),deve((struct numval*)(a->r)));
                      result->r=(struct ast*)(dev(a->l));
                  }break;           
        case 'F' :{
                      result=check_fn((struct fncall*)a);
                  
                  }   break;
        default : break;
    }
    return (struct ast *)result;

}

void showtree(struct ast *a )
{
    switch(a->nodetype)
    {
        case 'K': {
                      showk((struct numval *)a);
                  }break;
        case 'V': {
                      showv((struct varval *)a);                 
                  }break;
        case 'M':{
                      printf("(-");
                      showtree(a->l);
                      printf(")");
                 }break;
        case '+': {
                    int p1 = chkzero(a->l);
                    int p2 = chkzero(a->r);
                        if(chkzero(a->l)!=0 && chkzero(a->r)==0)
                        {
                            showtree(a->l);
                        }
                        else if((chkzero(a->l)) ==0 &&(chkzero(a->r)!=0))
                        {
                            showtree(a->r);
                        }
                        else if( p1 == 0 && p2 == 0)
                        {
                            printf("0");
                        
                        }
                        else
                        {
                            printf("(");
                            showtree(a->l);
                            printf("+");
                            showtree(a->r);
                            printf(")");
                        } 
                  }
                  break;
        case '-': { 
                      printf("(");
                      showtree(a->l);
                      printf("-");
                      showtree(a->r);
                      printf(")");

                  } break;
        case '*':{
                     
                    int a1 = chkzero(a->l);
                    int a2 = chkzero(a->r);
                    if(a1 > 0 && a2 ==0)
                    {
                        showtree(a->r);
                    }

                    if(a1 == 0 && a2 > 0 )
                    {
                        showtree(a->l);
                    }

                    if(a1 == 1 && a2 >1)
                    {
                        showtree(a->r);
                    }
                    if(a1 >= 1 && a2 == 1)
                    {
                        showtree(a->l);
                    }
                    if(a1>1 && a2 >1)
                    {
                        showtree(a->l);
                        printf("*");
                        showtree(a->r);
                    }

                 } break;
        case '/':{
                    printf("(");
                    showtree(a->l);
                    printf("/");
                    showtree(a->r);
                    printf(")");
                 } break;
        case '^':{
                    printf("(");
                    showtree(a->l);
                    printf("^");
                    showtree(a->r);
                    printf(")");
                 } break;
        case 'F':{
                     showf((struct fncall*)a); 
                     printf("(");
                     showtree(a->l);
                     printf(")");
                 } break;
       
        default : break;


    }



}

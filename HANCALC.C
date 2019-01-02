#include <math.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hanout.h"
#include "hancolor.h"
#include "hanwindw.h"
#include "hanerro2.h"
#include "hancoord.h"

#define DELIMITER 1
#define NUMBER    2
#define EOL       3
#define FINISHED  4

static char *prog;
static char token[80];
static char token_type;
static char tok;

static void get_exp(float *result);
static void level2(float *result);
static void level3(float *result);
static void level4(float *result);
static void level5(float *result);
static void level6(float *result);
static void primitive( float *result);
static void arith(char o, float *r, float *h);
static void unary(char o, float *r);
static void serror(void);
static char get_token(void);
static bool isfloat(char c);
static bool isdelim(char c);
static bool iswhite(char c);

static bool err_flag;

static void get_exp(float *result)
{
  err_flag = false;
  get_token();
  if(!*token) {
    serror();
    return;
  }
  level2(result);
}

static void level2(float *result)
{
  register char op;
  float hold;

  level3(result);
  while((op = *token) == '+' || op == '-'){
    get_token();
    level3(&hold);
    arith(op, result, &hold);
  }
}

static void level3(float *result)
{
  register char op;
  float hold;

  level4(result);
  while((op = *token) == '*' || op == '/' ){
    get_token();
    level4(&hold);
    arith(op, result, &hold);
  }
}

static void level4(float *result)
{
  float hold;

  level5(result);
  if((*token) == '^'){
    get_token();
    level4(&hold);
    arith('^', result, &hold);
  }
}

static void level5(float *result)
{
  register char op;

  op = 0;
  if((token_type==DELIMITER) && *token=='+' || *token=='-'){
    op = *token;
    get_token();
  }
  level6(result);
  if(op)
    unary(op, result);
}

static void level6(float *result)
{
  if((*token == '(') && (token_type == DELIMITER)){
    get_token();
    level2(result);
    if(*token != ')')
      serror();
    get_token();
  }
  else
   primitive(result);
}

static void primitive( float *result)
{
  switch(token_type){
    case NUMBER:
      *result = atof(token);
      get_token();
      return;
    defalt:
      serror();
  }
}

static void arith(char o, float *r, float *h)
{
  switch(o){
    case '-':
      *r = *r-*h;
      break;

    case '+':
      *r = *r+*h;
      break;

   case '*':
      *r = *r * *h;
      break;

   case '/':
      *r = (*r)/(*h);
      break;

/*   case '^' :
      for(t=*h-1; t>0; --t)
        *r = (*r) * ex;
      break; */
   }
}

static void unary(char o, float *r)
{
  if(o =='-') *r = -(*r);
}

static void serror(void)
{
  disperror("‘Ñ ÆÅØ¢∑i ∏i°µ ∑≥ùb–a≠v¥·∂a");
  err_flag = true;
}

static char get_token(void)
{
  register char *temp;

  token_type = 0;
  tok = 0;
  temp = token;
  if(*prog =='\0'){
    *token = 0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }

  while(iswhite(*prog)) ++prog;

  if(*prog=='\r'){
    ++prog; ++prog;
    tok = EOL; *token = '\r';
    token[1]='\n'; token[2]='\r';
    return (token_type = DELIMITER);
  }
  if(strchr("+-*/()",*prog)){
    *temp=*prog;
    prog++;
    temp++;
    *temp=0;
    return (token_type = DELIMITER);
  }

  if(isfloat(*prog)){
    while(!isdelim(*prog))
      *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  return token_type = FINISHED;
}

static bool isfloat(char c)
{
  if( !((int) c - (int) '.') || isdigit(c))
    return true;
  return false;
}

static bool isdelim(char c)
{
  if(strchr("+-/*()",c) || c==9 || c==0 || c=='\r')
    return true;
  return false;
}

static bool iswhite(char c)
{
  if(c==' ' || c=='\t') return true;
  else return false;
}

void calculator(void)
{
  float result;
  char item[80];
  int ans;

  item[0] = NULL;
  ans = win_hgetdata(28, "‘Ñ âÅ¨e–i ÆÅØ¢ (+ - * /)", "EEEEEEEEEEEEEEEEEEEEEEEEEEEE", item);
  if(ans) {
    prog = item;
    get_exp(&result);
    if(!err_flag && wopen(hgetmaxax()/2-14, hgetmaxay()/2-1, 28, 3)) {
      wtitle("‘ê âÅ¨e âiâ¡  ‘ë");
      hprintfxy(2, 2, "%25.2f", result);
      getxch();
      wclose();
    }
  }
}


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* external lexer interface */
extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int current_line;

/* expose AST root */
AST *astRoot = NULL;
%}

/* enable location tracking */
%locations

/* semantic value union */
%union {
    int iVal;
    double dVal;
    char *sVal;
    AST *node;
}

/* tokens */
%token <sVal> ID
%token <iVal> INT_LIT
%token <dVal> FLOAT_LIT
%token <sVal> STRING_LIT

%token CLASS IMPLEMENT FUNC CONSTRUCT ATTRIBUTE PUBLIC PRIVATE RETURN READ WRITE IF ELSE WHILE VOID SELF ISA LOCAL THEN

%token COLON SEMICOLON COMMA DOT ASSIGN ARROW LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token PLUS MINUS MULT DIV AND OR NOT EQ NE LT GT LE GE

/* nonterminals carry AST */
%type <node> statBlock statementList expr exprPrime relExpr arithExpr arithExprPrime term termPrime factor functionCall
%type <node> prog classDecl classInherit moreIds classBody implDef implFuncs
%type <node> funcDef funcHead funcBody varDeclOrStmtList varDeclOrStmt
%type <node> localVarDecl attributeDecl varDecl arraySizes arraySize statement assignStat
%type <node> fParams fParamsTailList aParams aParamsTailList type
%type <node> variable idnestChain idnest indiceChain indice idOrSelf sign addOp multOp

%%

/* top-level */
prog:
      classDecl                    { astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line); ast_append_child(astRoot, $1); }
    | classDecl prog                { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line); ast_append_child(astRoot, $1); }
    | funcDef                       { astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line); ast_append_child(astRoot, $1); }
    | funcDef prog                  { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line); ast_append_child(astRoot, $1); }
    | implDef                       { if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line); ast_append_child(astRoot, $1); }
    | /* empty */                   { astRoot = ast_new(NODE_PROGRAM, NULL, 0); }
;

/* CLASS */
classDecl:
      CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON
      {
          AST *c = ast_new(NODE_CLASS_DECL, $2, @2.first_line);
          if ($3) ast_append_child(c, $3);
          if ($5) ast_append_child(c, $5);
          $$ = c;
      }
;

classInherit:
      ISA ID moreIds
      {
          AST *list = ast_new(NODE_CLASS_INHERIT_LIST, NULL, @2.first_line);
          AST *idnode = ast_new(NODE_ID, $2, @2.first_line);
          ast_append_child(list, idnode);
          if ($3) ast_append_child(list, $3);
          $$ = list;
      }
    | /* empty */ { $$ = NULL; }
;

moreIds:
      COMMA ID moreIds
      {
          AST *idn = ast_new(NODE_ID, $2, @2.first_line);
          if ($3) ast_append_child(idn, $3);
          $$ = idn;
      }
    | /* empty */ { $$ = NULL; }
;

/* classBody */
classBody:
      PUBLIC attributeDecl classBody
      {
          if ($2 && $2->child) $2->child->typeName = strdup("PUBLIC");
          if ($3) ast_append_sibling(&$2, $3);
          $$ = $2;
      }
    | PRIVATE attributeDecl classBody
      {
          if ($2 && $2->child) $2->child->typeName = strdup("PRIVATE");
          if ($3) ast_append_sibling(&$2, $3);
          $$ = $2;
      }
    | /* empty */ { $$ = NULL; }
;

/* Implementation */
implDef:
      IMPLEMENT ID LBRACE implFuncs RBRACE
      {
          AST *n = ast_new(NODE_EMPTY, $2, @2.first_line);
          if ($4) n->child = $4;
          $$ = n;
      }
;

implFuncs:
      funcDef implFuncs { $$ = $1; if ($2) ast_append_sibling(&$$, $2); }
    | /* empty */ { $$ = NULL; }
;

/* Function definition */
funcDef:
      funcHead funcBody
      {
          AST *f = $1;
          if ($2) f->extra = $2;
          $$ = f;
      }
;

/* funcHead */
funcHead:
      FUNC ID LPAREN fParams RPAREN ARROW type
      {
          AST *fn = ast_new(NODE_FUNC_DECL, $2, @2.first_line);
          fn->typeName = $7 ? strdup($7->name) : NULL;
          if ($4) fn->child = $4;
          $$ = fn;
      }
    | CONSTRUCT LPAREN fParams RPAREN
      {
          AST *fn = ast_new(NODE_FUNC_DECL, "constructor", @1.first_line);
          if ($3) fn->child = $3;
          $$ = fn;
      }
;

/* function body */
funcBody:
      LBRACE varDeclOrStmtList RBRACE
      {
          AST *b = ast_new(NODE_FUNC_BODY, NULL, @1.first_line);
          if ($2) b->child = $2;
          $$ = b;
      }
;

varDeclOrStmtList:
      varDeclOrStmt varDeclOrStmtList
      {
          AST *head = $1;
          if ($2) ast_append_sibling(&head, $2);
          $$ = head;
      }
    | /* empty */ { $$ = NULL; }
;

varDeclOrStmt:
      localVarDecl { $$ = $1; }
    | statement    { $$ = $1; }
;

localVarDecl:
      LOCAL varDecl { $$ = $2; }
;

attributeDecl:
      ATTRIBUTE varDecl
      {
          AST *attr = ast_new(NODE_ATTRIBUTE, NULL, @1.first_line);
          if ($2) ast_append_child(attr, $2);
          $$ = attr;
      }
;

varDecl:
      ID COLON type arraySizes SEMICOLON
      {
          AST *v = ast_new(NODE_VAR_DECL, $1, @1.first_line);
          if ($3) v->typeName = $3->name ? strdup($3->name) : NULL;
          $$ = v;
      }
;

arraySizes:
      arraySize arraySizes { $$ = NULL; }
    | /* empty */ { $$ = NULL; }
;

arraySize:
      LBRACKET INT_LIT RBRACKET { $$ = ast_new_int($2, @2.first_line); }
    | LBRACKET RBRACKET { $$ = NULL; }
;

/* statements */
statement:
      assignStat SEMICOLON { $$ = $1; }
    | IF LPAREN expr RPAREN THEN statBlock ELSE statBlock
      {
          AST *node = ast_new(NODE_IF, NULL, @1.first_line);
          ast_append_child(node, $3);
          ast_append_child(node, $6);
          ast_append_child(node, $8);
          $$ = node;
      }
    | WHILE LPAREN expr RPAREN statBlock SEMICOLON
      {
          AST *node = ast_new(NODE_WHILE, NULL, @1.first_line);
          ast_append_child(node, $3);
          ast_append_child(node, $5);
          $$ = node;
      }
    | READ LPAREN variable RPAREN SEMICOLON
      {
          AST *n = ast_new(NODE_READ, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | WRITE LPAREN expr RPAREN SEMICOLON
      {
          AST *n = ast_new(NODE_WRITE, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | RETURN LPAREN expr RPAREN SEMICOLON
      {
          AST *n = ast_new(NODE_RETURN, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | functionCall SEMICOLON { $$ = $1; }
;

assignStat:
      variable ASSIGN expr
      {
          AST *assign = ast_new(NODE_ASSIGN, NULL, @1.first_line);
          ast_append_child(assign, $1);
          ast_append_child(assign, $3);
          $$ = assign;
      }
;

statementList:
      statement statementList { AST *h = $1; if ($2) ast_append_sibling(&h, $2); $$ = h; }
    | /* empty */ { $$ = NULL; }
;

statBlock:
      LBRACE statementList RBRACE { $$ = $2; }
    | statement { $$ = $1; }
    | /* empty */ { $$ = NULL; }
;

/* expressions - LL(1) right-recursive form */
expr:
      relExpr exprPrime
      {
          if ($2) {
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child ? op->child->sibling : NULL;
              op->child = left;
              if (right) op->child->sibling = right;
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

exprPrime:
      AND relExpr exprPrime
      {
          AST *op = ast_new(NODE_BINARY_OP, "and", @1.first_line);
          op->child = $2;
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
      }
    | OR relExpr exprPrime
      {
          AST *op = ast_new(NODE_BINARY_OP, "or", @1.first_line);
          op->child = $2;
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
      }
    | /* empty */ { $$ = NULL; }
;

relExpr:
      arithExpr EQ arithExpr { AST *n = ast_new(NODE_BINARY_OP, "==", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr NE arithExpr { AST *n = ast_new(NODE_BINARY_OP, "<>", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr LT arithExpr { AST *n = ast_new(NODE_BINARY_OP, "<", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr GT arithExpr { AST *n = ast_new(NODE_BINARY_OP, ">", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr LE arithExpr { AST *n = ast_new(NODE_BINARY_OP, "<=", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr GE arithExpr { AST *n = ast_new(NODE_BINARY_OP, ">=", @2.first_line); ast_append_child(n, $1); ast_append_child(n, $3); $$ = n; }
    | arithExpr               { $$ = $1; }
;

arithExpr:
      term arithExprPrime
      {
          if ($2) {
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child;
              op->child = left;
              if (right) op->child->sibling = right;
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

arithExprPrime:
      addOp term arithExprPrime
      {
          AST *op = ast_new(NODE_BINARY_OP, $1, @1.first_line);
          op->child = $2;
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
          free($1);
      }
    | /* empty */ { $$ = NULL; }
;

addOp:
      PLUS { $$ = strdup("+"); }
    | MINUS { $$ = strdup("-"); }
;

term:
      factor termPrime
      {
          if ($2) {
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child;
              op->child = left;
              if (right) op->child->sibling = right;
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

termPrime:
      multOp factor termPrime
      {
          AST *op = ast_new(NODE_BINARY_OP, $1, @1.first_line);
          op->child = $2;
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
          free($1);
      }
    | /* empty */ { $$ = NULL; }
;

multOp:
      MULT { $$ = strdup("*"); }
    | DIV { $$ = strdup("/"); }
    | AND { $$ = strdup("and"); }
;

factor:
      sign factor { AST *n = ast_new(NODE_UNARY_OP, $1, @1.first_line); ast_append_child(n, $2); $$ = n; free($1); }
    | NOT factor { AST *n = ast_new(NODE_UNARY_OP, "not", @1.first_line); ast_append_child(n, $2); $$ = n; }
    | variable { $$ = $1; }
    | INT_LIT   { $$ = ast_new(NODE_INT_LITERAL, NULL, @1.first_line); $$->intValue = $1; }
    | FLOAT_LIT { $$ = ast_new(NODE_FLOAT_LITERAL, NULL, @1.first_line); $$->floatValue = $1; }
    | STRING_LIT { $$ = ast_new(NODE_STRING_LITERAL, $1, @1.first_line); }
    | LPAREN arithExpr RPAREN { $$ = $2; }
    | functionCall { $$ = $1; }
;

sign:
      PLUS { $$ = strdup("+"); }
    | MINUS { $$ = strdup("-"); }
;

variable:
      idnestChain ID indiceChain
      {
          AST *var = ast_new(NODE_ID, $2, @2.first_line);
          if ($1) {
              AST *nested = $1;
              if (nested->child) {
                  AST *last = nested->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = var;
              } else {
                  nested->child = var;
              }
              var = nested;
          }
          if ($3) {
              AST *indices = $3;
              if (var->child) {
                  AST *last = var->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = indices;
              } else {
                  var->child = indices;
              }
          }
          $$ = var;
      }
;

idnestChain:
      idnest idnestChain
      {
          if ($2) {
              if ($2->child) {
                  AST *last = $2->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = $1;
              } else {
                  $2->child = $1;
              }
              $$ = $2;
          } else {
              AST *chain = ast_new(NODE_ID, NULL, @1.first_line);
              chain->child = $1;
              $$ = chain;
          }
      }
    | /* empty */ { $$ = NULL; }
;

idnest:
      idOrSelf indiceChain DOT
      {
          AST *access = $1;
          if ($2) {
              if (access->child) {
                  AST *last = access->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = $2;
              } else {
                  access->child = $2;
              }
          }
          $$ = access;
      }
    | idOrSelf LPAREN aParams RPAREN DOT
      {
          AST *call = ast_new(NODE_FUNCTION_CALL, $1->name, @1.first_line);
          call->child = $1;
          if ($3) {
              if (call->child->sibling) {
                  AST *last = call->child->sibling;
                  while (last->sibling) last = last->sibling;
                  last->sibling = $3;
              } else {
                  call->child->sibling = $3;
              }
          }
          $$ = call;
      }
;

idOrSelf:
      ID { $$ = ast_new(NODE_ID, $1, @1.first_line); }
    | SELF { $$ = ast_new(NODE_ID, "self", @1.first_line); }
;

indiceChain:
      indice indiceChain
      {
          if ($2) {
              AST *list = ast_new(NODE_ID, NULL, @1.first_line);
              list->child = $1;
              if ($2->child) {
                  AST *last = list->child;
                  while (last->sibling) last = last->sibling;
                  last->sibling = $2->child;
              }
              $$ = list;
          } else {
              AST *list = ast_new(NODE_ID, NULL, @1.first_line);
              list->child = $1;
              $$ = list;
          }
      }
    | /* empty */ { $$ = NULL; }
;

indice:
      LBRACKET arithExpr RBRACKET { $$ = $2; }
;

/* function call with nested access */
functionCall:
      idnestChain ID LPAREN aParams RPAREN
      {
          AST *c = ast_new(NODE_FUNCTION_CALL, $2, @2.first_line);
          if ($1) {
              AST *obj = $1->child;
              if (obj) {
                  c->child = obj;
                  if ($4) {
                      if (c->child->sibling) {
                          AST *last = c->child->sibling;
                          while (last->sibling) last = last->sibling;
                          last->sibling = $4;
                      } else {
                          c->child->sibling = $4;
                      }
                  }
              }
          } else {
              if ($4) c->child = $4;
          }
          $$ = c;
      }
;

/* parameter lists */
fParams:
      ID COLON type fParamsTailList
      {
          AST *p = ast_new(NODE_PARAM, $1, @1.first_line);
          if ($3) p->typeName = $3->name ? strdup($3->name) : NULL;
          if ($4) ast_append_sibling(&p, $4);
          $$ = p;
      }
    | /* empty */ { $$ = NULL; }
;

fParamsTailList:
      COMMA ID COLON type fParamsTailList
      {
          AST *p = ast_new(NODE_PARAM, $2, @2.first_line);
          if ($4) p->typeName = $4->name ? strdup($4->name) : NULL;
          if ($5) ast_append_sibling(&p, $5);
          $$ = p;
      }
    | /* empty */ { $$ = NULL; }
;

aParams:
      expr aParamsTailList { AST *h = $1; if ($2) ast_append_sibling(&h, $2); $$ = h; }
    | /* empty */ { $$ = NULL; }
;

aParamsTailList:
      COMMA expr aParamsTailList { AST *n = $2; if ($3) ast_append_sibling(&n, $3); $$ = n; }
    | /* empty */ { $$ = NULL; }
;

/* Normalize type names */
type:
      ID
      {
          const char *raw = $1;
          char buf[64] = {0};
          if (raw) {
              if (strcmp(raw, "int") == 0 || strcmp(raw, "integer") == 0) strncpy(buf, "integer", sizeof(buf)-1);
              else if (strcmp(raw, "float") == 0) strncpy(buf, "float", sizeof(buf)-1);
              else if (strcmp(raw, "void") == 0) strncpy(buf, "void", sizeof(buf)-1);
              else strncpy(buf, raw, sizeof(buf)-1);
          }
          AST *t = ast_new(NODE_TYPE, buf[0] ? strdup(buf) : NULL, @1.first_line);
          $$ = t;
      }
    | /* empty */ { $$ = NULL; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}

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
extern FILE *derivation_file;
static void log_production(const char *rule);

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
%token INTEGER_T FLOAT_T

%token COLON SEMICOLON COMMA DOT ASSIGN ARROW LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token PLUS MINUS MULT DIV AND OR NOT EQ NE LT GT LE GE

/* Precedence for operators */
%left OR
%left AND

/* Expect some conflicts due to left-recursive nested access in LALR(1) parser */
/* These conflicts are resolved correctly by Bison's default shift action */
%expect 7

/* nonterminals carry AST */
%type <node> statBlock statementList expr exprPrime relExpr arithExpr arithExprPrime term termPrime factor functionCall
%type <node> prog classOrImplOrFunc classDecl classInherit moreIds classBody memberDecl funcDecl implDef implFuncs
%type <node> funcDef funcHead funcBody varDeclOrStmtList varDeclOrStmt
%type <node> localVarDecl attributeDecl varDecl arraySizes arraySize statement assignStat
%type <node> variable idnest idOrSelf indice indiceList
%type <node> fParams fParamsTailList aParams aParamsTailList type returnType
%type <sVal> addOp multOp sign

%%

/* top-level - LL(1) right-recursive form: prog -> classOrImplOrFunc prog | epsilon */
prog:
      classOrImplOrFunc prog
      {
          log_production("prog -> classOrImplOrFunc prog");
          if (!astRoot) astRoot = ast_new(NODE_PROGRAM, NULL, @1.first_line);
          if ($1) ast_append_child(astRoot, $1);
      }
    | /* empty */
      {
          log_production("prog -> epsilon");
          astRoot = ast_new(NODE_PROGRAM, NULL, 0);
      }
;

/* classOrImplOrFunc -> classDecl | implDef | funcDef */
classOrImplOrFunc:
      classDecl
      {
          log_production("classOrImplOrFunc -> classDecl");
          $$ = $1;
      }
    | implDef
      {
          log_production("classOrImplOrFunc -> implDef");
          $$ = $1;
      }
    | funcDef
      {
          log_production("classOrImplOrFunc -> funcDef");
          $$ = $1;
      }
;

/* CLASS */
classDecl:
      CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON
      {
          log_production("classDecl -> CLASS id classInherit { classBody } ;");
          AST *c = ast_new(NODE_CLASS_DECL, $2, @2.first_line);
          if ($3) ast_append_child(c, $3);
          if ($5) ast_append_child(c, $5);
          $$ = c;
      }
;

classInherit:
      ISA ID moreIds
      {
          log_production("classInherit -> ISA id moreIds");
          AST *list = ast_new(NODE_CLASS_INHERIT_LIST, NULL, @2.first_line);
          AST *idnode = ast_new(NODE_ID, $2, @2.first_line);
          ast_append_child(list, idnode);
          if ($3) ast_append_child(list, $3);
          $$ = list;
      }
    | /* empty */
      {
          log_production("classInherit -> epsilon");
          $$ = NULL;
      }
;

moreIds:
      COMMA ID moreIds
      {
          log_production("moreIds -> , id moreIds");
          AST *idn = ast_new(NODE_ID, $2, @2.first_line);
          if ($3) ast_append_child(idn, $3);
          $$ = idn;
      }
    | /* empty */
      {
          log_production("moreIds -> epsilon");
          $$ = NULL;
      }
;

/* classBody - LL(1) right-recursive: classBody -> visibility memberDecl classBody | epsilon */
classBody:
      PUBLIC memberDecl classBody
      {
          log_production("classBody -> PUBLIC memberDecl classBody");
          if ($2) {
              if ($2->typeName) free($2->typeName);
              $2->typeName = strdup("public");
          }
          if ($3) ast_append_sibling(&$2, $3);
          $$ = $2;
      }
    | PRIVATE memberDecl classBody
      {
          log_production("classBody -> PRIVATE memberDecl classBody");
          if ($2) {
              if ($2->typeName) free($2->typeName);
              $2->typeName = strdup("private");
          }
          if ($3) ast_append_sibling(&$2, $3);
          $$ = $2;
      }
    | /* empty */
      {
          log_production("classBody -> epsilon");
          $$ = NULL;
      }
;

/* memberDecl -> funcDecl | attributeDecl */
memberDecl:
      funcDecl
      {
          log_production("memberDecl -> funcDecl");
          $$ = $1;
      }
    | attributeDecl
      {
          log_production("memberDecl -> attributeDecl");
          $$ = $1;
      }
;

/* funcDecl -> funcHead ; */
funcDecl:
      funcHead SEMICOLON
      {
          log_production("funcDecl -> funcHead ;");
          $$ = $1;
      }
;

/* Implementation */
implDef:
      IMPLEMENT ID LBRACE implFuncs RBRACE
      {
          log_production("implDef -> IMPLEMENT id { implFuncs }");
          AST *n = ast_new(NODE_EMPTY, $2, @2.first_line);
          if ($4) n->child = $4;
          $$ = n;
      }
;

implFuncs:
      funcDef implFuncs
      {
          log_production("implFuncs -> funcDef implFuncs");
          $$ = $1;
          if ($2) ast_append_sibling(&$$, $2);
      }
    | /* empty */
      {
          log_production("implFuncs -> epsilon");
          $$ = NULL;
      }
;

/* Function definition */
funcDef:
      funcHead funcBody
      {
          log_production("funcDef -> funcHead funcBody");
          AST *f = $1;
          if ($2) f->extra = $2;
          $$ = f;
      }
;

/* funcHead */
funcHead:
      FUNC ID LPAREN fParams RPAREN ARROW returnType
      {
          log_production("funcHead -> FUNC id ( fParams ) ARROW returnType");
          AST *fn = ast_new(NODE_FUNC_DECL, $2, @2.first_line);
          if ($7) {
              fn->typeName = $7->name ? strdup($7->name) : NULL;
              ast_free($7);
          }
          if ($4) fn->child = $4;
          $$ = fn;
      }
    | CONSTRUCT LPAREN fParams RPAREN
      {
          log_production("funcHead -> CONSTRUCT ( fParams )");
          AST *fn = ast_new(NODE_FUNC_DECL, "constructor", @1.first_line);
          if ($3) fn->child = $3;
          $$ = fn;
      }
;

/* function body */
funcBody:
      LBRACE varDeclOrStmtList RBRACE
      {
          log_production("funcBody -> { varDeclOrStmtList }");
          AST *b = ast_new(NODE_FUNC_BODY, NULL, @1.first_line);
          if ($2) b->child = $2;
          $$ = b;
      }
;

varDeclOrStmtList:
      varDeclOrStmt varDeclOrStmtList
      {
          log_production("varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList");
          AST *head = $1;
          if ($2) ast_append_sibling(&head, $2);
          $$ = head;
      }
    | /* empty */
      {
          log_production("varDeclOrStmtList -> epsilon");
          $$ = NULL;
      }
;

varDeclOrStmt:
      localVarDecl
      {
          log_production("varDeclOrStmt -> localVarDecl");
          $$ = $1;
      }
    | statement
      {
          log_production("varDeclOrStmt -> statement");
          $$ = $1;
      }
;

localVarDecl:
      LOCAL varDecl
      {
          log_production("localVarDecl -> LOCAL varDecl");
          $$ = $2;
      }
;

attributeDecl:
      ATTRIBUTE varDecl
      {
          log_production("attributeDecl -> ATTRIBUTE varDecl");
          AST *attr = ast_new(NODE_ATTRIBUTE, NULL, @1.first_line);
          if ($2) ast_append_child(attr, $2);
          $$ = attr;
      }
;

varDecl:
      ID COLON type arraySizes SEMICOLON
      {
          log_production("varDecl -> id : type arraySizes ;");
          AST *v = ast_new(NODE_VAR_DECL, $1, @1.first_line);
          if ($3) {
              v->typeName = $3->name ? strdup($3->name) : NULL;
              ast_free($3);
          }
          $$ = v;
      }
;

arraySizes:
      arraySize arraySizes
      {
          log_production("arraySizes -> arraySize arraySizes");
          $$ = NULL;
      }
    | /* empty */
      {
          log_production("arraySizes -> epsilon");
          $$ = NULL;
      }
;

arraySize:
      LBRACKET INT_LIT RBRACKET
      {
          log_production("arraySize -> [ INT ]");
          $$ = ast_new_int($2, @2.first_line);
      }
    | LBRACKET RBRACKET
      {
          log_production("arraySize -> [ ]");
          $$ = NULL;
      }
;

/* statements */
statement:
      assignStat SEMICOLON
      {
          log_production("statement -> assignStat ;");
          $$ = $1;
      }
    | IF LPAREN expr RPAREN THEN statBlock ELSE statBlock SEMICOLON
      {
          log_production("statement -> IF ( expr ) THEN statBlock ELSE statBlock ;");
          AST *node = ast_new(NODE_IF, NULL, @1.first_line);
          ast_append_child(node, $3);
          ast_append_child(node, $6);
          ast_append_child(node, $8);
          $$ = node;
      }
    | IF LPAREN expr RPAREN THEN statBlock SEMICOLON
      {
          log_production("statement -> IF ( expr ) THEN statBlock ;");
          AST *node = ast_new(NODE_IF, NULL, @1.first_line);
          ast_append_child(node, $3);
          ast_append_child(node, $6);
          /* else block is NULL */
          $$ = node;
      }
    | WHILE LPAREN expr RPAREN statBlock SEMICOLON
      {
          log_production("statement -> WHILE ( expr ) statBlock ;");
          AST *node = ast_new(NODE_WHILE, NULL, @1.first_line);
          ast_append_child(node, $3);
          ast_append_child(node, $5);
          $$ = node;
      }
    | READ LPAREN variable RPAREN SEMICOLON
      {
          log_production("statement -> READ ( variable ) ;");
          AST *n = ast_new(NODE_READ, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | WRITE LPAREN expr RPAREN SEMICOLON
      {
          log_production("statement -> WRITE ( expr ) ;");
          AST *n = ast_new(NODE_WRITE, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | RETURN LPAREN expr RPAREN SEMICOLON
      {
          log_production("statement -> RETURN ( expr ) ;");
          AST *n = ast_new(NODE_RETURN, NULL, @1.first_line);
          ast_append_child(n, $3);
          $$ = n;
      }
    | functionCall SEMICOLON
      {
          log_production("statement -> functionCall ;");
          $$ = $1;
      }
    | error SEMICOLON
      {
          log_production("statement -> error ;");
          yyerror("Recovering from statement error");
          yyerrok;
          $$ = NULL;
      }
;

assignStat:
      variable ASSIGN expr
      {
          log_production("assignStat -> variable ASSIGN expr");
          AST *assign = ast_new(NODE_ASSIGN, NULL, @1.first_line);
          ast_append_child(assign, $1);
          ast_append_child(assign, $3);
          $$ = assign;
      }
;

statementList:
      statement statementList
      {
          log_production("statementList -> statement statementList");
          AST *h = $1;
          if ($2) ast_append_sibling(&h, $2);
          $$ = h;
      }
    | /* empty */
      {
          log_production("statementList -> epsilon");
          $$ = NULL;
      }
;

statBlock:
      LBRACE statementList RBRACE
      {
          log_production("statBlock -> { statementList }");
          $$ = $2;
      }
    | statement
      {
          log_production("statBlock -> statement");
          $$ = $1;
      }
    | /* empty */
      {
          log_production("statBlock -> epsilon");
          $$ = NULL;
      }
;

/* expressions - LL(1) right-recursive form */
expr:
      relExpr exprPrime
      {
          log_production("expr -> relExpr exprPrime");
          if ($2) {
              /* Build left-associative tree from right-recursive parse */
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child ? op->child->sibling : NULL;
              op->child = left;
              if (right) {
                  op->child->sibling = right;
              }
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

exprPrime:
      AND relExpr exprPrime
      {
          log_production("exprPrime -> AND relExpr exprPrime");
          AST *op = ast_new(NODE_BINARY_OP, "and", @1.first_line);
          op->child = $2;  /* right operand */
          if ($3) {
              /* Chain: (left AND right) AND next */
              AST *chain = $3;
              chain->child->sibling = op;  /* attach to chain */
              $$ = chain;
          } else {
              $$ = op;
          }
      }
    | OR relExpr exprPrime
      {
          log_production("exprPrime -> OR relExpr exprPrime");
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
    | /* empty */
      {
          log_production("exprPrime -> epsilon");
          $$ = NULL;
      }
;

relExpr:
      arithExpr EQ arithExpr
      {
          log_production("relExpr -> arithExpr == arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "==", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr NE arithExpr
      {
          log_production("relExpr -> arithExpr <> arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<>", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr LT arithExpr
      {
          log_production("relExpr -> arithExpr < arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr GT arithExpr
      {
          log_production("relExpr -> arithExpr > arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, ">", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr LE arithExpr
      {
          log_production("relExpr -> arithExpr <= arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, "<=", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr GE arithExpr
      {
          log_production("relExpr -> arithExpr >= arithExpr");
          AST *n = ast_new(NODE_BINARY_OP, ">=", @2.first_line);
          ast_append_child(n, $1);
          ast_append_child(n, $3);
          $$ = n;
      }
    | arithExpr
      {
          log_production("relExpr -> arithExpr");
          $$ = $1;
      }
;

arithExpr:
      term arithExprPrime
      {
          log_production("arithExpr -> term arithExprPrime");
          if ($2) {
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child;
              op->child = left;
              if (right) {
                  op->child->sibling = right;
              }
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

arithExprPrime:
      addOp term arithExprPrime
      {
          log_production("arithExprPrime -> addOp term arithExprPrime");
          AST *op = ast_new(NODE_BINARY_OP, $1, @1.first_line);
          op->child = $2;  /* right operand (term) */
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
          free($1);
      }
    | /* empty */
      {
          log_production("arithExprPrime -> epsilon");
          $$ = NULL;
      }
;

addOp:
      PLUS
      {
          log_production("addOp -> +");
          $$ = strdup("+");
      }
    | MINUS
      {
          log_production("addOp -> -");
          $$ = strdup("-");
      }
    | OR
      {
          log_production("addOp -> or");
          $$ = strdup("or");
      }
;

term:
      factor termPrime
      {
          log_production("term -> factor termPrime");
          if ($2) {
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child;
              op->child = left;
              if (right) {
                  op->child->sibling = right;
              }
              $$ = op;
          } else {
              $$ = $1;
          }
      }
;

termPrime:
      multOp factor termPrime
      {
          log_production("termPrime -> multOp factor termPrime");
          AST *op = ast_new(NODE_BINARY_OP, $1, @1.first_line);
          op->child = $2;  /* right operand (factor) */
          if ($3) {
              AST *chain = $3;
              chain->child->sibling = op;
              $$ = chain;
          } else {
              $$ = op;
          }
          free($1);
      }
    | /* empty */
      {
          log_production("termPrime -> epsilon");
          $$ = NULL;
      }
;

multOp:
      MULT
      {
          log_production("multOp -> *");
          $$ = strdup("*");
      }
    | DIV
      {
          log_production("multOp -> /");
          $$ = strdup("/");
      }
    | AND
      {
          log_production("multOp -> and");
          $$ = strdup("and");
      }
;

factor:
      variable
      {
          log_production("factor -> variable");
          $$ = $1;
      }
    | functionCall
      {
          log_production("factor -> functionCall");
          $$ = $1;
      }
    | INT_LIT
      {
          log_production("factor -> INT_LIT");
          AST *n = ast_new(NODE_INT_LITERAL, NULL, @1.first_line);
          n->intValue = $1;
          $$ = n;
      }
    | FLOAT_LIT
      {
          log_production("factor -> FLOAT_LIT");
          AST *n = ast_new(NODE_FLOAT_LITERAL, NULL, @1.first_line);
          n->floatValue = $1;
          $$ = n;
      }
    | LPAREN arithExpr RPAREN
      {
          log_production("factor -> ( arithExpr )");
          $$ = $2;
      }
    | NOT factor
      {
          log_production("factor -> NOT factor");
          AST *n = ast_new(NODE_UNARY_OP, "not", @1.first_line);
          ast_append_child(n, $2);
          $$ = n;
      }
    | sign factor
      {
          log_production("factor -> sign factor");
          AST *n = ast_new(NODE_UNARY_OP, $1, @1.first_line);
          ast_append_child(n, $2);
          free($1);
          $$ = n;
      }
;

/* sign -> + | - */
sign:
      PLUS
      {
          log_production("sign -> +");
          $$ = strdup("+");
      }
    | MINUS
      {
          log_production("sign -> -");
          $$ = strdup("-");
      }
;

/* function call - functionCall -> idnestList id ( aParams ) */
/* Split into explicit cases to avoid ambiguity */
functionCall:
      ID LPAREN aParams RPAREN
      {
          log_production("functionCall -> id ( aParams )");
          AST *c = ast_new(NODE_FUNCTION_CALL, $1, @1.first_line);
          if ($3) c->child = $3;
          $$ = c;
      }
    | idnest DOT functionCall
      {
          log_production("functionCall -> idnest . functionCall");
          AST *c = $3;
          if ($1) {
              if (c->child) {
                  AST *last = $1;
                  while (last->sibling) last = last->sibling;
                  last->sibling = c->child;
                  c->child = $1;
              } else {
                  c->child = $1;
              }
          }
          $$ = c;
      }
;

/* variable -> idnestList id indiceList */
/* Split into explicit cases to avoid ambiguity */
variable:
      ID indiceList
      {
          log_production("variable -> id indiceList");
          AST *var = ast_new(NODE_ID, $1, @1.first_line);
          if ($2) var->sibling = $2;
          $$ = var;
      }
    | idnest DOT variable
      {
          log_production("variable -> idnest . variable");
          AST *var = $3;
          if ($1) {
              AST *last = $1;
              while (last->sibling) last = last->sibling;
              last->sibling = var;
              var = $1;
          }
          $$ = var;
      }
;



/* idnest -> idOrSelf indiceList . | idOrSelf ( aParams ) . */
idnest:
      idOrSelf indiceList DOT
      {
          log_production("idnest -> idOrSelf indiceList .");
          AST *n = $1;
          if ($2) {
              if (n->sibling) {
                  AST *last = n;
                  while (last->sibling) last = last->sibling;
                  last->sibling = $2;
              } else {
                  n->sibling = $2;
              }
          }
          $$ = n;
      }
    | idOrSelf LPAREN aParams RPAREN DOT
      {
          log_production("idnest -> idOrSelf ( aParams ) .");
          AST *call = ast_new(NODE_FUNCTION_CALL, $1->name, @1.first_line);
          call->child = $3;
          $$ = call;
      }
;

/* idOrSelf -> id | self */
idOrSelf:
      ID
      {
          log_production("idOrSelf -> id");
          $$ = ast_new(NODE_ID, $1, @1.first_line);
      }
    | SELF
      {
          log_production("idOrSelf -> self");
          $$ = ast_new(NODE_ID, "self", @1.first_line);
      }
;

/* indiceList -> indice indiceList | epsilon */
indiceList:
      indice indiceList
      {
          log_production("indiceList -> indice indiceList");
          AST *head = $1;
          if ($2) ast_append_sibling(&head, $2);
          $$ = head;
      }
    | /* empty */
      {
          log_production("indiceList -> epsilon");
          $$ = NULL;
      }
;

/* indice -> [ arithExpr ] */
indice:
      LBRACKET arithExpr RBRACKET
      {
          log_production("indice -> [ arithExpr ]");
          AST *idx = ast_new(NODE_BINARY_OP, "[]", @1.first_line);
          ast_append_child(idx, $2);
          $$ = idx;
      }
;

/* parameter lists */
fParams:
      ID COLON type arraySizes fParamsTailList
      {
          log_production("fParams -> id : type arraySizes fParamsTailList");
          AST *p = ast_new(NODE_PARAM, $1, @1.first_line);
          if ($3) {
              p->typeName = $3->name ? strdup($3->name) : NULL;
              ast_free($3);
          }
          if ($5) ast_append_sibling(&p, $5);
          $$ = p;
      }
    | /* empty */
      {
          log_production("fParams -> epsilon");
          $$ = NULL;
      }
;

fParamsTailList:
      COMMA ID COLON type arraySizes fParamsTailList
      {
          log_production("fParamsTailList -> , id : type arraySizes fParamsTailList");
          AST *p = ast_new(NODE_PARAM, $2, @2.first_line);
          if ($4) {
              p->typeName = $4->name ? strdup($4->name) : NULL;
              ast_free($4);
          }
          if ($6) ast_append_sibling(&p, $6);
          $$ = p;
      }
    | /* empty */
      {
          log_production("fParamsTailList -> epsilon");
          $$ = NULL;
      }
;

aParams:
      expr aParamsTailList
      {
          log_production("aParams -> expr aParamsTailList");
          AST *h = $1;
          if ($2) ast_append_sibling(&h, $2);
          $$ = h;
      }
    | /* empty */
      {
          log_production("aParams -> epsilon");
          $$ = NULL;
      }
;

aParamsTailList:
      COMMA expr aParamsTailList
      {
          log_production("aParamsTailList -> , expr aParamsTailList");
          AST *n = $2;
          if ($3) ast_append_sibling(&n, $3);
          $$ = n;
      }
    | /* empty */
      {
          log_production("aParamsTailList -> epsilon");
          $$ = NULL;
      }
;

/* Normalize type names */
type:
      INTEGER_T
      {
          log_production("type -> INTEGER");
          AST *t = ast_new(NODE_TYPE, "int", @1.first_line);
          $$ = t;
      }
    | FLOAT_T
      {
          log_production("type -> FLOAT");
          AST *t = ast_new(NODE_TYPE, "float", @1.first_line);
          $$ = t;
      }
    | ID
      {
          log_production("type -> id");
          AST *t = ast_new(NODE_TYPE, $1, @1.first_line);
          $$ = t;
      }
    | /* empty */
      {
          log_production("type -> epsilon");
          $$ = NULL;
      }
;

returnType:
      type
      {
          log_production("returnType -> type");
          $$ = $1;
      }
    | VOID
      {
          log_production("returnType -> VOID");
          AST *t = ast_new(NODE_TYPE, "void", @1.first_line);
          $$ = t;
      }
;

%%

static void log_production(const char *rule) {
    if (derivation_file && rule) {
        fprintf(derivation_file, "%s\n", rule);
        fflush(derivation_file);
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}

%{
#include <stdio.h>
#include <stdlib.h>

FILE *derivation;
extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;
%}

%union {
    int iVal;
    double dVal;
    char* sVal;
}

/* Tokens with semantic values */
%token <sVal> ID
%token <iVal> INT_LIT
%token <dVal> FLOAT_LIT

/* Keywords */
%token CLASS IMPLEMENT FUNC CONSTRUCT ATTRIBUTE PUBLIC PRIVATE RETURN READ WRITE IF ELSE WHILE VOID SELF
%token ISA LOCAL THEN

/* Operators and punctuation */
%token COLON SEMICOLON COMMA DOT ASSIGN ARROW LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token PLUS MINUS MULT DIV AND OR NOT EQ NE LT GT LE GE

%%

prog:
      classOrImplOrFunc prog
    | /* empty */
    {
        fprintf(derivation, "prog -> epsilon\n");
    }
;

classOrImplOrFunc:
      classDecl    { fprintf(derivation, "classOrImplOrFunc -> classDecl\n"); }
    | implDef      { fprintf(derivation, "classOrImplOrFunc -> implDef\n"); }
    | funcDef      { fprintf(derivation, "classOrImplOrFunc -> funcDef\n"); }
;

classDecl:
      CLASS ID classInherit LBRACE classBody RBRACE SEMICOLON
      { fprintf(derivation, "classDecl -> CLASS ID classInherit { classBody } ;\n"); }
;

classInherit:
      ISA ID moreIds
      { fprintf(derivation, "classInherit -> ISA ID moreIds\n"); }
    | /* empty */
      { fprintf(derivation, "classInherit -> epsilon\n"); }
;

moreIds:
      COMMA ID moreIds
      { fprintf(derivation, "moreIds -> , ID moreIds\n"); }
    | /* empty */
      { fprintf(derivation, "moreIds -> epsilon\n"); }
;

classBody:
      visibility memberDecl classBody
      { fprintf(derivation, "classBody -> visibility memberDecl classBody\n"); }
    | /* empty */
      { fprintf(derivation, "classBody -> epsilon\n"); }
;

implDef:
      IMPLEMENT ID LBRACE implFuncs RBRACE
      { fprintf(derivation, "implDef -> IMPLEMENT ID { implFuncs }\n"); }
;

implFuncs:
      funcDef implFuncs
      { fprintf(derivation, "implFuncs -> funcDef implFuncs\n"); }
    | /* empty */
      { fprintf(derivation, "implFuncs -> epsilon\n"); }
;

funcDef:
      funcHead funcBody
      { fprintf(derivation, "funcDef -> funcHead funcBody\n"); }
;

funcHead:
      FUNC ID LPAREN fParams RPAREN ARROW returnType
      { fprintf(derivation, "funcHead -> FUNC ID ( fParams ) => returnType\n"); }
    | CONSTRUCT LPAREN fParams RPAREN
      { fprintf(derivation, "funcHead -> CONSTRUCT ( fParams )\n"); }
;

funcBody:
      LBRACE varDeclOrStmtList RBRACE
      { fprintf(derivation, "funcBody -> { varDeclOrStmtList }\n"); }
;

varDeclOrStmtList:
      varDeclOrStmt varDeclOrStmtList
      { fprintf(derivation, "varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList\n"); }
    | /* empty */
      { fprintf(derivation, "varDeclOrStmtList -> epsilon\n"); }
;

varDeclOrStmt:
      localVarDecl     { fprintf(derivation, "varDeclOrStmt -> localVarDecl\n"); }
    | statement        { fprintf(derivation, "varDeclOrStmt -> statement\n"); }
;

localVarDecl:
      LOCAL varDecl
      { fprintf(derivation, "localVarDecl -> LOCAL varDecl\n"); }
;

attributeDecl:
      ATTRIBUTE varDecl
      { fprintf(derivation, "attributeDecl -> ATTRIBUTE varDecl\n"); }
;

varDecl:
      ID COLON type arraySizes SEMICOLON
      { fprintf(derivation, "varDecl -> ID : type arraySizes ;\n"); }
;

arraySizes:
      arraySize arraySizes
      { fprintf(derivation, "arraySizes -> arraySize arraySizes\n"); }
    | /* empty */
      { fprintf(derivation, "arraySizes -> epsilon\n"); }
;

arraySize:
      LBRACKET INT_LIT RBRACKET
      { fprintf(derivation, "arraySize -> [ INT_LIT ]\n"); }
    | LBRACKET RBRACKET
      { fprintf(derivation, "arraySize -> [ ]\n"); }
;

statement:
      assignStat SEMICOLON
      { fprintf(derivation, "statement -> assignStat ;\n"); }
    | IF LPAREN relExpr RPAREN THEN statBlock ELSE statBlock
      { fprintf(derivation, "statement -> IF ( relExpr ) THEN statBlock ELSE statBlock\n"); }
    | WHILE LPAREN relExpr RPAREN statBlock SEMICOLON
      { fprintf(derivation, "statement -> WHILE ( relExpr ) statBlock ;\n"); }
    | READ LPAREN ID RPAREN SEMICOLON
      { fprintf(derivation, "statement -> READ ( ID ) ;\n"); }
    | WRITE LPAREN expr RPAREN SEMICOLON
      { fprintf(derivation, "statement -> WRITE ( expr ) ;\n"); }
    | RETURN LPAREN expr RPAREN SEMICOLON
      { fprintf(derivation, "statement -> RETURN ( expr ) ;\n"); }
    | functionCall SEMICOLON
      { fprintf(derivation, "statement -> functionCall ;\n"); }
;

assignStat:
      ID ASSIGN expr
      { fprintf(derivation, "assignStat -> ID ASSIGN expr\n"); }
;

statBlock:
      LBRACE statementList RBRACE
      { fprintf(derivation, "statBlock -> { statementList }\n"); }
    | statement
      { fprintf(derivation, "statBlock -> statement\n"); }
    | /* empty */
      { fprintf(derivation, "statBlock -> epsilon\n"); }
;

statementList:
      statement statementList
      { fprintf(derivation, "statementList -> statement statementList\n"); }
    | /* empty */
      { fprintf(derivation, "statementList -> epsilon\n"); }
;

expr:
      relExpr     { fprintf(derivation, "expr -> relExpr\n"); }
    | arithExpr  { fprintf(derivation, "expr -> arithExpr\n"); }
;

relExpr:
      arithExpr relOp arithExpr
      { fprintf(derivation, "relExpr -> arithExpr relOp arithExpr\n"); }
;

relOp:
      EQ | NE | LT | GT | LE | GE
      { fprintf(derivation, "relOp -> operator\n"); }
;

arithExpr:
      term arithExprPrime
      { fprintf(derivation, "arithExpr -> term arithExprPrime\n"); }
;

arithExprPrime:
      addOp term arithExprPrime
      { fprintf(derivation, "arithExprPrime -> addOp term arithExprPrime\n"); }
    | /* empty */
      { fprintf(derivation, "arithExprPrime -> epsilon\n"); }
;

term:
      factor termPrime
      { fprintf(derivation, "term -> factor termPrime\n"); }
;

termPrime:
      multOp factor termPrime
      { fprintf(derivation, "termPrime -> multOp factor termPrime\n"); }
    | /* empty */
      { fprintf(derivation, "termPrime -> epsilon\n"); }
;

multOp:
      MULT | DIV | AND
      { fprintf(derivation, "multOp -> operator\n"); }
;

addOp:
      PLUS | MINUS | OR
      { fprintf(derivation, "addOp -> operator\n"); }
;

factor:
      ID
    | FLOAT_LIT
    | INT_LIT
    | LPAREN arithExpr RPAREN
    | NOT factor
    | PLUS factor
    | MINUS factor
    { fprintf(derivation, "factor -> ...\n"); }
;

type:
      ID | CLASS | VOID
      { fprintf(derivation, "type -> ...\n"); }
;

fParams:
      ID COLON type arraySizes fParamsTailList
      { fprintf(derivation, "fParams -> ID : type arraySizes fParamsTailList\n"); }
    | /* empty */
      { fprintf(derivation, "fParams -> epsilon\n"); }
;

fParamsTailList:
      COMMA ID COLON type arraySizes fParamsTailList
      { fprintf(derivation, "fParamsTailList -> , ID : type arraySizes fParamsTailList\n"); }
    | /* empty */
      { fprintf(derivation, "fParamsTailList -> epsilon\n"); }
;

aParams:
      expr aParamsTailList
      { fprintf(derivation, "aParams -> expr aParamsTailList\n"); }
    | /* empty */
      { fprintf(derivation, "aParams -> epsilon\n"); }
;

aParamsTailList:
      COMMA expr aParamsTailList
      { fprintf(derivation, "aParamsTailList -> , expr aParamsTailList\n"); }
    | /* empty */
      { fprintf(derivation, "aParamsTailList -> epsilon\n"); }
;

functionCall:
      ID LPAREN aParams RPAREN
      { fprintf(derivation, "functionCall -> ID ( aParams )\n"); }
;

/* Visibility can be PUBLIC or PRIVATE */
visibility:
      PUBLIC   { fprintf(derivation, "visibility -> PUBLIC\n"); }
    | PRIVATE  { fprintf(derivation, "visibility -> PRIVATE\n"); }
;

/* Member declarations inside class: either attribute or function */
memberDecl:
      attributeDecl  { fprintf(derivation, "memberDecl -> attributeDecl\n"); }
    | funcDef        { fprintf(derivation, "memberDecl -> funcDef\n"); }
;

/* Return type in function: can be a type like ID, CLASS, or VOID */
returnType:
      type           { fprintf(derivation, "returnType -> type\n"); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }

    derivation = fopen("derivation.txt", "w");
    if (!derivation) {
        fprintf(stderr, "Failed to open derivation file.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Failed to open source file: %s\n", argv[1]);
        return 1;
    }
    yyin = inputFile;

    yyparse();

    fclose(inputFile);
    fclose(derivation);
    return 0;
}

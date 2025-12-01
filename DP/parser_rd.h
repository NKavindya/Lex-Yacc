#ifndef PARSER_RD_H
#define PARSER_RD_H

#include <stdio.h>
#include "common.h"

/* Token definitions matching lexer.l */
#define YYEOF 0
#define ERROR_TOKEN 256

/* Literals */
#define INTEGER_CONSTANT 257
#define FLOATING_POINT_CONSTANT 258
#define IDENTIFIER 259

/* Reserved words */
#define ELSE_TOKEN 260
#define INTEGER_TOKEN 261
#define SELF_TOKEN 262
#define FLOAT_TOKEN 263
#define ISA_TOKEN 264
#define CONSTRUCT_TOKEN 265
#define FUNC_TOKEN 266
#define PRIVATE_TOKEN 267
#define THEN_TOKEN 268
#define IF_TOKEN 269
#define PUBLIC_TOKEN 270
#define LOCAL_TOKEN 271
#define IMPLEMENT_TOKEN 272
#define READ_TOKEN 273
#define VOID_TOKEN 274
#define AND_TOKEN 275
#define CLASS_TOKEN 276
#define OR_TOKEN 277
#define RETURN_TOKEN 278
#define WHILE_TOKEN 279
#define ATTRIBUTE_TOKEN 280
#define WRITE_TOKEN 281
#define NOT_TOKEN 282

/* Operators */
#define ASSIGN_OP 283
#define EQ_OP 284
#define NE_OP 285
#define LE_OP 286
#define GE_OP 287
#define ARROW 288
#define PLUS_OP 289
#define MINUS_OP 290
#define MUL_OP 291
#define DIV_OP 292
#define LT_OP 293
#define GT_OP 294

/* Punctuation */
#define LPAREN 295
#define RPAREN 296
#define LBRACE 297
#define RBRACE 298
#define LBRACKET 299
#define RBRACKET 300
#define SEMICOLON 301
#define COMMA 302
#define DOT 303
#define COLON 304

/* Parser state */
extern Token current_token;
extern FILE *derivation_file;
extern int syntax_errors;

/* Lexer interface */
extern int yylex(void);
extern FILE *yyin;
extern int yylineno;
extern Token get_last_token(void);

/* Parser functions */
void parse_prog(void);
void parse_classOrImplOrFunc(void);
void parse_classDecl(void);
void parse_implDef(void);
void parse_funcDef(void);
void parse_visibility(void);
void parse_memberDecl(void);
void parse_funcDecl(void);
void parse_funcHead(void);
void parse_funcBody(void);
void parse_localVarDeclOrStmt(void);
void parse_attributeDecl(void);
void parse_localVarDecl(void);
void parse_varDecl(void);
void parse_statement(void);
void parse_assignStat(void);
void parse_statBlock(void);
void parse_expr(void);
void parse_relExpr(void);
void parse_arithExpr(void);
void parse_arithExprPrime(void);
void parse_sign(void);
void parse_term(void);
void parse_termPrime(void);
void parse_factor(void);
void parse_variable(void);
void parse_functionCall(void);
void parse_idnest(void);
void parse_idnestList(void);
void parse_idOrSelf(void);
void parse_indice(void);
void parse_indiceList(void);
void parse_arraySize(void);
void parse_arraySizeList(void);
void parse_type(void);
void parse_returnType(void);
void parse_fParams(void);
void parse_fParamsTail(void);
void parse_fParamsTailList(void);
void parse_aParams(void);
void parse_aParamsTail(void);
void parse_aParamsTailList(void);
void parse_assignOp(void);
void parse_relOp(void);
void parse_addOp(void);
void parse_multOp(void);

/* Helper functions */
void match(int expected_token);
void write_derivation(const char *non_terminal, const char *production);
void syntax_error(const char *message);
void skip_to_sync_point(void);

#endif /* PARSER_RD_H */


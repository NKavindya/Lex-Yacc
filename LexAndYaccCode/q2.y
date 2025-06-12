%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare external lexer function and error handler
int yylex(void);
void yyerror(const char *s);
extern int lineno;    // Line number tracking (defined in lexer)
extern int yychar;    // Current token being processed

int test_num = 1;     // Counter for test cases
%}

// Data Type Union  //
%union {
    char* str;        // For IDs, literals, etc.
}


// Token Declarations   //
%token <str> ID INTLIT FLOATLIT

// Reserved words
%token IF ELSE WHILE RETURN READ WRITE CONSTRUCTOR
%token FUNC IMPLEMENT CLASS
%token PUBLIC PRIVATE LOCAL
%token ATTRIBUTE
%token SELF ISA
%token INTEGER FLOAT VOID

// Operators
%token PLUS MINUS MULT DIV EQ NEQ LT GT LTE GTE ASSIGN

// Punctuation
%token SEMICOLON COMMA COLON LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET ARROW DOT

// Logical operators
%token OR AND NOT THEN

%token DELIM // Test case delimiter

// Operator Precedence
%left OR
%left AND
%right NOT
%left EQ NEQ LT GT LTE GTE
%left PLUS MINUS
%left MULT DIV

// Grammar Rules     //

%%
// Entry Point       //
prog:
    program_elements
    | tests
    ;


// Automated Tests   //
tests:
    tests test
    | test
    ;

test:
    test_unit DELIM {
        printf(">>> Test %d passed \n", test_num++);
    }
    | error DELIM {
        printf(">>> Test %d failed (syntax error at line %d)\n", test_num++, lineno);
        yyerrok; // Reset parser after error
    }
    ;

test_unit:
    stmts                 // Sequence of statements
    | classOrImplOrFunc   // Class, implementation, or function definition
    ;

// Top-Level Declarations //
program_elements:
    /* empty */
    | program_elements classOrImplOrFunc
    ;

classOrImplOrFunc:
    classDecl
    | implDef
    | funcDef
    ;

// Class Definition
classDecl:
    CLASS ID optional_isa LBRACE memberDecl_list RBRACE SEMICOLON
    ;

optional_isa:
    /* empty */
    | ISA ID_list
    ;

ID_list:
    ID
    | ID_list COMMA ID
    ;

// Implementation Block
implDef:
    IMPLEMENT ID LBRACE funcDef_list RBRACE
    ;

funcDef_list:
    /* empty */
    | funcDef_list funcDef
    ;

// Function Definition
funcDef:
    funcHead funcBody
    ;

funcHead:
    FUNC ID LPAREN fParams RPAREN ARROW returnType
    | CONSTRUCTOR LPAREN fParams RPAREN
    ;

funcBody:
    LBRACE localVarDeclOrStmt_list RBRACE
    ;

localVarDeclOrStmt_list:
    /* empty */
    | localVarDeclOrStmt_list localVarDeclOrStmt
    ;

localVarDeclOrStmt:
    localVarDecl
    | stmt
    ;


// Class Members: Attributes & Funcs
memberDecl_list:
    /* empty */
    | memberDecl_list visibility memberDecl
    ;

visibility:
    PUBLIC
    | PRIVATE
    ;

memberDecl:
    funcDef
    | funcDecl
    | attributeDecl
    ;

funcDecl:
    funcHead SEMICOLON
    ;

attributeDecl:
    ATTRIBUTE varDecl
    ;

// Local Var Decl
localVarDecl:
    LOCAL varDecl
    ;

varDecl:
    ID COLON type arraySize_list SEMICOLON
    ;

arraySize_list:
    /* empty */
    | arraySize_list arraySize
    ;

arraySize:
    LBRACKET INTLIT RBRACKET
    | LBRACKET RBRACKET
    ;

type:
    INTEGER
    | FLOAT
    | ID
    ;

returnType:
    type
    | VOID
    ;

// Function Params
fParams:
    /* empty */
    | ID COLON type arraySize_list fParamsTail
    ;

fParamsTail:
    /* empty */
    | COMMA ID COLON type arraySize_list fParamsTail
    ;


// Statements
stmt:
      variable ASSIGN expr SEMICOLON
    | IF LPAREN expr RPAREN THEN statBlock optional_else
    | WHILE LPAREN expr RPAREN statBlock
    | READ LPAREN variable RPAREN SEMICOLON
    | WRITE LPAREN expr RPAREN SEMICOLON
    | RETURN LPAREN expr RPAREN SEMICOLON
    | functionCall SEMICOLON
    ;

optional_else:
      ELSE statBlock
    | /* empty */
    ;

statBlock:
    LBRACE stmts RBRACE
    | stmt
    | /* empty */
    ;

stmts:
    /* empty */
    | stmts stmt
    ;

// Expressions
expr:
    expr OR expr
    | expr AND expr
    | NOT expr
    | relExpr
    ;

relExpr:
    arithExpr relOp arithExpr
    | arithExpr
    ;

relOp:
    EQ | NEQ | LT | GT | LTE | GTE
    ;

arithExpr:
    arithExpr addOp term
    | term
    ;

addOp:
    PLUS | MINUS
    ;

term:
    term mulOp factor
    | factor
    ;

mulOp:
    MULT | DIV
    ;

factor:
    sign factor
    | NOT factor
    | LPAREN expr RPAREN
    | functionCall
    | variable
    | INTLIT
    | FLOATLIT
    ;

sign:
    PLUS
    | MINUS
    ;


// Function Calls
functionCall:
    id_chain ID LPAREN aParams RPAREN
    ;


// Variable Access
variable:
    id_chain ID optional_indice
    ;

id_chain:
    /* empty */
    | id_chain idnest
    ;

idnest:
    idOrSelf optional_indice DOT
    | idOrSelf LPAREN aParams RPAREN DOT
    ;

optional_indice:
    /* empty */
    | optional_indice indice
    ;

indice:
    LBRACKET expr RBRACKET
    ;

idOrSelf:
    ID
    | SELF
    ;

// Actual Parameters
aParams:
    /* empty */
    | expr aParamsTail
    ;

aParamsTail:
    /* empty */
    | COMMA expr aParamsTail
    ;

%%

// Error Handling Function
extern char *yytext;

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s", lineno, s);
    if (yychar > 0) {
        fprintf(stderr, ", unexpected token: ");
        switch (yychar) {
            case ID: fprintf(stderr, "ID (%s)", yylval.str); break;
            case INTLIT: fprintf(stderr, "INTLIT (%s)", yylval.str); break;
            case FLOATLIT: fprintf(stderr, "FLOATLIT (%s)", yylval.str); break;
            default: fprintf(stderr, "'%s'", yytext); break;
        }
    }
    fprintf(stderr, "\n");
}

// Entry Point Function
int main() {
    initialize();  // Open file at the start
    yyparse();     // Start the parsing process
    close_token_file();  // Ensure buffer is flushed and file closed
    return 0;
}

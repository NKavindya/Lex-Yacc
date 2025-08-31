/* parser.c - recursive descent driver with derivation tracing
   Put this file in your project and compile with lexer + symtab.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "symtab.h"
#include "parser.tab.h" // token defines and YYSTYPE
extern char *yytext;   /* from Flex */

// ----------------------
// Globals
// ----------------------
int lookahead;
FILE *derivationFile;

int yylex(void);
void yyerror(const char *s);

// access last token semantic values (yylval)
// extern YYSTYPE yylval;
// typedef struct
// {
//     SymbolEntry *sEntry;
//     int iValue;
//     double fValue;
// } SemanticValue;

// SemanticValue curVal;
/* use Bison's semantic union type so curVal and yylval are compatible */
typedef YYSTYPE SemanticValue;
extern YYSTYPE yylval;   /* defined by Bison */
SemanticValue curVal;

// convenience copies of semantic values for the *current* token
SymbolEntry *curSym = NULL;
int curInt = 0;
double curFloat = 0.0;

// ----------------------
// Helper
// ----------------------
void sync_on_error(void)
{
    // Attempt simple resynchronization by discarding tokens until SEMICOLON or RBRACE or EOF
    while (lookahead != SEMICOLON && lookahead != RBRACE && lookahead != 0)
    {
        lookahead = yylex();
        curVal = yylval; // <--- important
        curSym = curVal.sEntry;
        curInt = curVal.iValue;
        curFloat = curVal.fValue;
    }
}

void match(int expected)
{
    if (lookahead == expected)
    {
        // Print matched token with additional info where meaningful
        if (expected == IDENTIFIER )
        {
            if (!curSym)
                curSym = install(yytext, 0); // insert into symbol table if missing
            fprintf(derivationFile, "Matched IDENTIFIER: %s (token %d)\n", curSym->lexeme, lookahead);
        }
        else if (expected == INTEGER_CONSTANT)
        {
            fprintf(derivationFile, "Matched INTEGER_CONSTANT: %d (token %d)\n", curInt, lookahead);
        }
        else if (expected == FLOATING_POINT_CONSTANT)
        {
            fprintf(derivationFile, "Matched FLOATING_POINT_CONSTANT: %f (token %d)\n", curFloat, lookahead);
        }
        else
        {
            fprintf(derivationFile, "Matched token %d\n", lookahead);
        }

        // consume and read next token
        lookahead = yylex();
        // copy semantic value for the token we just read
        curVal = yylval; // <--- important

        curSym = curVal.sEntry;
        curInt = curVal.iValue;
        curFloat = curVal.fValue;
    }
    else
    {
        fprintf(stderr, "Syntax error: expected token %d, found token %d\n", expected, lookahead);
        fprintf(derivationFile, "Syntax error: expected %d found %d\n", expected, lookahead);
        // attempt to recover
        sync_on_error();
        if (lookahead == expected)
        {
            match(expected); // try again after sync
        }
        else
        {
            // If still not good, keep going (caller decides)
        }
    }
}

// ----------------------
// Forward declarations
// ----------------------
void prog();
void classOrImplOrFunc();
void classDecl();
void funcDef();
void implDef();
void statement();
void declaration();
void expr();
void arithExpr();
void term();
void factor();

// ----------------------
// Parser Implementation
// ----------------------
void prog()
{
    fprintf(derivationFile, "prog -> { classOrImplOrFunc | statement }*\n");
    while (lookahead != 0 && lookahead != RBRACE)
    { // stop at EOF or a closing brace (caller handles RBRACE)
        if (lookahead == CLASS_TOKEN || lookahead == IMPLEMENT_TOKEN || lookahead == FUNC_TOKEN)
        {
            classOrImplOrFunc();
        }
        else
        {
            statement();
        }
    }
}

void classOrImplOrFunc()
{
    if (lookahead == CLASS_TOKEN)
        classDecl();
    else if (lookahead == FUNC_TOKEN)
        funcDef();
    else if (lookahead == IMPLEMENT_TOKEN)
        implDef();
    else
        yyerror("Expected class, func, or implement");
}

void classDecl()
{
    fprintf(derivationFile, "classDecl -> CLASS IDENTIFIER '{' prog '}'\n");
    match(CLASS_TOKEN);
    match(IDENTIFIER);
    match(LBRACE);
    prog();
    match(RBRACE);
}

void funcDef()
{
    fprintf(derivationFile, "funcDef -> FUNC IDENTIFIER '(' ')' '{' prog '}'\n");
    match(FUNC_TOKEN);
    match(IDENTIFIER);
    match(LPAREN);
    match(RPAREN);
    match(LBRACE);
    prog();
    match(RBRACE);
}

void implDef()
{
    fprintf(derivationFile, "implDef -> IMPLEMENT IDENTIFIER '{' prog '}'\n");
    match(IMPLEMENT_TOKEN);
    match(IDENTIFIER);
    match(LBRACE);
    prog();
    match(RBRACE);
}

void statement()
{
    fprintf(derivationFile, "statement -> ... (lookahead=%d)\n", lookahead);
    if (lookahead == INTEGER_TOKEN || lookahead == FLOAT_TOKEN)
    {
        declaration();
        match(SEMICOLON);
    }
    else if (lookahead == IDENTIFIER)
    {
        fprintf(derivationFile, "statement -> assignment\n");
        match(IDENTIFIER);
        match(ASSIGN_OP);
        expr();
        match(SEMICOLON);
    }
    else if (lookahead == IF_TOKEN)
    {
        fprintf(derivationFile, "statement -> if_stmt\n");
        match(IF_TOKEN);
        match(LPAREN);
        expr();
        match(RPAREN);
        match(LBRACE);
        prog();
        match(RBRACE);
    }
    else if (lookahead == WHILE_TOKEN)
    {
        fprintf(derivationFile, "statement -> while_stmt\n");
        match(WHILE_TOKEN);
        match(LPAREN);
        expr();
        match(RPAREN);
        match(LBRACE);
        prog();
        match(RBRACE);
    }
    else if (lookahead == READ_TOKEN)
    {
        fprintf(derivationFile, "statement -> read_stmt\n");
        match(READ_TOKEN);
        match(LPAREN);
        match(IDENTIFIER);
        match(RPAREN);
        match(SEMICOLON);
    }
    else if (lookahead == WRITE_TOKEN)
    {
        fprintf(derivationFile, "statement -> write_stmt\n");
        match(WRITE_TOKEN);
        match(LPAREN);
        expr();
        match(RPAREN);
        match(SEMICOLON);
    }
    else if (lookahead == SEMICOLON)
    {
        match(SEMICOLON); // empty statement
    }
    else
    {
        yyerror("Unexpected token in statement; attempting to recover");
        sync_on_error();
        if (lookahead == SEMICOLON)
            match(SEMICOLON);
    }
}
void declaration()
{
    fprintf(derivationFile, "declaration -> type IDENTIFIER [ := expr ]\n");
    if (lookahead == INTEGER_TOKEN)
    {
        match(INTEGER_TOKEN);
        if (curSym != NULL)
            curSym->type = INTEGER_TOKEN; // assign token id as type
        match(IDENTIFIER);
        if (lookahead == ASSIGN_OP)
        {
            match(ASSIGN_OP);
            expr();
        }
    }
    else if (lookahead == FLOAT_TOKEN)
    {
        match(FLOAT_TOKEN);
        if (curSym != NULL)
            curSym->type = FLOAT_TOKEN; // assign token id as type
        match(IDENTIFIER);
        if (lookahead == ASSIGN_OP)
        {
            match(ASSIGN_OP);
            expr();
        }
    }
    else
    {
        yyerror("Expected type in declaration");
    }
}

// void declaration()
// {
//     fprintf(derivationFile, "declaration -> type IDENTIFIER [ := expr ]\n");
//     if (lookahead == INTEGER_TOKEN)
//     {
//         match(INTEGER_TOKEN);
//         match(IDENTIFIER);
//         if (lookahead == ASSIGN_OP)
//         {
//             match(ASSIGN_OP);
//             expr();
//         }
//     }
//     else if (lookahead == FLOAT_TOKEN)
//     {
//         match(FLOAT_TOKEN);
//         match(IDENTIFIER);
//         if (lookahead == ASSIGN_OP)
//         {
//             match(ASSIGN_OP);
//             expr();
//         }
//     }
//     else
//     {
//         yyerror("Expected type in declaration");
//     }
// }

void expr()
{
    fprintf(derivationFile, "expr -> arithExpr (possibly relop)\n");
    arithExpr();
    // optionally a relational operator + another arithExpr
    if (lookahead == EQ_OP || lookahead == NE_OP || lookahead == LE_OP ||
        lookahead == GE_OP || lookahead == LT_OP || lookahead == GT_OP)
    {
        match(lookahead);
        arithExpr();
    }
}

void arithExpr()
{
    fprintf(derivationFile, "arithExpr -> term { (+|-) term }\n");
    term();
    while (lookahead == PLUS_OP || lookahead == MINUS_OP)
    {
        match(lookahead);
        term();
    }
}

void term()
{
    fprintf(derivationFile, "term -> factor { (*|/) factor }\n");
    factor();
    while (lookahead == MUL_OP || lookahead == DIV_OP)
    {
        match(lookahead);
        factor();
    }
}

void factor()
{
    if (lookahead == INTEGER_CONSTANT)
    {
        match(INTEGER_CONSTANT);
    }
    else if (lookahead == FLOATING_POINT_CONSTANT)
    {
        match(FLOATING_POINT_CONSTANT);
    }
    else if (lookahead == IDENTIFIER)
    {
        match(IDENTIFIER);
    }
    else if (lookahead == LPAREN)
    {
        match(LPAREN);
        expr();
        match(RPAREN);
    }
    else
    {
        yyerror("Invalid factor; attempting recovery");
        sync_on_error();
    }
}

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s at token %d\n", s, lookahead);
    fprintf(derivationFile, "Error: %s at token %d\n", s, lookahead);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("fopen");
        return 1;
    }

    // Set lexer input
    extern FILE *yyin;
    yyin = fp;

    derivationFile = fopen("derivation.txt", "w");
    if (!derivationFile)
        derivationFile = stdout;

    // initialize first token (and copy semantic value)
    lookahead = yylex();
    curVal = yylval; // <--- important
    curSym = curVal.sEntry;
    curInt = curVal.iValue;
    curFloat = curVal.fValue;

    prog(); // start parsing

    fprintf(derivationFile, "\nParsing finished.\n");
    printf("\nParsing finished.\n");

    print_symbol_table();

    if (derivationFile != stdout)
        fclose(derivationFile);
    fclose(fp);
    return 0;
}
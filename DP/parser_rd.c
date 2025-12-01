#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser_rd.h"
#include "symtab.h"

/* Parser state */
Token current_token;
FILE *derivation_file = NULL;
int syntax_errors = 0;

/* Forward declarations for helper functions */
static void advance_token(void);
static const char *token_name(int token);
static int parse_idnest_and_id(void);

/* Initialize parser and get first token */
void init_parser(FILE *input, FILE *deriv_file) {
    yyin = input;
    derivation_file = deriv_file;
    syntax_errors = 0;
    advance_token();
}

/* Advance to next token */
static void advance_token(void) {
    int token_type = yylex();
    /* Get token information from lexer */
    current_token = get_last_token();
    current_token.type = token_type;
}

/* Match expected token or report error */
void match(int expected_token) {
    if (current_token.type == expected_token) {
        advance_token();
    } else {
        syntax_error("Expected token mismatch");
    }
}

/* Write derivation step to file */
void write_derivation(const char *non_terminal, const char *production) {
    if (derivation_file) {
        fprintf(derivation_file, "%s -> %s\n", non_terminal, production);
    }
}

/* Report syntax error */
void syntax_error(const char *message) {
    syntax_errors++;
    fprintf(stderr, "Syntax error at line %d: %s (found token %s)\n", 
            yylineno, message, token_name(current_token.type));
}

/* Skip to synchronization point (semicolon, brace, etc.) */
void skip_to_sync_point(void) {
    while (current_token.type != YYEOF && 
           current_token.type != SEMICOLON &&
           current_token.type != RBRACE &&
           current_token.type != RPAREN) {
        advance_token();
    }
}

/* Get token name for error messages */
static const char *token_name(int token) {
    switch (token) {
        case YYEOF: return "EOF";
        case INTEGER_CONSTANT: return "INTEGER_CONSTANT";
        case FLOATING_POINT_CONSTANT: return "FLOATING_POINT_CONSTANT";
        case IDENTIFIER: return "IDENTIFIER";
        case ELSE_TOKEN: return "else";
        case INTEGER_TOKEN: return "integer";
        case SELF_TOKEN: return "self";
        case FLOAT_TOKEN: return "float";
        case ISA_TOKEN: return "isa";
        case CONSTRUCT_TOKEN: return "construct";
        case FUNC_TOKEN: return "func";
        case PRIVATE_TOKEN: return "private";
        case THEN_TOKEN: return "then";
        case IF_TOKEN: return "if";
        case PUBLIC_TOKEN: return "public";
        case LOCAL_TOKEN: return "local";
        case IMPLEMENT_TOKEN: return "implement";
        case READ_TOKEN: return "read";
        case VOID_TOKEN: return "void";
        case AND_TOKEN: return "and";
        case CLASS_TOKEN: return "class";
        case OR_TOKEN: return "or";
        case RETURN_TOKEN: return "return";
        case WHILE_TOKEN: return "while";
        case ATTRIBUTE_TOKEN: return "attribute";
        case WRITE_TOKEN: return "write";
        case NOT_TOKEN: return "not";
        case ASSIGN_OP: return ":=";
        case EQ_OP: return "==";
        case NE_OP: return "<>";
        case LE_OP: return "<=";
        case GE_OP: return ">=";
        case ARROW: return "=>";
        case PLUS_OP: return "+";
        case MINUS_OP: return "-";
        case MUL_OP: return "*";
        case DIV_OP: return "/";
        case LT_OP: return "<";
        case GT_OP: return ">";
        case LPAREN: return "(";
        case RPAREN: return ")";
        case LBRACE: return "{";
        case RBRACE: return "}";
        case LBRACKET: return "[";
        case RBRACKET: return "]";
        case SEMICOLON: return ";";
        case COMMA: return ",";
        case DOT: return ".";
        case COLON: return ":";
        default: return "UNKNOWN";
    }
}

/* Grammar productions - transformed to LL(1) */

/* prog -> classOrImplOrFuncList */
/* classOrImplOrFuncList -> classOrImplOrFunc classOrImplOrFuncList | epsilon */
void parse_prog(void) {
    write_derivation("prog", "classOrImplOrFuncList");
    while (current_token.type == CLASS_TOKEN || 
           current_token.type == IMPLEMENT_TOKEN ||
           current_token.type == FUNC_TOKEN ||
           current_token.type == CONSTRUCT_TOKEN) {
        parse_classOrImplOrFunc();
    }
    write_derivation("classOrImplOrFuncList", "epsilon");
}

/* classOrImplOrFunc -> classDecl | implDef | funcDef */
void parse_classOrImplOrFunc(void) {
    if (current_token.type == CLASS_TOKEN) {
        write_derivation("classOrImplOrFunc", "classDecl");
        parse_classDecl();
    } else if (current_token.type == IMPLEMENT_TOKEN) {
        write_derivation("classOrImplOrFunc", "implDef");
        parse_implDef();
    } else if (current_token.type == FUNC_TOKEN || current_token.type == CONSTRUCT_TOKEN) {
        write_derivation("classOrImplOrFunc", "funcDef");
        parse_funcDef();
    } else {
        syntax_error("Expected class, implement, func, or construct");
    }
}

/* classDecl -> class id classInheritOpt { memberDeclList } ; */
/* classInheritOpt -> isa id idList | epsilon */
/* idList -> , id idList | epsilon */
/* memberDeclList -> visibility memberDecl memberDeclList | epsilon */
void parse_classDecl(void) {
    match(CLASS_TOKEN);
    match(IDENTIFIER);
    
    if (current_token.type == ISA_TOKEN) {
        write_derivation("classInheritOpt", "isa id idList");
        match(ISA_TOKEN);
        match(IDENTIFIER);
        while (current_token.type == COMMA) {
            match(COMMA);
            match(IDENTIFIER);
        }
    } else {
        write_derivation("classInheritOpt", "epsilon");
    }
    
    match(LBRACE);
    while (current_token.type == PUBLIC_TOKEN || current_token.type == PRIVATE_TOKEN) {
        parse_visibility();
        parse_memberDecl();
    }
    write_derivation("memberDeclList", "epsilon");
    match(RBRACE);
    match(SEMICOLON);
}

/* implDef -> implement id { funcDefList } */
/* funcDefList -> funcDef funcDefList | epsilon */
void parse_implDef(void) {
    match(IMPLEMENT_TOKEN);
    match(IDENTIFIER);
    match(LBRACE);
    while (current_token.type == FUNC_TOKEN || current_token.type == CONSTRUCT_TOKEN) {
        parse_funcDef();
    }
    write_derivation("funcDefList", "epsilon");
    match(RBRACE);
}

/* funcDef -> funcHead funcBody */
void parse_funcDef(void) {
    parse_funcHead();
    parse_funcBody();
}

/* visibility -> public | private */
void parse_visibility(void) {
    if (current_token.type == PUBLIC_TOKEN) {
        write_derivation("visibility", "public");
        match(PUBLIC_TOKEN);
    } else if (current_token.type == PRIVATE_TOKEN) {
        write_derivation("visibility", "private");
        match(PRIVATE_TOKEN);
    } else {
        syntax_error("Expected public or private");
    }
}

/* memberDecl -> funcDecl | attributeDecl */
void parse_memberDecl(void) {
    if (current_token.type == FUNC_TOKEN || current_token.type == CONSTRUCT_TOKEN) {
        write_derivation("memberDecl", "funcDecl");
        parse_funcDecl();
    } else if (current_token.type == ATTRIBUTE_TOKEN) {
        write_derivation("memberDecl", "attributeDecl");
        parse_attributeDecl();
    } else {
        syntax_error("Expected funcDecl or attributeDecl");
    }
}

/* funcDecl -> funcHead ; */
void parse_funcDecl(void) {
    parse_funcHead();
    match(SEMICOLON);
}

/* funcHead -> func id ( fParams ) => returnType | construct ( fParams ) */
void parse_funcHead(void) {
    if (current_token.type == FUNC_TOKEN) {
        write_derivation("funcHead", "func id ( fParams ) => returnType");
        match(FUNC_TOKEN);
        match(IDENTIFIER);
        match(LPAREN);
        parse_fParams();
        match(RPAREN);
        match(ARROW);
        parse_returnType();
    } else if (current_token.type == CONSTRUCT_TOKEN) {
        write_derivation("funcHead", "construct ( fParams )");
        match(CONSTRUCT_TOKEN);
        match(LPAREN);
        parse_fParams();
        match(RPAREN);
    } else {
        syntax_error("Expected func or construct");
    }
}

/* funcBody -> { varDeclOrStmtList } */
/* varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList | epsilon */
void parse_funcBody(void) {
    match(LBRACE);
    while (current_token.type == LOCAL_TOKEN ||
           current_token.type == IDENTIFIER ||
           current_token.type == IF_TOKEN ||
           current_token.type == WHILE_TOKEN ||
           current_token.type == READ_TOKEN ||
           current_token.type == WRITE_TOKEN ||
           current_token.type == RETURN_TOKEN) {
        parse_localVarDeclOrStmt();
    }
    write_derivation("varDeclOrStmtList", "epsilon");
    match(RBRACE);
}

/* localVarDeclOrStmt -> localVarDecl | statement */
void parse_localVarDeclOrStmt(void) {
    if (current_token.type == LOCAL_TOKEN) {
        write_derivation("localVarDeclOrStmt", "localVarDecl");
        parse_localVarDecl();
    } else {
        write_derivation("localVarDeclOrStmt", "statement");
        parse_statement();
    }
}

/* attributeDecl -> attribute varDecl */
void parse_attributeDecl(void) {
    match(ATTRIBUTE_TOKEN);
    parse_varDecl();
}

/* localVarDecl -> local varDecl */
void parse_localVarDecl(void) {
    match(LOCAL_TOKEN);
    parse_varDecl();
}

/* varDecl -> id : type arraySizeList ; */
/* arraySizeList -> arraySize arraySizeList | epsilon */
void parse_varDecl(void) {
    match(IDENTIFIER);
    match(COLON);
    parse_type();
    while (current_token.type == LBRACKET) {
        parse_arraySize();
    }
    write_derivation("arraySizeList", "epsilon");
    match(SEMICOLON);
}

/* Helper: parse idnestList and id, return token after */
static int parse_idnest_and_id(void) {
    parse_idnestList();
    if (current_token.type == IDENTIFIER) {
        match(IDENTIFIER);
        parse_indiceList();
        return current_token.type;
    }
    syntax_error("Expected identifier");
    return ERROR_TOKEN;
}

/* statement -> assignStat ; | if ( relExpr ) then statBlock else statBlock ; 
 *            | while ( relExpr ) statBlock ; | read ( variable ) ; 
 *            | write ( expr ) ; | return ( expr ) ; | functionCall ; */
void parse_statement(void) {
    if (current_token.type == IDENTIFIER || current_token.type == SELF_TOKEN) {
        /* Parse idnestList and id, then check what follows */
        int next_token = parse_idnest_and_id();
        
        if (next_token == ASSIGN_OP) {
            write_derivation("statement", "assignStat ;");
            parse_assignOp();
            parse_expr();
            match(SEMICOLON);
        } else if (next_token == LPAREN) {
            write_derivation("statement", "functionCall ;");
            parse_aParams();
            match(RPAREN);
            match(SEMICOLON);
        } else {
            syntax_error("Expected := or ( after identifier");
        }
    } else if (current_token.type == IF_TOKEN) {
        write_derivation("statement", "if ( relExpr ) then statBlock else statBlock ;");
        match(IF_TOKEN);
        match(LPAREN);
        parse_relExpr();
        match(RPAREN);
        match(THEN_TOKEN);
        parse_statBlock();
        match(ELSE_TOKEN);
        parse_statBlock();
        match(SEMICOLON);
    } else if (current_token.type == WHILE_TOKEN) {
        write_derivation("statement", "while ( relExpr ) statBlock ;");
        match(WHILE_TOKEN);
        match(LPAREN);
        parse_relExpr();
        match(RPAREN);
        parse_statBlock();
        match(SEMICOLON);
    } else if (current_token.type == READ_TOKEN) {
        write_derivation("statement", "read ( variable ) ;");
        match(READ_TOKEN);
        match(LPAREN);
        parse_variable();
        match(RPAREN);
        match(SEMICOLON);
    } else if (current_token.type == WRITE_TOKEN) {
        write_derivation("statement", "write ( expr ) ;");
        match(WRITE_TOKEN);
        match(LPAREN);
        parse_expr();
        match(RPAREN);
        match(SEMICOLON);
    } else if (current_token.type == RETURN_TOKEN) {
        write_derivation("statement", "return ( expr ) ;");
        match(RETURN_TOKEN);
        match(LPAREN);
        parse_expr();
        match(RPAREN);
        match(SEMICOLON);
    } else {
        syntax_error("Expected statement");
    }
}

/* assignStat -> variable assignOp expr */
void parse_assignStat(void) {
    parse_variable();
    parse_assignOp();
    parse_expr();
}

/* statBlock -> { statementList } | statement | epsilon */
/* statementList -> statement statementList | epsilon */
void parse_statBlock(void) {
    if (current_token.type == LBRACE) {
        write_derivation("statBlock", "{ statementList }");
        match(LBRACE);
        while (current_token.type == IDENTIFIER ||
               current_token.type == IF_TOKEN ||
               current_token.type == WHILE_TOKEN ||
               current_token.type == READ_TOKEN ||
               current_token.type == WRITE_TOKEN ||
               current_token.type == RETURN_TOKEN) {
            parse_statement();
        }
        write_derivation("statementList", "epsilon");
        match(RBRACE);
    } else if (current_token.type == IDENTIFIER ||
               current_token.type == IF_TOKEN ||
               current_token.type == WHILE_TOKEN ||
               current_token.type == READ_TOKEN ||
               current_token.type == WRITE_TOKEN ||
               current_token.type == RETURN_TOKEN) {
        write_derivation("statBlock", "statement");
        parse_statement();
    } else {
        write_derivation("statBlock", "epsilon");
    }
}

/* expr -> arithExpr | relExpr */
/* We need to look ahead: if we see a relOp after first arithExpr, it's relExpr */
void parse_expr(void) {
    /* Parse first arithExpr */
    parse_arithExpr();
    
    /* Check if this is actually a relExpr */
    if (current_token.type == EQ_OP || current_token.type == NE_OP ||
        current_token.type == LT_OP || current_token.type == GT_OP ||
        current_token.type == LE_OP || current_token.type == GE_OP) {
        write_derivation("expr", "relExpr");
        parse_relOp();
        parse_arithExpr();
    } else {
        write_derivation("expr", "arithExpr");
    }
}

/* relExpr -> arithExpr relOp arithExpr */
void parse_relExpr(void) {
    parse_arithExpr();
    parse_relOp();
    parse_arithExpr();
}

/* arithExpr -> term arithExprPrime */
/* arithExprPrime -> addOp term arithExprPrime | epsilon */
void parse_arithExpr(void) {
    parse_term();
    parse_arithExprPrime();
}

void parse_arithExprPrime(void) {
    if (current_token.type == PLUS_OP || current_token.type == MINUS_OP || 
        current_token.type == OR_TOKEN) {
        write_derivation("arithExprPrime", "addOp term arithExprPrime");
        parse_addOp();
        parse_term();
        parse_arithExprPrime();
    } else {
        write_derivation("arithExprPrime", "epsilon");
    }
}

/* sign -> + | - */
void parse_sign(void) {
    if (current_token.type == PLUS_OP) {
        write_derivation("sign", "+");
        match(PLUS_OP);
    } else if (current_token.type == MINUS_OP) {
        write_derivation("sign", "-");
        match(MINUS_OP);
    } else {
        syntax_error("Expected + or -");
    }
}

/* term -> factor termPrime */
/* termPrime -> multOp factor termPrime | epsilon */
void parse_term(void) {
    parse_factor();
    parse_termPrime();
}

void parse_termPrime(void) {
    if (current_token.type == MUL_OP || current_token.type == DIV_OP || 
        current_token.type == AND_TOKEN) {
        write_derivation("termPrime", "multOp factor termPrime");
        parse_multOp();
        parse_factor();
        parse_termPrime();
    } else {
        write_derivation("termPrime", "epsilon");
    }
}

/* factor -> variable | functionCall | intLit | floatLit | ( arithExpr ) | not factor | sign factor */
void parse_factor(void) {
    if (current_token.type == IDENTIFIER || current_token.type == SELF_TOKEN) {
        /* Need to distinguish variable from functionCall */
        /* Parse idnestList and id, then check what follows */
        parse_idnestList();
        if (current_token.type == IDENTIFIER) {
            match(IDENTIFIER);
            if (current_token.type == LPAREN) {
                write_derivation("factor", "functionCall");
                parse_aParams();
                match(RPAREN);
            } else {
                write_derivation("factor", "variable");
                parse_indiceList();
            }
        } else {
            syntax_error("Expected identifier in factor");
        }
    } else if (current_token.type == INTEGER_CONSTANT) {
        write_derivation("factor", "intLit");
        match(INTEGER_CONSTANT);
    } else if (current_token.type == FLOATING_POINT_CONSTANT) {
        write_derivation("factor", "floatLit");
        match(FLOATING_POINT_CONSTANT);
    } else if (current_token.type == LPAREN) {
        write_derivation("factor", "( arithExpr )");
        match(LPAREN);
        parse_arithExpr();
        match(RPAREN);
    } else if (current_token.type == NOT_TOKEN) {
        write_derivation("factor", "not factor");
        match(NOT_TOKEN);
        parse_factor();
    } else if (current_token.type == PLUS_OP || current_token.type == MINUS_OP) {
        write_derivation("factor", "sign factor");
        parse_sign();
        parse_factor();
    } else {
        syntax_error("Expected factor");
    }
}

/* variable -> idnestList id indiceList */
/* idnestList -> idnest idnestList | epsilon */
/* indiceList -> indice indiceList | epsilon */
void parse_variable(void) {
    parse_idnestList();
    match(IDENTIFIER);
    parse_indiceList();
}

void parse_idnestList(void) {
    while (current_token.type == IDENTIFIER || current_token.type == SELF_TOKEN) {
        parse_idnest();
    }
    write_derivation("idnestList", "epsilon");
}

/* functionCall -> idnestList id ( aParams ) */
void parse_functionCall(void) {
    parse_idnestList();
    match(IDENTIFIER);
    match(LPAREN);
    parse_aParams();
    match(RPAREN);
}

/* idnest -> idOrSelf indiceList . | idOrSelf ( aParams ) . */
void parse_idnest(void) {
    parse_idOrSelf();
    if (current_token.type == LBRACKET) {
        write_derivation("idnest", "idOrSelf indiceList .");
        parse_indiceList();
    } else if (current_token.type == LPAREN) {
        write_derivation("idnest", "idOrSelf ( aParams ) .");
        match(LPAREN);
        parse_aParams();
        match(RPAREN);
    }
    match(DOT);
}

/* idOrSelf -> id | self */
void parse_idOrSelf(void) {
    if (current_token.type == IDENTIFIER) {
        write_derivation("idOrSelf", "id");
        match(IDENTIFIER);
    } else if (current_token.type == SELF_TOKEN) {
        write_derivation("idOrSelf", "self");
        match(SELF_TOKEN);
    } else {
        syntax_error("Expected id or self");
    }
}

/* indice -> [ arithExpr ] */
void parse_indice(void) {
    match(LBRACKET);
    parse_arithExpr();
    match(RBRACKET);
}

void parse_indiceList(void) {
    while (current_token.type == LBRACKET) {
        parse_indice();
    }
    write_derivation("indiceList", "epsilon");
}

/* arraySize -> [ intLit ] | [ ] */
void parse_arraySize(void) {
    match(LBRACKET);
    if (current_token.type == INTEGER_CONSTANT) {
        write_derivation("arraySize", "[ intLit ]");
        match(INTEGER_CONSTANT);
    } else {
        write_derivation("arraySize", "[ ]");
    }
    match(RBRACKET);
}

/* type -> integer | float | id */
void parse_type(void) {
    if (current_token.type == INTEGER_TOKEN) {
        write_derivation("type", "integer");
        match(INTEGER_TOKEN);
    } else if (current_token.type == FLOAT_TOKEN) {
        write_derivation("type", "float");
        match(FLOAT_TOKEN);
    } else if (current_token.type == IDENTIFIER) {
        write_derivation("type", "id");
        match(IDENTIFIER);
    } else {
        syntax_error("Expected type");
    }
}

/* returnType -> type | void */
void parse_returnType(void) {
    if (current_token.type == VOID_TOKEN) {
        write_derivation("returnType", "void");
        match(VOID_TOKEN);
    } else {
        write_derivation("returnType", "type");
        parse_type();
    }
}

/* fParams -> id : type arraySizeList fParamsTailList | epsilon */
/* fParamsTailList -> fParamsTail fParamsTailList | epsilon */
/* fParamsTail -> , id : type arraySizeList */
void parse_fParams(void) {
    if (current_token.type == IDENTIFIER) {
        write_derivation("fParams", "id : type arraySizeList fParamsTailList");
        match(IDENTIFIER);
        match(COLON);
        parse_type();
        while (current_token.type == LBRACKET) {
            parse_arraySize();
        }
        write_derivation("arraySizeList", "epsilon");
        parse_fParamsTailList();
    } else {
        write_derivation("fParams", "epsilon");
    }
}

void parse_fParamsTailList(void) {
    while (current_token.type == COMMA) {
        parse_fParamsTail();
    }
    write_derivation("fParamsTailList", "epsilon");
}

void parse_fParamsTail(void) {
    match(COMMA);
    match(IDENTIFIER);
    match(COLON);
    parse_type();
    while (current_token.type == LBRACKET) {
        parse_arraySize();
    }
    write_derivation("arraySizeList", "epsilon");
}

/* aParams -> expr aParamsTailList | epsilon */
/* aParamsTailList -> aParamsTail aParamsTailList | epsilon */
/* aParamsTail -> , expr */
void parse_aParams(void) {
    if (current_token.type == IDENTIFIER ||
        current_token.type == INTEGER_CONSTANT ||
        current_token.type == FLOATING_POINT_CONSTANT ||
        current_token.type == LPAREN ||
        current_token.type == NOT_TOKEN ||
        current_token.type == PLUS_OP ||
        current_token.type == MINUS_OP) {
        write_derivation("aParams", "expr aParamsTailList");
        parse_expr();
        parse_aParamsTailList();
    } else {
        write_derivation("aParams", "epsilon");
    }
}

void parse_aParamsTailList(void) {
    while (current_token.type == COMMA) {
        parse_aParamsTail();
    }
    write_derivation("aParamsTailList", "epsilon");
}

void parse_aParamsTail(void) {
    match(COMMA);
    parse_expr();
}

/* assignOp -> := */
void parse_assignOp(void) {
    match(ASSIGN_OP);
}

/* relOp -> == | <> | < | > | <= | >= */
void parse_relOp(void) {
    if (current_token.type == EQ_OP) {
        write_derivation("relOp", "==");
        match(EQ_OP);
    } else if (current_token.type == NE_OP) {
        write_derivation("relOp", "<>");
        match(NE_OP);
    } else if (current_token.type == LT_OP) {
        write_derivation("relOp", "<");
        match(LT_OP);
    } else if (current_token.type == GT_OP) {
        write_derivation("relOp", ">");
        match(GT_OP);
    } else if (current_token.type == LE_OP) {
        write_derivation("relOp", "<=");
        match(LE_OP);
    } else if (current_token.type == GE_OP) {
        write_derivation("relOp", ">=");
        match(GE_OP);
    } else {
        syntax_error("Expected relational operator");
    }
}

/* addOp -> + | - | or */
void parse_addOp(void) {
    if (current_token.type == PLUS_OP) {
        write_derivation("addOp", "+");
        match(PLUS_OP);
    } else if (current_token.type == MINUS_OP) {
        write_derivation("addOp", "-");
        match(MINUS_OP);
    } else if (current_token.type == OR_TOKEN) {
        write_derivation("addOp", "or");
        match(OR_TOKEN);
    } else {
        syntax_error("Expected additive operator");
    }
}

/* multOp -> * | / | and */
void parse_multOp(void) {
    if (current_token.type == MUL_OP) {
        write_derivation("multOp", "*");
        match(MUL_OP);
    } else if (current_token.type == DIV_OP) {
        write_derivation("multOp", "/");
        match(DIV_OP);
    } else if (current_token.type == AND_TOKEN) {
        write_derivation("multOp", "and");
        match(AND_TOKEN);
    } else {
        syntax_error("Expected multiplicative operator");
    }
}


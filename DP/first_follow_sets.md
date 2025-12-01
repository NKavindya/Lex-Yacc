# FIRST and FOLLOW Sets

## FIRST Sets

### Terminals
- FIRST(`id`) = { IDENTIFIER }
- FIRST(`intLit`) = { INTEGER_CONSTANT }
- FIRST(`floatLit`) = { FLOATING_POINT_CONSTANT }
- FIRST(`class`) = { CLASS_TOKEN }
- FIRST(`implement`) = { IMPLEMENT_TOKEN }
- FIRST(`func`) = { FUNC_TOKEN }
- FIRST(`construct`) = { CONSTRUCT_TOKEN }
- FIRST(`public`) = { PUBLIC_TOKEN }
- FIRST(`private`) = { PRIVATE_TOKEN }
- FIRST(`attribute`) = { ATTRIBUTE_TOKEN }
- FIRST(`local`) = { LOCAL_TOKEN }
- FIRST(`integer`) = { INTEGER_TOKEN }
- FIRST(`float`) = { FLOAT_TOKEN }
- FIRST(`void`) = { VOID_TOKEN }
- FIRST(`isa`) = { ISA_TOKEN }
- FIRST(`self`) = { SELF_TOKEN }
- FIRST(`if`) = { IF_TOKEN }
- FIRST(`then`) = { THEN_TOKEN }
- FIRST(`else`) = { ELSE_TOKEN }
- FIRST(`while`) = { WHILE_TOKEN }
- FIRST(`read`) = { READ_TOKEN }
- FIRST(`write`) = { WRITE_TOKEN }
- FIRST(`return`) = { RETURN_TOKEN }
- FIRST(`not`) = { NOT_TOKEN }
- FIRST(`or`) = { OR_TOKEN }
- FIRST(`and`) = { AND_TOKEN }
- FIRST(`:=`) = { ASSIGN_OP }
- FIRST(`==`) = { EQ_OP }
- FIRST(`<>`) = { NE_OP }
- FIRST(`<`) = { LT_OP }
- FIRST(`>`) = { GT_OP }
- FIRST(`<=`) = { LE_OP }
- FIRST(`>=`) = { GE_OP }
- FIRST(`+`) = { PLUS_OP }
- FIRST(`-`) = { MINUS_OP }
- FIRST(`*`) = { MUL_OP }
- FIRST(`/`) = { DIV_OP }
- FIRST(`(`) = { LPAREN }
- FIRST(`)`) = { RPAREN }
- FIRST(`{`) = { LBRACE }
- FIRST(`}`) = { RBRACE }
- FIRST(`[`) = { LBRACKET }
- FIRST(`]`) = { RBRACKET }
- FIRST(`;`) = { SEMICOLON }
- FIRST(`,`) = { COMMA }
- FIRST(`.`) = { DOT }
- FIRST(`:`) = { COLON }
- FIRST(`=>`) = { ARROW }

### Non-terminals

**FIRST(prog)** = FIRST(classOrImplOrFuncList) = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN, epsilon }

**FIRST(classOrImplOrFuncList)** = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN, epsilon }

**FIRST(classOrImplOrFunc)** = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN }

**FIRST(classDecl)** = { CLASS_TOKEN }

**FIRST(classInheritOpt)** = { ISA_TOKEN, epsilon }

**FIRST(idList)** = { COMMA, epsilon }

**FIRST(memberDeclList)** = { PUBLIC_TOKEN, PRIVATE_TOKEN, epsilon }

**FIRST(implDef)** = { IMPLEMENT_TOKEN }

**FIRST(funcDefList)** = { FUNC_TOKEN, CONSTRUCT_TOKEN, epsilon }

**FIRST(funcDef)** = { FUNC_TOKEN, CONSTRUCT_TOKEN }

**FIRST(visibility)** = { PUBLIC_TOKEN, PRIVATE_TOKEN }

**FIRST(memberDecl)** = { FUNC_TOKEN, CONSTRUCT_TOKEN, ATTRIBUTE_TOKEN }

**FIRST(funcDecl)** = { FUNC_TOKEN, CONSTRUCT_TOKEN }

**FIRST(funcHead)** = { FUNC_TOKEN, CONSTRUCT_TOKEN }

**FIRST(funcBody)** = { LBRACE }

**FIRST(varDeclOrStmtList)** = { LOCAL_TOKEN, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, epsilon }

**FIRST(varDeclOrStmt)** = { LOCAL_TOKEN, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN }

**FIRST(attributeDecl)** = { ATTRIBUTE_TOKEN }

**FIRST(localVarDecl)** = { LOCAL_TOKEN }

**FIRST(varDecl)** = { IDENTIFIER }

**FIRST(arraySizeList)** = { LBRACKET, epsilon }

**FIRST(arraySize)** = { LBRACKET }

**FIRST(statement)** = { IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN }

**FIRST(assignStat)** = { IDENTIFIER, SELF_TOKEN }

**FIRST(statBlock)** = { LBRACE, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, epsilon }

**FIRST(statementList)** = { IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, epsilon }

**FIRST(expr)** = FIRST(arithExpr) = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FIRST(relExpr)** = FIRST(arithExpr) = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FIRST(arithExpr)** = FIRST(term) = FIRST(factor) = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FIRST(arithExprPrime)** = { PLUS_OP, MINUS_OP, OR_TOKEN, epsilon }

**FIRST(sign)** = { PLUS_OP, MINUS_OP }

**FIRST(term)** = FIRST(factor) = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FIRST(termPrime)** = { MUL_OP, DIV_OP, AND_TOKEN, epsilon }

**FIRST(factor)** = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FIRST(variable)** = { IDENTIFIER, SELF_TOKEN }

**FIRST(functionCall)** = { IDENTIFIER, SELF_TOKEN }

**FIRST(idnestList)** = { IDENTIFIER, SELF_TOKEN, epsilon }

**FIRST(idnest)** = { IDENTIFIER, SELF_TOKEN }

**FIRST(idOrSelf)** = { IDENTIFIER, SELF_TOKEN }

**FIRST(indiceList)** = { LBRACKET, epsilon }

**FIRST(indice)** = { LBRACKET }

**FIRST(type)** = { INTEGER_TOKEN, FLOAT_TOKEN, IDENTIFIER }

**FIRST(returnType)** = { INTEGER_TOKEN, FLOAT_TOKEN, IDENTIFIER, VOID_TOKEN }

**FIRST(fParams)** = { IDENTIFIER, epsilon }

**FIRST(fParamsTailList)** = { COMMA, epsilon }

**FIRST(fParamsTail)** = { COMMA }

**FIRST(aParams)** = FIRST(expr) ∪ { epsilon } = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP, epsilon }

**FIRST(aParamsTailList)** = { COMMA, epsilon }

**FIRST(aParamsTail)** = { COMMA }

**FIRST(assignOp)** = { ASSIGN_OP }

**FIRST(relOp)** = { EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP }

**FIRST(addOp)** = { PLUS_OP, MINUS_OP, OR_TOKEN }

**FIRST(multOp)** = { MUL_OP, DIV_OP, AND_TOKEN }

## FOLLOW Sets

**FOLLOW(prog)** = { $ }

**FOLLOW(classOrImplOrFuncList)** = { $ }

**FOLLOW(classOrImplOrFunc)** = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN, $ }

**FOLLOW(classDecl)** = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN, $ }

**FOLLOW(classInheritOpt)** = { LBRACE }

**FOLLOW(idList)** = { LBRACE }

**FOLLOW(memberDeclList)** = { RBRACE }

**FOLLOW(implDef)** = { CLASS_TOKEN, IMPLEMENT_TOKEN, FUNC_TOKEN, CONSTRUCT_TOKEN, $ }

**FOLLOW(funcDefList)** = { RBRACE }

**FOLLOW(funcDef)** = { FUNC_TOKEN, CONSTRUCT_TOKEN, RBRACE }

**FOLLOW(visibility)** = { FUNC_TOKEN, CONSTRUCT_TOKEN, ATTRIBUTE_TOKEN }

**FOLLOW(memberDecl)** = { PUBLIC_TOKEN, PRIVATE_TOKEN, RBRACE }

**FOLLOW(funcDecl)** = { PUBLIC_TOKEN, PRIVATE_TOKEN, RBRACE }

**FOLLOW(funcHead)** = { LBRACE, SEMICOLON }

**FOLLOW(funcBody)** = { FUNC_TOKEN, CONSTRUCT_TOKEN, RBRACE }

**FOLLOW(varDeclOrStmtList)** = { RBRACE }

**FOLLOW(varDeclOrStmt)** = { LOCAL_TOKEN, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, RBRACE }

**FOLLOW(attributeDecl)** = { PUBLIC_TOKEN, PRIVATE_TOKEN, RBRACE }

**FOLLOW(localVarDecl)** = { LOCAL_TOKEN, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, RBRACE }

**FOLLOW(varDecl)** = { LOCAL_TOKEN, IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, RBRACE, SEMICOLON }

**FOLLOW(arraySizeList)** = { SEMICOLON, COMMA }

**FOLLOW(arraySize)** = { LBRACKET, SEMICOLON, COMMA }

**FOLLOW(statement)** = { IDENTIFIER, SELF_TOKEN, IF_TOKEN, WHILE_TOKEN, READ_TOKEN, WRITE_TOKEN, RETURN_TOKEN, RBRACE, SEMICOLON }

**FOLLOW(assignStat)** = { SEMICOLON }

**FOLLOW(statBlock)** = { ELSE_TOKEN, SEMICOLON, RBRACE }

**FOLLOW(statementList)** = { RBRACE }

**FOLLOW(expr)** = { RPAREN, SEMICOLON, COMMA }

**FOLLOW(relExpr)** = { RPAREN, THEN_TOKEN, SEMICOLON }

**FOLLOW(arithExpr)** = { EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, RPAREN, SEMICOLON, COMMA, RBRACKET }

**FOLLOW(arithExprPrime)** = { EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, RPAREN, SEMICOLON, COMMA, RBRACKET }

**FOLLOW(sign)** = { IDENTIFIER, SELF_TOKEN, INTEGER_CONSTANT, FLOATING_POINT_CONSTANT, LPAREN, NOT_TOKEN, PLUS_OP, MINUS_OP }

**FOLLOW(term)** = { PLUS_OP, MINUS_OP, OR_TOKEN, EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, RPAREN, SEMICOLON, COMMA, RBRACKET }

**FOLLOW(termPrime)** = { PLUS_OP, MINUS_OP, OR_TOKEN, EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, RPAREN, SEMICOLON, COMMA, RBRACKET }

**FOLLOW(factor)** = { MUL_OP, DIV_OP, AND_TOKEN, PLUS_OP, MINUS_OP, OR_TOKEN, EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, RPAREN, SEMICOLON, COMMA, RBRACKET }

**FOLLOW(variable)** = { ASSIGN_OP, RPAREN, SEMICOLON, COMMA }

**FOLLOW(functionCall)** = { SEMICOLON, COMMA }

**FOLLOW(idnestList)** = { IDENTIFIER }

**FOLLOW(idnest)** = { IDENTIFIER, SELF_TOKEN }

**FOLLOW(idOrSelf)** = { LBRACKET, LPAREN, DOT }

**FOLLOW(indiceList)** = { ASSIGN_OP, RPAREN, SEMICOLON, COMMA, DOT }

**FOLLOW(indice)** = { LBRACKET, ASSIGN_OP, RPAREN, SEMICOLON, COMMA, DOT }

**FOLLOW(type)** = { LBRACKET, SEMICOLON, COMMA, ARROW }

**FOLLOW(returnType)** = { LBRACE }

**FOLLOW(fParams)** = { RPAREN }

**FOLLOW(fParamsTailList)** = { RPAREN }

**FOLLOW(fParamsTail)** = { COMMA, RPAREN }

**FOLLOW(aParams)** = { RPAREN }

**FOLLOW(aParamsTailList)** = { RPAREN }

**FOLLOW(aParamsTail)** = { COMMA, RPAREN }

## LL(1) Check

For the grammar to be LL(1), for each non-terminal A with productions:
- A -> α₁ | α₂ | ... | αₙ

We need:
- FIRST(αᵢ) ∩ FIRST(αⱼ) = ∅ for all i ≠ j
- If ε ∈ FIRST(αᵢ), then FIRST(αⱼ) ∩ FOLLOW(A) = ∅ for all j ≠ i

Most productions satisfy this. The main challenge is:
- `expr -> arithExpr | relExpr` - requires lookahead (parse arithExpr, then check for relOp)
- `variable` vs `functionCall` - both start with `idnestList id`, require lookahead after parsing these

These are handled in the parser implementation using lookahead.


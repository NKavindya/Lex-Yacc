# Grammar Transformation to LL(1)

## Original Grammar Issues

### 1. EBNF Notations
- `{{ ... }}` - zero or more repetitions
- `[[ ... ]]` - optional (zero or one)

### 2. Left Recursion
- `arithExpr -> arithExpr addOp term | term`
- `term -> term multOp factor | factor`

### 3. Ambiguities
- `expr -> arithExpr | relExpr` - requires lookahead to distinguish
- `variable` vs `functionCall` - both start with `idnestList id`

## Transformed LL(1) Grammar

### Removed EBNF and Left Recursion

#### Original: `prog -> {{ classOrImplOrFunc }}`
**Transformed:**
```
prog -> classOrImplOrFuncList
classOrImplOrFuncList -> classOrImplOrFunc classOrImplOrFuncList | epsilon
```

#### Original: `classDecl -> class id [[ isa id {{ , id }} ]] { {{ visibility memberDecl }} } ;`
**Transformed:**
```
classDecl -> class id classInheritOpt { memberDeclList } ;
classInheritOpt -> isa id idList | epsilon
idList -> , id idList | epsilon
memberDeclList -> visibility memberDecl memberDeclList | epsilon
```

#### Original: `implDef -> implement id { {{funcDef}} }`
**Transformed:**
```
implDef -> implement id { funcDefList }
funcDefList -> funcDef funcDefList | epsilon
```

#### Original: `funcBody -> { {{varDeclOrStmt}} }`
**Transformed:**
```
funcBody -> { varDeclOrStmtList }
varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList | epsilon
```

#### Original: `varDecl -> id : type {{arraySize}} ;`
**Transformed:**
```
varDecl -> id : type arraySizeList ;
arraySizeList -> arraySize arraySizeList | epsilon
```

#### Original: `statBlock -> { {{statement}} } | statement | epsilon`
**Transformed:**
```
statBlock -> { statementList } | statement | epsilon
statementList -> statement statementList | epsilon
```

#### Original: `arithExpr -> arithExpr addOp term | term` (LEFT RECURSION)
**Transformed:**
```
arithExpr -> term arithExprPrime
arithExprPrime -> addOp term arithExprPrime | epsilon
```

#### Original: `term -> term multOp factor | factor` (LEFT RECURSION)
**Transformed:**
```
term -> factor termPrime
termPrime -> multOp factor termPrime | epsilon
```

#### Original: `variable -> {{idnest}} id {{indice}}`
**Transformed:**
```
variable -> idnestList id indiceList
idnestList -> idnest idnestList | epsilon
indiceList -> indice indiceList | epsilon
```

#### Original: `functionCall -> {{idnest}} id ( aParams )`
**Transformed:**
```
functionCall -> idnestList id ( aParams )
```

#### Original: `fParams -> id : type {{arraySize}} {{fParamsTail}} | epsilon`
**Transformed:**
```
fParams -> id : type arraySizeList fParamsTailList | epsilon
fParamsTailList -> fParamsTail fParamsTailList | epsilon
fParamsTail -> , id : type arraySizeList
```

#### Original: `aParams -> expr {{aParamsTail}} | epsilon`
**Transformed:**
```
aParams -> expr aParamsTailList | epsilon
aParamsTailList -> aParamsTail aParamsTailList | epsilon
aParamsTail -> , expr
```

## Complete Transformed Grammar

```
prog -> classOrImplOrFuncList
classOrImplOrFuncList -> classOrImplOrFunc classOrImplOrFuncList | epsilon
classOrImplOrFunc -> classDecl | implDef | funcDef

classDecl -> class id classInheritOpt { memberDeclList } ;
classInheritOpt -> isa id idList | epsilon
idList -> , id idList | epsilon
memberDeclList -> visibility memberDecl memberDeclList | epsilon

implDef -> implement id { funcDefList }
funcDefList -> funcDef funcDefList | epsilon

funcDef -> funcHead funcBody

visibility -> public | private
memberDecl -> funcDecl | attributeDecl
funcDecl -> funcHead ;

funcHead -> func id ( fParams ) => returnType | construct ( fParams )

funcBody -> { varDeclOrStmtList }
varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList | epsilon
varDeclOrStmt -> localVarDecl | statement

attributeDecl -> attribute varDecl
localVarDecl -> local varDecl
varDecl -> id : type arraySizeList ;
arraySizeList -> arraySize arraySizeList | epsilon
arraySize -> [ intLit ] | [ ]

statement -> assignStat ; | if ( relExpr ) then statBlock else statBlock ;
         | while ( relExpr ) statBlock ; | read ( variable ) ; 
         | write ( expr ) ; | return ( expr ) ; | functionCall ;

assignStat -> variable assignOp expr

statBlock -> { statementList } | statement | epsilon
statementList -> statement statementList | epsilon

expr -> arithExpr | relExpr
relExpr -> arithExpr relOp arithExpr

arithExpr -> term arithExprPrime
arithExprPrime -> addOp term arithExprPrime | epsilon

sign -> + | -
term -> factor termPrime
termPrime -> multOp factor termPrime | epsilon

factor -> variable | functionCall | intLit | floatLit | ( arithExpr ) 
       | not factor | sign factor

variable -> idnestList id indiceList
idnestList -> idnest idnestList | epsilon
indiceList -> indice indiceList | epsilon

functionCall -> idnestList id ( aParams )
idnest -> idOrSelf indiceList . | idOrSelf ( aParams ) .
idOrSelf -> id | self
indice -> [ arithExpr ]

type -> integer | float | id
returnType -> type | void

fParams -> id : type arraySizeList fParamsTailList | epsilon
fParamsTailList -> fParamsTail fParamsTailList | epsilon
fParamsTail -> , id : type arraySizeList

aParams -> expr aParamsTailList | epsilon
aParamsTailList -> aParamsTail aParamsTailList | epsilon
aParamsTail -> , expr

assignOp -> :=
relOp -> == | <> | < | > | <= | >=
addOp -> + | - | or
multOp -> * | / | and
```

## Notes on Disambiguation

1. **expr**: Requires lookahead - parse `arithExpr`, then check if `relOp` follows
2. **variable vs functionCall**: Both start with `idnestList id`. After parsing these, check if next token is `(` (functionCall) or `[`/`:=`/other (variable)
3. **statBlock**: Check if `{` follows (block) or statement token (single statement) or nothing (epsilon)


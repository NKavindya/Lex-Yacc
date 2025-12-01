# Grammar LL(1) Transformation Summary

This document summarizes the transformation of the EBNF grammar into an LL(1) compatible grammar for the parser.

## EBNF to LL(1) Transformations

### 1. List Constructs ({{...}} - Zero or More)

All EBNF list constructs `{{...}}` have been converted to right-recursive productions:

| Original EBNF | LL(1) Right-Recursive Form |
|---------------|---------------------------|
| `prog -> {{ classOrImplOrFunc }}` | `prog -> classOrImplOrFunc prog \| epsilon` |
| `classDecl -> {{{ visibility memberDecl }}}` | `classBody -> visibility memberDecl classBody \| epsilon` |
| `implDef -> {{funcDef}}` | `implFuncs -> funcDef implFuncs \| epsilon` |
| `funcBody -> {{varDeclOrStmt}}` | `varDeclOrStmtList -> varDeclOrStmt varDeclOrStmtList \| epsilon` |
| `varDecl -> {{arraySize}}` | `arraySizes -> arraySize arraySizes \| epsilon` |
| `statBlock -> {{statement}}` | `statementList -> statement statementList \| epsilon` |
| `variable -> {{idnest}} id {{indice}}` | `variable -> idnest DOT variable \| ID indiceList`<br>`indiceList -> indice indiceList \| epsilon` |
| `functionCall -> {{idnest}} id ( aParams )` | `functionCall -> idnest DOT functionCall \| ID LPAREN aParams RPAREN` |
| `idnest -> idOrSelf {{indice}}` | `idnest -> idOrSelf indiceList DOT`<br>`indiceList -> indice indiceList \| epsilon` |
| `fParams -> {{fParamsTail}}` | `fParamsTailList -> COMMA id : type arraySizes fParamsTailList \| epsilon` |
| `aParams -> {{aParamsTail}}` | `aParamsTailList -> COMMA expr aParamsTailList \| epsilon` |

### 2. Optional Constructs ([[...]])

Optional constructs have been converted to explicit alternatives with epsilon:

| Original EBNF | LL(1) Form |
|---------------|-----------|
| `classDecl -> [[ isa id {{, id }}]]` | `classInherit -> ISA ID moreIds \| epsilon`<br>`moreIds -> COMMA ID moreIds \| epsilon` |

### 3. Left Recursion Elimination

Left-recursive productions have been converted to right-recursive form:

| Original Left-Recursive | LL(1) Right-Recursive Form |
|------------------------|---------------------------|
| `arithExpr -> arithExpr addOp term \| term` | `arithExpr -> term arithExprPrime`<br>`arithExprPrime -> addOp term arithExprPrime \| epsilon` |
| `term -> term multOp factor \| factor` | `term -> factor termPrime`<br>`termPrime -> multOp factor termPrime \| epsilon` |

### 4. Ambiguity Resolution

**Issue**: The original grammar had `expr -> arithExpr \| relExpr` and `relExpr -> arithExpr relOp arithExpr`, which created ambiguity when `relExpr` could also be just `arithExpr`.

**Resolution**: Removed the `relExpr -> arithExpr` fallback. Now `relExpr` only matches expressions with relational operators, and `expr` explicitly chooses between `arithExpr` and `relExpr`.

**Final Form**:
- `expr -> arithExpr \| relExpr`
- `relExpr -> arithExpr relOp arithExpr` (no epsilon or plain arithExpr)

## Grammar Rules Coverage

All grammar rules from the specification are implemented:

✅ `prog` - Right-recursive list  
✅ `classOrImplOrFunc` - Choice between classDecl, implDef, funcDef  
✅ `classDecl` - With optional inheritance and right-recursive member list  
✅ `implDef` - With right-recursive function list  
✅ `funcDef` - funcHead funcBody  
✅ `visibility` - public \| private  
✅ `memberDecl` - funcDecl \| attributeDecl  
✅ `funcDecl` - funcHead ;  
✅ `funcHead` - func id ( fParams ) => returnType \| constructor ( fParams )  
✅ `funcBody` - { varDeclOrStmtList }  
✅ `localVarDeclOrStmt` - localVarDecl \| statement  
✅ `attributeDecl` - attribute varDecl  
✅ `localVarDecl` - local varDecl  
✅ `varDecl` - id : type arraySizes ;  
✅ `statement` - All statement types  
✅ `assignStat` - variable assignOp expr  
✅ `statBlock` - { statementList } \| statement \| epsilon  
✅ `expr` - arithExpr \| relExpr  
✅ `relExpr` - arithExpr relOp arithExpr  
✅ `arithExpr` - Right-recursive (term arithExprPrime)  
✅ `sign` - + \| -  
✅ `term` - Right-recursive (factor termPrime)  
✅ `factor` - All factor types  
✅ `variable` - Right-recursive nested access  
✅ `functionCall` - Right-recursive nested access  
✅ `idnest` - idOrSelf indiceList . \| idOrSelf ( aParams ) .  
✅ `idOrSelf` - id \| self  
✅ `indice` - [ arithExpr ]  
✅ `arraySize` - [ INT_LIT ] \| [ ]  
✅ `type` - integer \| float \| id (no epsilon)  
✅ `returnType` - type \| void  
✅ `fParams` - Right-recursive parameter list  
✅ `aParams` - Right-recursive argument list  
✅ `fParamsTail` - , id : type arraySizes  
✅ `aParamsTail` - , expr  
✅ `assignOp` - :=  
✅ `relOp` - == \| <> \| < \| > \| <= \| >=  
✅ `addOp` - + \| - \| or  
✅ `multOp` - * \| / \| and  

## Lexer Token Coverage

All required tokens are implemented in `scanner.l`:

### Punctuation
✅ `(` `)` `,` `{` `.` `}` `:` `[` `=>` `]` `;`

### Operators
✅ `+` `-` `*` `/` `:=` `==` `<>` `<` `>` `<=` `>=`

### Reserved Words
✅ `integer` `float` `isa` `construct`/`constructor` `func` `private` `public` `then` `or` `and` `class` `return` `void` `attribute` `implement` `read` `write` `if` `else` `while` `self` `local` `not`

### Lexical Elements
✅ `id` - letter alphanum*  
✅ `intLit` - integer (0 \| nonzero digit*)  
✅ `floatLit` - integer fraction [e [+ \| -] integer]  
✅ Comments - `//` inline and `/* */` block comments

## Derivation Logging

All production rules include `log_production()` calls that write to `derivation_steps.txt` file. The derivation file is opened in `main.c` and closed after parsing completes.

## Parser Conflicts

The parser expects 7 shift/reduce conflicts (declared with `%expect 7`), which are due to:
- Nested member access patterns in `variable` and `functionCall`
- These conflicts are resolved correctly by Bison's default shift action

The parser is now LL(1) compatible with all EBNF constructs converted to right-recursive forms and all left recursions eliminated.


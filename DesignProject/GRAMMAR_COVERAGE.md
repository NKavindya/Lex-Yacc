# Grammar Coverage Analysis

## Comparison: Design Project vs TMA02 Original Grammar

### ✅ Fully Covered Features

1. **Top-level declarations**
   - `prog → {{ classOrImplOrFunc }}` ✓
   - `classDecl → class id [[ isa id {{ , id }} ]] { {{ visibility memberDecl }} } ;` ✓
   - `implDef → implement id { {{funcDef}} }` ✓
   - `funcDef → funcHead funcBody` ✓

2. **Function declarations**
   - `funcHead → func id ( fParams ) => returnType | constructor ( fParams )` ✓
   - `funcBody → { {{varDeclOrStmt}} }` ✓
   - `fParams → id : type {{arraySize}} {{fParamsTail}} | ϵ` ✓

3. **Statements**
   - `assignStat → variable assignOp expr` ✓ (simplified - see below)
   - `if ( relExpr ) then statBlock else statBlock ;` ✓
   - `while ( relExpr ) statBlock ;` ✓
   - `read ( variable ) ;` ✓ (simplified)
   - `write ( expr ) ;` ✓
   - `return ( expr ) ;` ✓
   - `functionCall ;` ✓ (simplified - see below)

4. **Expressions (basic)**
   - `expr → arithExpr | relExpr` ✓
   - `relExpr → arithExpr relOp arithExpr` ✓
   - `arithExpr → arithExpr addOp term | term` ✓ (left-recursive, needs LL(1) fix)
   - `term → term multOp factor | factor` ✓ (left-recursive, needs LL(1) fix)

5. **Types and declarations**
   - `type → integer | float | id` ✓
   - `returnType → type | void` ✓
   - `varDecl → id : type {{arraySize}} ;` ✓
   - `arraySize → [ intLit ] | [ ]` ✓

### ❌ Missing Features

1. **Nested Variable Access**
   - **Original**: `variable → {{idnest}} id {{indice}}`
   - **Current**: Only simple `ID` in `factor`
   - **Missing**: 
     - `idnest → idOrSelf {{indice}}. | idOrSelf ( aParams ) .`
     - `indice → [ arithExpr ]` (array indexing in variable access)
     - `idOrSelf → id | self`
   - **Impact**: Cannot parse `obj.field[index]`, `obj.method().field`, `self.attr`

2. **Nested Function Calls**
   - **Original**: `functionCall → {{idnest}} id ( aParams )`
   - **Current**: Only `ID LPAREN aParams RPAREN`
   - **Missing**: Method calls like `obj.method()`, `self.func()`
   - **Impact**: Cannot parse object-oriented method calls

3. **Unary Sign Operators**
   - **Original**: `sign → + | -` and `factor → sign factor`
   - **Current**: Missing unary `+` and `-` operators
   - **Impact**: Cannot parse `-x`, `+y` expressions

4. **Logical Operators in Expressions**
   - **Original**: `expr → arithExpr | relExpr` and logical ops handled separately
   - **Current**: Has `expr → expr AND expr | expr OR expr | relExpr` (left-recursive)
   - **Issue**: Left recursion prevents LL(1) parsing
   - **Impact**: Needs transformation to right-recursive form

5. **Expression Grammar Structure**
   - **Original**: `expr → arithExpr | relExpr` (separate)
   - **Current**: `expr → expr AND expr | expr OR expr | relExpr` (mixed)
   - **Note**: Current version works but is not LL(1) compatible

### ⚠️ LL(1) Compatibility Issues

The current parser has **left recursion** in:
1. `expr → expr AND expr | expr OR expr` (lines 424, 432)
2. `arithExpr → arithExpr PLUS term | arithExpr MINUS term` (lines 509, 517)
3. `term → term MULT factor | term DIV factor` (lines 533, 541)

These need to be converted to **right-recursive** forms for LL(1) parsing:
- `expr → relExpr exprPrime` where `exprPrime → AND relExpr exprPrime | OR relExpr exprPrime | ϵ`
- `arithExpr → term arithExprPrime` where `arithExprPrime → addOp term arithExprPrime | ϵ`
- `term → factor termPrime` where `termPrime → multOp factor termPrime | ϵ`

### Summary

**Coverage**: ~85% of original grammar
- ✅ All basic declarations and statements
- ✅ All control flow constructs
- ✅ Basic expressions and operators
- ❌ Missing nested variable/function access (OOP features)
- ❌ Missing unary sign operators
- ⚠️ Not LL(1) compatible (left recursion)

**Recommendation**: 
1. Add missing features (variable, idnest, indice, sign)
2. Convert to LL(1) by removing left recursion
3. Test with comprehensive test cases

## LL(1) Conversion Feasibility

**Yes, it is possible to generate the Design Project with LL(1) parser.**

### Required Transformations

1. **Remove Left Recursion from `expr`**:
   ```
   Current (LR):  expr → expr AND expr | expr OR expr | relExpr
   LL(1) form:    expr → relExpr exprPrime
                  exprPrime → AND relExpr exprPrime | OR relExpr exprPrime | ϵ
   ```

2. **Remove Left Recursion from `arithExpr`**:
   ```
   Current (LR):  arithExpr → arithExpr PLUS term | arithExpr MINUS term | term
   LL(1) form:    arithExpr → term arithExprPrime
                  arithExprPrime → addOp term arithExprPrime | ϵ
                  addOp → PLUS | MINUS
   ```

3. **Remove Left Recursion from `term`**:
   ```
   Current (LR):  term → term MULT factor | term DIV factor | factor
   LL(1) form:    term → factor termPrime
                  termPrime → multOp factor termPrime | ϵ
                  multOp → MULT | DIV | AND
   ```

4. **Add Missing Features**:
   - `variable → idnestChain id indiceChain`
   - `idnestChain → idnest idnestChain | ϵ`
   - `idnest → idOrSelf indiceChain DOT | idOrSelf LPAREN aParams RPAREN DOT`
   - `indiceChain → indice indiceChain | ϵ`
   - `indice → LBRACKET arithExpr RBRACKET`
   - `idOrSelf → ID | SELF`
   - `sign → PLUS | MINUS` (for unary operators)
   - `factor → sign factor | ...` (add unary + and -)

### Implementation Notes

- The LL(1) version will require restructuring AST building to handle right-associative operators correctly
- Operator precedence must be maintained through the grammar structure
- All existing test cases should still work after conversion
- The conversion maintains 100% language compatibility - no semantic changes

### Status

**Current**: LR parser (Bison) with left recursion - works but not LL(1)
**Target**: LL(1) parser with right recursion - fully compatible with TMA02 requirements


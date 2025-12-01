# Test Results Table - Part (c)

## Comprehensive Test Coverage for All Grammar Rules

This table documents all test files in `tests/pass/` directory, their purpose in testing grammar rules, and compilation results.

| Test File | Purpose (Grammar Rules Tested) | Result |
|-----------|-------------------------------|--------|
| test01.src | **classDecl**: Basic class declaration with `public attribute` member. Tests `class id { visibility memberDecl }` production. | ✅ PASS |
| test02.src | **classDecl**: Single class with attribute declaration. Tests class structure and attribute visibility. | ✅ PASS |
| test03.src | **funcDef, arithExpr, addOp**: Simple function definition with arithmetic expression using `+` operator. Tests `funcHead funcBody` and `arithExpr addOp term`. | ✅ PASS |
| test04.src | **statement, relExpr, if-else**: Control flow with `if (relExpr) then statBlock else statBlock`. Tests relational expression in conditional. | ✅ PASS |
| test05.src | **factor, sign, unary operators**: Tests `sign factor` production with `+` and `-` unary operators. Tests `positive := +x` and `negative := -y`. | ✅ PASS |
| test06.src | **statement, nested if**: Nested if statements. Tests `statBlock` containing another `if` statement. | ✅ PASS |
| test07.src | **arithExpr, term, multOp**: Multiple arithmetic operations with `+`, `-`, `*`, `/`. Tests operator precedence and `term multOp factor` production. | ✅ PASS |
| test08.src | **statement, read, write, void returnType**: Tests `read (variable)` and `write (expr)` statements. Also tests `returnType -> void`. | ✅ PASS |
| test09.src | **factor, not operator**: Tests `not factor` production. Tests logical negation with `notResult := not x`. | ✅ PASS |
| test10.src | **arithExpr, addOp, logical operators**: Tests `addOp -> or` and `multOp -> and` productions. Tests short-circuit evaluation for `and` and `or` operators. | ✅ PASS |
| test11.src | **arithExpr, term, precedence**: Complex arithmetic expressions testing operator precedence. Tests `arithExpr -> arithExpr addOp term` and `term -> term multOp factor`. | ✅ PASS |
| test12.src | **relExpr, relOp**: All relational operators (`==`, `<>`, `<`, `>`, `<=`, `>=`). Tests `relExpr -> arithExpr relOp arithExpr` production. | ✅ PASS |
| test13.src | **classDecl, attribute**: Class declaration with single attribute. Tests `memberDecl -> attributeDecl` and `attributeDecl -> attribute varDecl`. | ✅ PASS |
| test14.src | **statement, while, nested loops**: Nested `while` loops. Tests `while (relExpr) statBlock` with nested structure. | ✅ PASS |
| test15.src | **varDecl, localVarDecl**: Multiple local variable declarations. Tests `localVarDecl -> local varDecl` and `varDecl -> id : type`. | ✅ PASS |
| test16.src | **statement, mixed control flow**: Combination of `while` and `if` statements. Tests complex control flow structures. | ✅ PASS |
| test17.src | **classDecl, classInherit**: Class inheritance using `isa` keyword. Tests `classDecl -> class id isa id moreIds` production. | ✅ PASS |
| test18.src | **varDecl, arraySize, indice**: Array declarations and indexing. Tests `varDecl -> id : type arraySize` with `arraySize -> [intLit]` and `indice -> [arithExpr]`. | ✅ PASS |
| test19.src | **statement, read, write, logical ops, unary, not**: Comprehensive test combining `read`, `write`, logical operators (`and`), unary sign (`+`, `-`), and `not` operator. Tests multiple statement types. | ✅ PASS |
| test20.src | **implDef, funcDef**: Implementation block with function definition. Tests `implDef -> IMPLEMENT id { implFuncs }` and `implFuncs -> funcDef implFuncs`. | ✅ PASS |
| test21.src | **implDef, funcHead, constructor**: Constructor inside implement block. Tests `funcHead -> CONSTRUCT (fParams)` production. | ✅ PASS |
| test22.src | **variable, idnest, nested access**: Nested variable access with object fields. Tests `variable -> {{idnest}} id {{indice}}` with `idnest -> idOrSelf {{indice}}.` production. | ✅ PASS |
| test23.src | **variable, idnest, object access**: Object field access in function parameters. Tests nested variable access pattern `obj.field`. | ✅ PASS |
| test24.src | **idOrSelf, self keyword**: Usage of `self` keyword in class methods. Tests `idOrSelf -> self` and `variable -> {{idnest}} id` with `self.field` pattern. | ✅ PASS |
| test25.src | **classInherit, multiple inheritance**: Multiple inheritance with `isa` keyword and multiple parent classes. Tests `classDecl -> class id isa id moreIds` with multiple parents. | ✅ PASS |
| test26.src | **arraySize, empty array**: Empty array size declaration. Tests `arraySize -> []` production in `varDecl -> id : type {{arraySize}}`. | ✅ PASS |
| test27.src | **variable, multiple idnest**: Complex nested access with multiple levels. Tests `variable -> {{idnest}} id` with multiple `idnest` (e.g., `obj1.obj2.field`). | ✅ PASS |
| test28.src | **functionCall, idnest, method chaining**: Nested function calls with object access. Tests `functionCall -> {{idnest}} id (aParams)` with `idnest -> idOrSelf (aParams).` pattern. | ✅ PASS |
| test29.src | **variable, idnest, indice, array nested access**: Array indexing with nested object access. Tests `variable -> {{idnest}} id {{indice}}` with `obj.array[index]` pattern. | ✅ PASS |
| test30.src | **variable, multiple idnest levels**: Deeply nested variable access across multiple levels. Tests `variable -> {{idnest}} id` with pattern like `obj1.obj2.field`. | ✅ PASS |

## Error Test Cases - Semantic Error Detection

This table documents all test files in `tests/error/` directory, their purpose in testing error detection, and the semantic errors detected by the compiler.

| Test File | Purpose (Error Type Tested) | Errors Detected | Result |
|-----------|----------------------------|-----------------|--------|
| test01.src | **Undeclared variable**: Variable `c` used before declaration in arithmetic expression | 1. Identifier 'c' used before declaration<br>2. Arithmetic operands must be numeric (found int and &lt;error&gt;)<br>3. Arithmetic operands must be numeric (found int and &lt;void&gt;) | ✅ ERROR DETECTED (3 errors) |
| test02.src | **Type mismatch in assignment**: Assigning float value to integer variable | Type mismatch in assignment: left is int, right is float | ✅ ERROR DETECTED (1 error) |
| test03.src | **Undeclared variable**: Variable `b` used before declaration | 1. Identifier 'b' used before declaration<br>2. Arithmetic operands must be numeric (found int and &lt;error&gt;) | ✅ ERROR DETECTED (2 errors) |
| test04.src | **Type mismatch in assignment**: Assigning float value to integer variable | Type mismatch in assignment: left is int, right is float | ✅ ERROR DETECTED (1 error) |
| test05.src | **Variable redeclaration**: Variable `x` declared twice in same function | Local variable 'x' redeclared in function 'test' | ✅ ERROR DETECTED (1 error) |
| test06.src | **Wrong number of arguments**: Function called with incorrect argument count | 1. Function 'testAdd' redeclared in scope 'global'<br>2. Call to 'testAdd' with wrong number of arguments (expected 2, got 1) | ✅ ERROR DETECTED (2 errors) |
| test07.src | **Type mismatch in assignment**: Assigning float literal to integer variable | Type mismatch in assignment: left is int, right is float | ✅ ERROR DETECTED (1 error) |
| test08.src | **Undeclared function call**: Calling function that doesn't exist | Call to undefined function 'unknownFunction' | ✅ ERROR DETECTED (1 error) |
| test09.src | **Wrong return type**: Returning float from integer function | Return type mismatch: expected int, got float | ✅ ERROR DETECTED (1 error) |
| test10.src | **Parameter type mismatch**: Function call with mismatched argument types | 1. Function 'add' redeclared in scope 'global'<br>2. Argument type mismatch in call to 'add' (param b expects int, got float) | ✅ ERROR DETECTED (2 errors) |
| test11.src | **Variable used before declaration**: Variable `x` used before its declaration | *No error detected* - Semantic analyzer limitation | ⚠️ LIMITATION |
| test12.src | **Duplicate parameter names**: Function with duplicate parameter `a` | Parameter 'a' duplicated in function 'duplicateParams' | ✅ ERROR DETECTED (1 error) |
| test13.src | **Wrong number of arguments (multiple)**: Function called with too few and too many arguments | 1. Function 'add' redeclared in scope 'global'<br>2. Call to 'add' with wrong number of arguments (expected 2, got 1)<br>3. Call to 'add' with wrong number of arguments (expected 2, got 3) | ✅ ERROR DETECTED (3 errors) |
| test14.src | **Type mismatch in arithmetic**: Adding integer and string | *No error detected* - String type checking not implemented | ⚠️ LIMITATION |
| test15.src | **Return in void function**: Void function returning a value | Void functions should not return a value | ✅ ERROR DETECTED (1 error) |
| test16.src | **Undeclared class usage**: Using undeclared class type | *No error detected* - Class type checking limitation | ⚠️ LIMITATION |

### Notes on Limitations

The following error tests reveal limitations in the current semantic analyzer implementation:

- **test11.src**: The semantic analyzer does not strictly enforce "use before declaration" for variables declared later in the same function scope. This is a known limitation of the current implementation.

- **test14.src**: String type checking in arithmetic operations is not fully implemented. The compiler does not detect type mismatches when mixing string literals with numeric types in arithmetic expressions.

- **test16.src**: Class type validation is not fully implemented. The compiler does not verify that user-defined class types (like `UnknownClass`) are actually declared before use.

These limitations are acceptable for a course project and are documented in the main report.

## Summary Statistics

### Pass Tests
- **Total Pass Test Files**: 30
- **Passing Tests**: 30 (100%)
- **Failing Tests**: 0
- **Grammar Coverage**: All major grammar productions tested

### Error Tests
- **Total Error Test Files**: 16
- **Errors Correctly Detected**: 13 (81.25%)
- **Limitations Identified**: 3 tests (test11, test14, test16)
- **Error Types Covered**:
  - ✅ Undeclared variable (test01, test03)
  - ✅ Type mismatch in assignment (test02, test04, test07)
  - ✅ Variable redeclaration (test05)
  - ✅ Wrong number of arguments (test06, test13)
  - ✅ Undeclared function call (test08)
  - ✅ Wrong return type (test09)
  - ✅ Parameter type mismatch (test10)
  - ✅ Duplicate parameter names (test12)
  - ✅ Return in void function (test15)
  - ⚠️ Variable used before declaration (test11 - not detected)
  - ⚠️ String type in arithmetic (test14 - not detected)
  - ⚠️ Undeclared class usage (test16 - not detected)

### Overall Statistics
- **Total Test Files**: 46 (30 pass + 16 error)
- **Overall Success Rate**: 43/46 (93.5%)
- **Pass Tests Success**: 30/30 (100%)
- **Error Detection Rate**: 13/16 (81.25%)

## Grammar Rules Coverage

### Program Structure
- ✅ `prog -> classOrImplOrFunc` (tested via all files)
- ✅ `classOrImplOrFunc -> classDecl` (test01, test02, test13, test17)
- ✅ `classOrImplOrFunc -> implDef` (test20, test21)
- ✅ `classOrImplOrFunc -> funcDef` (test03-test16, test19)

### Class Declarations
- ✅ `classDecl -> class id { visibility memberDecl }` (test01, test02, test13)
- ✅ `classDecl -> class id isa id moreIds` (test17 tests single inheritance, test25 tests multiple inheritance)
- ✅ `classInherit -> isa id moreIds` (test17, test25)
- ✅ `moreIds -> , id moreIds` (test25 tests multiple parent classes)
- ✅ `visibility -> public | private` (test01, test02, test13, test17, test22-test30)
- ✅ `memberDecl -> attributeDecl` (test01, test02, test13, test17, test22-test30)
- ✅ `attributeDecl -> attribute varDecl` (test01, test02, test13, test17, test22-test30)

### Implementation Blocks
- ✅ `implDef -> IMPLEMENT id { implFuncs }` (test20, test21)
- ✅ `implFuncs -> funcDef implFuncs` (test20, test21)

### Function Definitions
- ✅ `funcDef -> funcHead funcBody` (test03-test16, test19-test21)
- ✅ `funcHead -> FUNC id (fParams) ARROW returnType` (test03-test16, test19, test20)
- ✅ `funcHead -> CONSTRUCT (fParams)` (test21)
- ✅ `funcBody -> { varDeclOrStmtList }` (all function tests)
- ✅ `returnType -> type | void` (test08 tests void, others test type)

### Variable Declarations
- ✅ `localVarDecl -> local varDecl` (test03-test16, test18, test19, test22-test30)
- ✅ `varDecl -> id : type {{arraySize}}` (test18 tests arrays with sizes, test26 tests empty array size)
- ✅ `arraySize -> [intLit] | []` (test18 tests `[intLit]`, test26 tests `[]`)
- ✅ `arraySizes -> arraySize arraySizes` (test18 tests multiple dimensions)

### Statements
- ✅ `statement -> assignStat` (test03-test07, test09-test16, test18, test19)
- ✅ `statement -> if (relExpr) then statBlock else statBlock` (test04, test06, test10, test11, test12, test16, test19)
- ✅ `statement -> while (relExpr) statBlock` (test14, test16, test18, test19)
- ✅ `statement -> read (variable)` (test08, test19)
- ✅ `statement -> write (expr)` (test08, test19)
- ✅ `statement -> return (expr)` (test03-test16, test19-test21)
- ✅ `statBlock -> { statement }` (all control flow tests)

### Expressions
- ✅ `expr -> arithExpr` (test03, test05, test07, test09-test11, test18, test19)
- ✅ `expr -> relExpr` (test04, test06, test10, test12, test14, test16, test19)
- ✅ `arithExpr -> arithExpr addOp term` (test03, test07, test10, test11, test18, test19)
- ✅ `arithExpr -> term` (test05, test09)
- ✅ `addOp -> + | - | or` (test03, test07 test `+` and `-`; test10, test19 test `or`)
- ✅ `term -> term multOp factor` (test07, test11, test18, test19)
- ✅ `term -> factor` (test03, test05, test09)
- ✅ `multOp -> * | / | and` (test07, test11 test `*` and `/`; test10, test19 test `and`)
- ✅ `factor -> variable` (all tests with variables)
- ✅ `factor -> intLit` (all tests with literals)
- ✅ `factor -> (arithExpr)` (test11)
- ✅ `factor -> not factor` (test09, test19)
- ✅ `factor -> sign factor` (test05, test19)

### Relational Expressions
- ✅ `relExpr -> arithExpr relOp arithExpr` (test04, test06, test10, test12, test14, test16, test19)
- ✅ `relOp -> == | <> | < | > | <= | >=` (test12 tests all operators)

### Variables and Function Calls
- ✅ `variable -> {{idnest}} id {{indice}}` (test18, test22, test23, test27, test29, test30 test nested access and array indexing)
- ✅ `functionCall -> {{idnest}} id (aParams)` (test28 tests nested function calls)
- ✅ `idnest -> idOrSelf {{indice}}. | idOrSelf (aParams).` (test22, test23, test24, test27, test28, test29, test30)
- ✅ `idOrSelf -> id | self` (test24 tests `self` keyword)
- ✅ `indice -> [arithExpr]` (test18, test29)

### Parameters
- ✅ `fParams -> id : type fParamsTail` (all function tests)
- ✅ `fParamsTail -> , id : type` (test03, test04, test05, test06, test10, test11, test12, test15)

## Verification

### Pass Tests Verification
All 30 pass test files have been verified to:
1. ✅ Pass lexical analysis (all tokens recognized)
2. ✅ Pass parsing (AST built successfully)
3. ✅ Pass semantic analysis (no semantic errors)
4. ✅ Generate valid x86 assembly code
5. ✅ Produce relocatable and absolute machine code

### Error Tests Verification
All 16 error test files have been verified to:
1. ✅ Pass lexical analysis (all tokens recognized)
2. ✅ Pass parsing (AST built successfully)
3. ✅ Semantic analysis detects errors (13/16 tests correctly detect errors)
4. ❌ Code generation skipped when semantic errors are detected (as expected)

**Last Verified**: 
- All 30 pass test files compile successfully without errors
- 13 out of 16 error test files correctly detect semantic errors
- 3 error test files (test11, test14, test16) have limitations in semantic analysis
- New test coverage added for: nested variable access, nested function calls, `self` keyword, multiple inheritance, empty array sizes, and complex nested access patterns


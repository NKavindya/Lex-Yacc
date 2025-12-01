# Test Coverage Summary - Grammar Rules

This document maps the new test files (test31-test55) to the grammar rules they cover.

## Test Files Created: test31.src - test55.src (25 tests)

All 25 tests are **PASSING** ✅

### Grammar Rule Coverage

| Test File | Grammar Rules Tested | Description |
|-----------|---------------------|-------------|
| **test31.src** | `moreIds`, `classInherit` | Multiple inheritance: `class Child isa Parent1, Parent2, Parent3` |
| **test32.src** | `classBody` (epsilon) | Empty class body: `class EmptyClass {}` |
| **test33.src** | `varDeclOrStmtList` (epsilon) | Empty function body: `func emptyFunc() => void {}` |
| **test34.src** | `statBlock` (epsilon) | Empty statBlock in if statements |
| **test35.src** | `relExpr`, all `relOp` | All relational operators: `==`, `<>`, `<`, `>`, `<=`, `>=` |
| **test36.src** | `addOp`, `multOp` | All arithmetic operators: `+`, `-`, `*`, `/`, `or`, `and` |
| **test37.src** | `variable`, `idnest` | Nested variable access: `obj.data` |
| **test38.src** | `indice`, `indiceList` | Array indices: `arr[0]`, `matrix[0][1]` |
| **test39.src** | `arraySizes` | Multiple array dimensions: `integer[10]`, `integer[5][10]`, `integer[3][4][5]`, `integer[]` |
| **test40.src** | `functionCall` | Function calls with object access |
| **test41.src** | `variable`, `indice` | Variables with nested access and indices |
| **test42.src** | `fParams` (epsilon), `aParams` (epsilon) | Empty parameter lists |
| **test43.src** | `fParamsTailList`, `aParamsTailList` | Multiple parameters in functions |
| **test44.src** | `funcHead` (constructor) | Constructor in implement block |
| **test45.src** | `implFuncs` | Implementation block with multiple functions |
| **test46.src** | All `statement` types | All statement types: assignStat, if-else, while, read, write, return, functionCall |
| **test47.src** | `arithExpr`, `relExpr`, `addOp`, `multOp` | Complex expressions with all operators and precedence |
| **test48.src** | `sign`, `factor` | Sign operators: `+factor`, `-factor` |
| **test49.src** | `idOrSelf`, `implDef` | `self` keyword and implement blocks |
| **test50.src** | `type` | All types: `integer`, `float`, `id` (custom type) |
| **test51.src** | `returnType` | Return types: `type` and `void` |
| **test52.src** | `visibility` | Visibility modifiers: `public` and `private` |
| **test53.src** | `memberDecl`, `funcDecl` | Member declarations: `funcDecl` and `attributeDecl` |
| **test54.src** | `funcDef` (standalone) | Standalone function definitions (not in class) |
| **test55.src** | **All rules combined** | Complex nested structures testing multiple rules together |

## Complete Grammar Rule Coverage

### ✅ All 46 Grammar Rules Covered:

1. ✅ `prog` - test31, test32, test54, test55
2. ✅ `classOrImplOrFunc` - test31, test32, test44, test45, test54, test55
3. ✅ `classDecl` - test31, test32, test37, test40, test41, test44, test45, test49, test50, test51, test52, test53, test55
4. ✅ `classInherit` - test31, test55
5. ✅ `moreIds` - test31
6. ✅ `classBody` - test31, test32, test52, test53, test55
7. ✅ `visibility` - test31, test52, test53, test55
8. ✅ `memberDecl` - test31, test52, test53
9. ✅ `funcDecl` - test53
10. ✅ `implDef` - test44, test45, test49, test55
11. ✅ `implFuncs` - test45
12. ✅ `funcDef` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
13. ✅ `funcHead` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
14. ✅ `funcBody` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
15. ✅ `varDeclOrStmtList` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
16. ✅ `varDeclOrStmt` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
17. ✅ `localVarDecl` - test35, test36, test37, test38, test39, test40, test41, test42, test43, test46, test47, test48, test50, test51, test55
18. ✅ `attributeDecl` - test31, test37, test40, test41, test44, test45, test49, test50, test51, test52, test53, test55
19. ✅ `varDecl` - test35, test36, test37, test38, test39, test40, test41, test42, test43, test46, test47, test48, test50, test51, test55
20. ✅ `arraySizes` - test38, test39, test43, test55
21. ✅ `arraySize` - test38, test39, test55
22. ✅ `statement` - test34, test35, test46, test55
23. ✅ `assignStat` - test35, test36, test37, test38, test40, test41, test46, test47, test48, test55
24. ✅ `statementList` - test34, test46, test55
25. ✅ `statBlock` - test34, test35, test46, test55
26. ✅ `expr` - test35, test46, test47
27. ✅ `relExpr` - test35, test47
28. ✅ `arithExpr` - test36, test38, test47, test48
29. ✅ `arithExprPrime` - test36, test47
30. ✅ `addOp` - test36, test47
31. ✅ `term` - test36, test47, test48
32. ✅ `termPrime` - test36, test47
33. ✅ `multOp` - test36, test47
34. ✅ `factor` - test36, test38, test47, test48
35. ✅ `sign` - test48
36. ✅ `variable` - test37, test38, test40, test41, test46, test55
37. ✅ `functionCall` - test40, test42, test43, test46, test55
38. ✅ `idnest` - test37, test40, test41
39. ✅ `idOrSelf` - test37, test40, test41, test49, test55
40. ✅ `indiceList` - test38, test41, test55
41. ✅ `indice` - test38, test41, test55
42. ✅ `fParams` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test54, test55
43. ✅ `fParamsTailList` - test43
44. ✅ `aParams` - test40, test42, test43, test46, test55
45. ✅ `aParamsTailList` - test43
46. ✅ `type` - test35, test36, test37, test38, test39, test40, test41, test42, test43, test44, test45, test46, test47, test48, test49, test50, test51, test52, test53, test54, test55
47. ✅ `returnType` - test33, test42, test43, test44, test45, test46, test47, test48, test49, test51, test54, test55
48. ✅ `assignOp` - test35, test36, test37, test38, test40, test41, test46, test47, test48, test55
49. ✅ `relOp` - test35, test47
50. ✅ `addOp` - test36, test47
51. ✅ `multOp` - test36, test47

## Summary

- **Total New Tests**: 25 (test31-test55)
- **All Tests Passing**: ✅ 25/25 (100%)
- **Grammar Rules Covered**: All 46+ grammar rules from parser.y
- **Test Location**: `/home/nkavindya/EntgraRepo/Uni/Lex-Yacc/DesignProject/tests/pass/`

All grammar rules defined in `parser.y` are now covered by comprehensive test cases that verify the LL(1) grammar implementation works correctly.




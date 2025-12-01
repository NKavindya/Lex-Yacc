| Test File | Status | Key Grammar Rules Covered | Notes |
| --- | --- | --- | --- |
| grammar01_prog_classes.src | PASS | prog, classOrImplOrFunc, classDecl with inheritance (`isa` + `moreIds`), classBody (public/private), attributeDecl, varDecl with `arraySizes` (`[INT]`, `[]`), funcDecl/funcHead (`func`, `construct`), fParams (non-empty, ε), returnType (`void`, `float`, `id`), type (`integer`, `float`, `id`), implDef with empty implFuncs | Declaration-only coverage; also hits classBody ε via empty classes and implFuncs ε via `implement C { }`. |
| grammar02_impl_funcs.src | PASS | funcDef, funcBody, varDeclOrStmtList, localVarDecl, statement variants: assignStat, IF/THEN/ELSE, WHILE with `{}` block, WHILE with statement (statBlock→statement), WHILE with ε statBlock, READ, WRITE, RETURN, functionCall statement, aParams/aParamsTailList (multi-arg call), fParams/fParamsTailList (multi-params), statementList ε (empty `{}`), statBlock alternatives | Also exercises `and`/`or` precedence in conditions and return paths; uses both integer/float locals. |
| grammar03_expressions.src | PASS | Expression stack: expr→arithExpr/relExpr, relOp variants (`==`, `<>`, `<`, `>`, `<=`, `>=`), addOp (`+`, `-`, `or`), multOp (`*`, `/`, `and`), unary `sign` and `not`, factor forms (INT_LIT, FLOAT_LIT, `(arithExpr)`, variable, functionCall), indiceList with multiple indices, aParams ε and tail, recursive functionCall | Covers arithmetic/relational precedence and nested array indexing. |
| grammar04_chained_access.src | PASS | idnest/idOrSelf/variable chains (`self.child.value`, `self.getChild().value`), functionCall via idnest, indiceList with multiple dims, attribute access, type→ID, returnType→ID, attributeDecl with `[ ]` arrays, funcDef in implementations | Demonstrates dotted access on attributes and function results plus arrayed attributes. |

## Focused Grammar Test Cases

These test cases focus on specific grammar areas, providing targeted coverage for individual grammar rules.

| Test File | Status | Focus Area | Key Grammar Rules Covered | Notes |
| --- | --- | --- | --- | --- |
| test01_class_basic.src | PASS | Basic class declaration | classDecl → CLASS ID { classBody } ; | Simple class without inheritance |
| test02_class_inheritance.src | PASS | Class inheritance | classInherit → ISA ID moreIds \| epsilon, moreIds → COMMA ID moreIds \| epsilon | Single and multiple inheritance |
| test03_visibility_modifiers.src | PASS | Visibility modifiers | visibility → PUBLIC \| PRIVATE, classBody → visibility memberDecl classBody | Public and private members |
| test04_attributes_arrays.src | PASS | Array attributes | attributeDecl → ATTRIBUTE varDecl, arraySize → [ INT_LIT ] \| [ ] | Single, multi-dimensional, and dynamic arrays |
| test05_function_declarations.src | PASS | Function declarations | funcDecl → funcHead ;, funcHead → FUNC ID ( fParams ) ARROW returnType \| CONSTRUCT ( fParams ) | Function and constructor declarations |
| test06_return_types.src | PASS | Return types | returnType → type \| VOID, type → INTEGER_T \| FLOAT_T \| ID | All return type variants |
| test07_implement_block.src | PASS | Implement blocks | implDef → IMPLEMENT ID { implFuncs }, implFuncs → funcDef implFuncs \| epsilon | Implementation with function definitions |
| test08_local_variables.src | PASS | Local variables | localVarDecl → LOCAL varDecl, varDecl → ID : type arraySizes ; | Local variable declarations with arrays |
| test09_assignment_statements.src | PASS | Assignment statements | assignStat → variable ASSIGN expr, statement → assignStat ; | Simple and expression assignments |
| test10_if_statements.src | PASS | If statements | statement → IF LPAREN expr RPAREN THEN statBlock ELSE statBlock SEMICOLON | If-then-else with block and statement statBlocks |
| test11_while_loops.src | PASS | While loops | statement → WHILE LPAREN expr RPAREN statBlock SEMICOLON, statBlock → { statementList } \| statement \| epsilon | While with all statBlock variants |
| test12_read_write.src | PASS | Read/Write statements | statement → READ LPAREN variable RPAREN ; \| WRITE LPAREN expr RPAREN ; | Input/output operations |
| test13_return_statements.src | PASS | Return statements | statement → RETURN LPAREN expr RPAREN ; | Return with literals and expressions |
| test14_function_calls.src | PASS | Function calls | statement → functionCall ;, functionCall → ID ( aParams ), aParams → expr aParamsTailList \| epsilon | Function calls with and without parameters |
| test15_arithmetic_operators.src | PASS | Arithmetic operators | addOp → + \| - \| or, multOp → * \| / \| and | All arithmetic and logical operators |
| test16_relational_operators.src | PASS | Relational operators | relExpr → arithExpr relOp arithExpr, relOp → == \| <> \| < \| > \| <= \| >= | All relational comparison operators |
| test17_unary_operators.src | PASS | Unary operators | factor → sign factor \| not factor, sign → + \| - | Unary plus, minus, and not |
| test18_array_indexing.src | PASS | Array indexing | variable → {{idnest}} id {{indice}}, indice → [ arithExpr ], indiceList → indice indiceList \| epsilon | Single and multi-dimensional array access |
| test19_member_access.src | PASS | Member access | variable → idOrSelf DOT ID indiceList, idOrSelf → ID \| SELF | Dot operator for member access |
| test20_parameter_lists.src | PASS | Parameter lists | fParams → ID : type arraySizes fParamsTailList \| epsilon, fParamsTailList → COMMA ID : type arraySizes fParamsTailList \| epsilon | Functions with zero, one, and multiple parameters |

## Error Test Cases

| Test File | Expected Status | Error Type | Grammar Rule Violated | Notes |
| --- | --- | --- | --- | --- |
| error01_missing_semicolon.src | FAIL | Missing semicolon | statement → assignStat ; | Assignment statement missing required semicolon after expression |
| error02_missing_brace.src | FAIL | Missing closing brace | classDecl → CLASS ID ... { classBody } ; | Class declaration missing closing brace before semicolon |
| error03_missing_then.src | FAIL | Missing 'then' keyword | statement → IF ( expr ) THEN statBlock ELSE statBlock ; | If statement missing required 'then' keyword |
| error04_missing_parens.src | FAIL | Missing parentheses | functionCall → ID ( aParams ) | Function call missing parentheses around arguments |
| error05_invalid_assign_op.src | FAIL | Invalid assignment operator | assignStat → variable ASSIGN expr | Using '=' instead of ':=' for assignment |
| error06_missing_return_type.src | FAIL | Missing return type | funcHead → FUNC ID ( fParams ) ARROW returnType | Function declaration missing return type after arrow |
| error07_missing_colon.src | FAIL | Missing colon | varDecl → ID COLON type arraySizes ; | Variable declaration missing colon between identifier and type |
| error08_invalid_array_syntax.src | FAIL | Invalid array syntax | varDecl → ID COLON type arraySizes ; | Invalid array declaration syntax (missing brackets) |

All error test cases live under `tests/error/` and are designed to test parser error detection and recovery. Each test case should produce syntax errors that are recorded in `syntax_error.txt`. To run: `./compiler tests/error/<testfile>.src` - each is expected to fail parsing with appropriate error messages.

---

All four pass tests live under `tests/pass/` and are designed to touch every production or alternative in `parser.y` at least once (including ε branches). Existing legacy tests remain unchanged. To run: `make test` or invoke the compiler on each `.src` file; each is expected to parse and pass semantic checks.

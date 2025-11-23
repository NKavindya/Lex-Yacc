# TMA03 - Viva Questions and Answers

## Semantic Analysis - Comprehensive Q&A

This document contains 20+ potential viva questions for TMA03 (Semantic Analysis) with detailed explanatory answers.

---

### Q1: What is semantic analysis and why is it necessary in a compiler?

**Answer:**
Semantic analysis is the third phase of compilation that checks if a program makes sense beyond just syntax. While syntax analysis (parsing) verifies that tokens form valid grammatical structures, semantic analysis verifies:

1. **Type correctness**: Can you add an integer to a string? (No)
2. **Scope rules**: Is a variable declared before use?
3. **Declaration consistency**: Are functions called with correct arguments?
4. **Type compatibility**: Do assignment types match?

**Why it's necessary:**
- Syntax checking only ensures "the sentence is grammatically correct" but not "the sentence makes sense"
- Example: `x := y + "hello";` is syntactically valid but semantically invalid (can't add number to string)
- Catches errors that would cause runtime failures
- Enables type information for code generation

**In our implementation:**
- Pass A builds symbol tables (what's declared)
- Pass B checks types and semantics (how things are used)

---

### Q2: Explain the two-pass architecture of your semantic analyzer. Why use two passes instead of one?

**Answer:**
Our semantic analyzer uses **two separate passes** over the AST:

**Pass A (Symbol Table Construction)**:
- Traverses AST to collect all declarations
- Creates symbol table entries for classes, functions, variables, parameters, attributes
- Builds scope hierarchy (global → class → function scopes)
- Detects duplicate declarations

**Pass B (Semantic Checking)**:
- Traverses AST again to check usage
- Looks up identifiers in symbol tables
- Checks type compatibility in expressions, assignments, function calls
- Validates return types, control flow conditions

**Why two passes?**
1. **Forward references**: In Pass A, we need to see all declarations first. In Pass B, we can safely look up any identifier because all symbols are already in tables.
2. **Separation of concerns**: Declaration checking (Pass A) is different from usage checking (Pass B)
3. **Error reporting**: We can report "undeclared variable" errors only after we know what's declared
4. **Efficiency**: Build tables once, check many times

**Example:**
```c
func add(x : int) -> int {
    return(x + y);  // 'y' not declared - Pass B catches this
}
```
Pass A creates symbol table for `add` and `x`. Pass B checks `x + y` and finds `y` doesn't exist.

---

### Q3: How does your symbol table handle scoping? Explain with an example.

**Answer:**
Our symbol table uses **hierarchical scoping** with parent-child relationships:

**Structure:**
- Each scope (SymTable) has a `parent` pointer to its enclosing scope
- Symbols are stored in linked lists within each scope
- Lookup searches current scope, then climbs up parent chain

**Scope Hierarchy:**
```
global scope
├── class MyClass
│   └── class scope (parent = global)
│       ├── attribute x : int
│       └── func method()
│           └── function scope (parent = class scope)
│               ├── parameter y : int
│               └── local variable z : float
└── func standalone()
    └── function scope (parent = global)
        └── local variable a : int
```

**Lookup Algorithm:**
```c
Symbol *symtable_lookup(SymTable *table, const char *name) {
    // Search current scope
    for (Symbol *s = table->symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    // If not found, search parent scope
    if (table->parent) return symtable_lookup(table->parent, name);
    return NULL;  // Not found in any scope
}
```

**Example:**
```c
class MyClass {
    attribute x : int;
    func method(y : int) -> int {
        local z : float;
        return(x + y + z);  // x from class, y from function, z from function
    }
}
```
When resolving `x` in the function:
1. Check function scope → not found
2. Check class scope → found `x : int` ✅

---

### Q4: How do you detect and report duplicate declarations?

**Answer:**
Duplicate declarations are detected in **Pass A** during symbol table insertion:

**Mechanism:**
```c
int symtable_insert(SymTable *table, const char *name, const char *typeName, 
                    SymKind kind, int lineno) {
    // Check if symbol already exists in current scope
    if (symtable_lookup(table, name)) {
        return 1;  // Duplicate found
    }
    // Insert new symbol
    // ...
    return 0;  // Success
}
```

**Detection Points:**
1. **Function redeclaration**: Same function name in same scope
2. **Variable redeclaration**: Same variable name in same scope
3. **Parameter duplication**: Same parameter name in function signature
4. **Class redeclaration**: Same class name in global scope

**Error Reporting:**
```c
if (symtable_insert(curScope, node->name, ..., SYM_FUNC, node->lineno)) {
    sem_error(node->lineno,
              "Function '%s' redeclared in scope '%s'",
              node->name, curScope->scopeName);
}
```

**Example:**
```c
func add(x : int, x : float) -> int {  // Error: parameter 'x' duplicated
    local result : int;
    local result : float;  // Error: variable 'result' redeclared
    return(result);
}
```

---

### Q5: Explain how type checking works for binary operations. What types are compatible?

**Answer:**
Type checking for binary operations is performed in **Pass B** using `resolve_type_of_expr()`:

**Arithmetic Operations** (`+`, `-`, `*`, `/`):
- Both operands must be numeric (`int` or `float`)
- Result type: `float` if either operand is `float`, otherwise `int`
- Error if operands are strings or other types

```c
case NODE_BINARY_OP:
    if (operator is +, -, *, /) {
        if (left is numeric && right is numeric) {
            if (left is float || right is float) return "float";
            return "int";
        } else {
            error("Arithmetic operands must be numeric");
        }
    }
```

**Relational Operations** (`==`, `<>`, `<`, `>`, `<=`, `>=`):
- Operands must be same type OR both numeric
- Result type: always `int` (boolean result)
- Error if incompatible types

**Logical Operations** (`and`, `or`):
- Both operands must be `int`
- Result type: `int`
- Error if operands are not integers

**Examples:**
```c
5 + 3.14        // ✅ int + float → float
"hello" + 5     // ❌ Error: string + int
5 == 3          // ✅ int == int → int
5 < 3.14        // ✅ int < float → int
"a" < "b"       // ✅ string < string → int
5 and 3         // ✅ int and int → int
5 and 3.14      // ❌ Error: int and float
```

---

### Q6: How do you handle type promotion (e.g., int to float) in assignments?

**Answer:**
Type promotion allows assigning `int` to `float` variables, which is a common and safe operation:

**Implementation:**
```c
static void check_assignment(AST *node, SymTable *scope) {
    const char *lt = resolve_type_of_expr(scope, lhs);  // left type
    const char *rt = resolve_type_of_expr(scope, rhs);  // right type
    
    if (strcmp(lt, rt) != 0) {
        // Allow int → float promotion
        if (strcmp(lt, "float") == 0 && strcmp(rt, "int") == 0) {
            return;  // ✅ Allowed
        }
        // Other mismatches are errors
        sem_error(node->lineno, "Type mismatch: left is %s, right is %s", lt, rt);
    }
}
```

**Rules:**
- ✅ `float x; x := 5;` (int promoted to float)
- ✅ `int x; x := 5;` (same type)
- ❌ `int x; x := 3.14;` (float to int - loss of precision, not allowed)
- ❌ `string x; x := 5;` (incompatible types)

**Why allow int → float?**
- No information loss (int can be exactly represented as float)
- Common in programming languages
- Makes code more flexible

---

### Q7: Describe how function call checking works. What aspects are validated?

**Answer:**
Function call checking validates three aspects:

**1. Function Existence:**
```c
Symbol *fn = symtable_lookup(curScope, functionName);
if (!fn || fn->kind != SYM_FUNC) {
    error("Call to undefined function '%s'", functionName);
}
```

**2. Argument Count:**
```c
int argCount = 0, paramCount = 0;
for (AST *a = expr->child; a; a = a->sibling) argCount++;
for (Symbol *p = fn->params; p; p = p->next) paramCount++;

if (argCount != paramCount) {
    error("Call to '%s' with wrong number of arguments (expected %d, got %d)",
          functionName, paramCount, argCount);
}
```

**3. Argument Types:**
```c
AST *arg = expr->child;
Symbol *param = fn->params;
while (arg && param) {
    const char *argType = resolve_type_of_expr(curScope, arg);
    if (strcmp(argType, param->typeName) != 0) {
        error("Argument type mismatch: param %s expects %s, got %s",
              param->name, param->typeName, argType);
    }
    arg = arg->sibling;
    param = param->next;
}
```

**Example:**
```c
func add(x : int, y : float) -> float { ... }

add(5, 3.14);        // ✅ Correct: 2 args, types match
add(5);              // ❌ Error: wrong number of arguments
add(5, 3.14, 10);    // ❌ Error: wrong number of arguments
add(5, "hello");     // ❌ Error: argument type mismatch (y expects float, got string)
```

---

### Q8: How are return statements validated? What checks are performed?

**Answer:**
Return statement validation checks:

**1. Return Context:**
- Must be inside a function (not at global level)
- Tracks expected return type from function declaration

**2. Void Functions:**
- If function return type is `void`, return statement must not have a value
- Error: `return(5);` in void function

**3. Non-Void Functions:**
- Return expression type must match function's declared return type
- Allows `int` → `float` promotion (if function returns float)

**Implementation:**
```c
case NODE_RETURN: {
    const char *exprType = resolve_type_of_expr(scope, p->child);
    const char *expectedReturn = currentReturn;  // from function declaration
    
    if (strcmp(expectedReturn, "void") == 0) {
        if (exprType && exprType != "<nil>" && exprType != "<void>") {
            error("Void functions should not return a value");
        }
    } else {
        if (strcmp(exprType, expectedReturn) != 0) {
            // Allow int → float promotion
            if (!(strcmp(expectedReturn, "float") == 0 && strcmp(exprType, "int") == 0)) {
                error("Return type mismatch: expected %s, got %s",
                      expectedReturn, exprType);
            }
        }
    }
    break;
}
```

**Examples:**
```c
func add(x : int) -> int {
    return(x + 1);        // ✅ int returns int
    return(3.14);         // ❌ Error: float returns int
}

func print() -> void {
    return();             // ✅ void returns nothing
    return(5);           // ❌ Error: void function returning value
}

func getFloat() -> float {
    return(5);           // ✅ int promoted to float
    return(3.14);        // ✅ float returns float
}
```

---

### Q9: Explain how control flow statements (if, while) are type-checked.

**Answer:**
Control flow statements require their conditions to be **numeric** (not strings):

**Implementation:**
```c
static void check_condition(AST *condNode, SymTable *scope, const char *keyword) {
    const char *t = resolve_type_of_expr(scope, condNode);
    if (!is_numeric_type(t)) {  // is_numeric_type checks for "int" or "float"
        sem_error(condNode->lineno,
                  "%s condition must be numeric (found %s)",
                  keyword, t);
    }
}

// Usage:
case NODE_IF: {
    AST *cond = p->child;
    check_condition(cond, scope, "IF");
    // ...
}

case NODE_WHILE: {
    AST *cond = p->child;
    check_condition(cond, scope, "WHILE");
    // ...
}
```

**Why numeric?**
- Conditions are evaluated as true/false
- Numeric values: `0` = false, non-zero = true
- Strings cannot be meaningfully evaluated as boolean

**Examples:**
```c
if (x > 5) { ... }           // ✅ x > 5 is numeric (int)
if (x + y) { ... }           // ✅ x + y is numeric
if ("hello") { ... }         // ❌ Error: string condition
if (x == "test") { ... }     // ✅ x == "test" is int (relational result)
while (count < 10) { ... }   // ✅ count < 10 is numeric
while ("loop") { ... }       // ❌ Error: string condition
```

---

### Q10: How does your semantic analyzer handle nested scopes and variable shadowing?

**Answer:**
Our implementation uses **lexical scoping** (static scoping):

**Lookup Behavior:**
- Always searches current scope first
- If not found, searches parent scope
- Continues up the scope chain to global
- First match wins (inner scope shadows outer)

**Example:**
```c
func outer() -> void {
    local x : int;        // x in outer scope
    func inner() -> void {
        local x : float;  // x in inner scope (shadows outer x)
        x := 3.14;        // Uses inner x (float)
    }
    x := 5;               // Uses outer x (int)
}
```

**Implementation:**
```c
Symbol *symtable_lookup(SymTable *table, const char *name) {
    // Search current scope first (inner scope takes precedence)
    for (Symbol *s = table->symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    // If not found, search parent (outer scope)
    if (table->parent) return symtable_lookup(table->parent, name);
    return NULL;
}
```

**Shadowing Rules:**
- ✅ Inner scope can declare same name as outer scope (shadowing allowed)
- ❌ Same scope cannot redeclare (duplicate error)
- Lookup always finds the **nearest** declaration

**Note:** Our implementation doesn't warn about shadowing - it's allowed but could be flagged as a warning in a production compiler.

---

### Q11: What is the difference between a syntax error and a semantic error? Give examples.

**Answer:**

**Syntax Error:**
- Violates grammar rules
- Detected during parsing
- Example: Missing semicolon, unmatched parentheses, invalid token sequence

**Examples:**
```c
x := 5        // ❌ Syntax: missing semicolon
if (x > 5 {   // ❌ Syntax: missing closing parenthesis
x := + 5      // ❌ Syntax: invalid operator sequence
```

**Semantic Error:**
- Grammatically correct but meaningless
- Detected during semantic analysis
- Example: Type mismatch, undeclared variable, wrong function arguments

**Examples:**
```c
x := "hello" + 5;        // ❌ Semantic: can't add string to int
y := z;                  // ❌ Semantic: z not declared
func add(x : int) -> int { return(x); }
add(5, 10);              // ❌ Semantic: wrong number of arguments
```

**Key Difference:**
- **Syntax**: "Is this valid according to the grammar?"
- **Semantic**: "Does this make sense? Are types compatible? Are identifiers declared?"

**In our compiler:**
- Syntax errors: Reported by parser (`yyerror()`)
- Semantic errors: Reported by semantic analyzer (`sem_error()`)

---

### Q12: How do you ensure semantic errors are reported in a synchronized order?

**Answer:**
Semantic errors are collected during both passes but reported in **synchronized order**:

**Mechanism:**
```c
#define MAX_ERRORS 1000
static char *errorMsgs[MAX_ERRORS];
static int errorCount = 0;

static void sem_error(int lineno, const char *fmt, ...) {
    // Format error message with line number
    char fullMsg[1200];
    snprintf(fullMsg, sizeof(fullMsg), "Semantic Error [line %d]: %s", lineno, buffer);
    
    // Deduplicate
    if (!already_reported(fullMsg)) {
        if (errorCount < MAX_ERRORS)
            errorMsgs[errorCount++] = strdup(fullMsg);
        
        // Write to file immediately
        if (errFile) fprintf(errFile, "%s\n", fullMsg);
    }
}
```

**Synchronization:**
1. All errors stored in array with line numbers
2. Errors written to file as they're discovered
3. Line numbers ensure chronological order
4. Deduplication prevents same error reported multiple times

**Example Output:**
```
Semantic Error [line 1]: Parameter 'x' duplicated in function 'add'
Semantic Error [line 4]: Identifier 'y' used before declaration
Semantic Error [line 6]: Arithmetic operands must be numeric (found string and int)
Semantic Error [line 8]: Return type mismatch: expected int, got float
```

**Why synchronized?**
- Makes debugging easier (errors in source order)
- Consistent reporting regardless of pass order
- Professional error reporting standard

---

### Q13: Explain the AST traversal strategy used in semantic analysis.

**Answer:**
We use **depth-first traversal** (recursive tree walk):

**Pass A Strategy:**
```c
static void semantic_passA_build(SymTable *curScope, AST *node) {
    if (!node) return;
    
    switch (node->kind) {
        case NODE_CLASS_DECL:
            // Create class scope, insert class symbol
            // Recursively process class body
            break;
        case NODE_FUNC_DECL:
            // Create function scope, insert function symbol
            // Process parameters, then body
            break;
        // ... other cases
    }
    
    // Process siblings (next in list)
    if (node->sibling)
        semantic_passA_build(curScope, node->sibling);
}
```

**Traversal Order:**
1. **Pre-order**: Process node before children (for declarations)
2. **Depth-first**: Process all children before siblings
3. **Scope-aware**: Create new scope before processing children

**Example AST:**
```
NODE_PROGRAM
├── NODE_FUNC_DECL ("add")
│   ├── NODE_PARAM ("x")
│   └── NODE_FUNC_BODY
│       ├── NODE_VAR_DECL ("result")
│       └── NODE_ASSIGN
│           ├── NODE_ID ("result")
│           └── NODE_BINARY_OP ("+")
│               ├── NODE_ID ("x")
│               └── NODE_INT_LITERAL (1)
```

**Traversal Sequence:**
1. NODE_PROGRAM (create global scope)
2. NODE_FUNC_DECL (create function scope, insert function)
3. NODE_PARAM (insert parameter)
4. NODE_FUNC_BODY
5. NODE_VAR_DECL (insert variable)
6. NODE_ASSIGN
7. NODE_ID ("result") - lookup in Pass B
8. NODE_BINARY_OP - type check in Pass B
9. NODE_ID ("x") - lookup in Pass B
10. NODE_INT_LITERAL - type is "int"

---

### Q14: How do you handle array types and array indexing in semantic analysis?

**Answer:**
Arrays are handled through the `arraySize` grammar production:

**Declaration:**
```c
local arr : int[10];     // Array of 10 integers
local matrix : float[][]; // Multi-dimensional array
```

**AST Structure:**
- `NODE_VAR_DECL` with `arraySizes` children
- Each `arraySize` represents one dimension

**Type Information:**
- Base type stored in `varDecl->typeName` (e.g., "int")
- Array dimensions stored as AST children
- Symbol table stores base type, array-ness tracked separately

**Indexing:**
```c
arr[5]           // Single index
matrix[2][3]     // Multiple indices
```

**Type Checking:**
- Array indexing: `arr[expr]` - `expr` must be numeric (integer)
- Result type: Base type of array (e.g., `arr[5]` has type `int`)

**Current Implementation:**
- Arrays are declared and stored
- Index bounds checking is **not** implemented (would require runtime)
- Type checking ensures indices are numeric
- Array access returns base type

**Future Enhancement:**
- Could add static bounds checking for constant indices
- Could track array dimensions in symbol table
- Could validate index count matches dimension count

---

### Q15: What semantic rules are checked for READ and WRITE statements?

**Answer:**

**READ Statement:**
```c
read(variable);
```

**Checks:**
1. Operand must be an identifier (not expression)
2. Identifier must be declared
3. Type is not restricted (can read into any variable)

**Implementation:**
```c
case NODE_READ: {
    AST *v = p->child;
    if (!v || v->kind != NODE_ID) {
        sem_error(p->lineno, "READ expects an identifier");
    } else if (!symtable_lookup(scope, v->name)) {
        sem_error(v->lineno, "READ on undeclared variable '%s'", v->name);
    }
    break;
}
```

**WRITE Statement:**
```c
write(expression);
```

**Checks:**
1. Operand can be any expression (not just identifier)
2. Expression type is resolved (for validation)
3. No type restriction (can write any type)

**Implementation:**
```c
case NODE_WRITE: {
    (void)resolve_type_of_expr(scope, p->child);  // Resolve type, report errors
    break;
}
```

**Examples:**
```c
read(x);              // ✅ x is declared identifier
read(x + y);          // ❌ Error: READ expects identifier, not expression
read(undeclared);     // ❌ Error: undeclared variable

write(5);             // ✅ Can write integer
write(3.14);          // ✅ Can write float
write("hello");       // ✅ Can write string
write(x + y);         // ✅ Can write expression result
```

---

### Q16: How are class attributes and member functions handled differently from local variables?

**Answer:**

**Class Attributes:**
- Declared in class body with `attribute` keyword
- Stored in **class scope** (not global or function scope)
- Accessible to all methods in the class
- Symbol kind: `SYM_ATTR`

**Member Functions:**
- Declared in class body (without `attribute`)
- Stored in **class scope**
- Can access class attributes via `self.attr`
- Symbol kind: `SYM_FUNC`

**Local Variables:**
- Declared in function body with `local` keyword
- Stored in **function scope**
- Only accessible within that function
- Symbol kind: `SYM_VAR`

**Scope Hierarchy:**
```
global scope
└── class MyClass
    └── class scope
        ├── attribute x : int        (SYM_ATTR, in class scope)
        └── func method()            (SYM_FUNC, in class scope)
            └── function scope
                └── local y : int   (SYM_VAR, in function scope)
```

**Access Rules:**
```c
class MyClass {
    attribute x : int;
    func method() -> int {
        local y : int;
        return(x + y);  // ✅ x from class scope, y from function scope
    }
}
```

**Implementation:**
- Pass A: Creates class scope, inserts attributes and functions into class scope
- Pass B: Function scope can lookup in parent (class scope) for attributes

---

### Q17: Explain how unary operators are type-checked.

**Answer:**
Unary operators are checked in `resolve_type_of_expr()`:

**Unary NOT (`not`):**
- Operand must be `int`
- Result type: `int`
- Used for logical negation

```c
case NODE_UNARY_OP:
    if (strcmp(expr->name, "not") == 0) {
        const char *operand = resolve_type_of_expr(curScope, expr->child);
        if (strcmp(operand, "int") == 0) {
            return "int";
        } else {
            error("Operand of 'not' must be integer (found %s)", operand);
        }
    }
```

**Unary Sign (`+`, `-`):**
- Operand must be numeric (`int` or `float`)
- Result type: Same as operand type
- Used for positive/negative values

```c
if (strcmp(expr->name, "+") == 0 || strcmp(expr->name, "-") == 0) {
    const char *operand = resolve_type_of_expr(curScope, expr->child);
    if (is_numeric_type(operand)) {
        return operand;  // Same type as operand
    } else {
        error("Unary %s expects numeric operand (found %s)", expr->name, operand);
    }
}
```

**Examples:**
```c
not 5           // ✅ int → int
not 3.14        // ❌ Error: float operand
not "hello"     // ❌ Error: string operand

-5              // ✅ int → int
-3.14           // ✅ float → float
-"hello"        // ❌ Error: string operand

+10             // ✅ int → int
+2.5            // ✅ float → float
```

---

### Q18: What happens when a semantic error is detected? Does the analysis continue?

**Answer:**
Yes, semantic analysis **continues** after detecting errors (error recovery):

**Strategy:**
1. **Error is reported** with line number and message
2. **Error is stored** in error array (for counting)
3. **Analysis continues** to find more errors
4. **Type resolution** returns error type (`"<error>"`) to prevent cascading errors

**Implementation:**
```c
static const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    // ... type resolution ...
    if (error detected) {
        sem_error(lineno, "Error message");
        return "<error>";  // Return error type
    }
    return actualType;
}

// Later checks skip if type is "<error>"
if (strcmp(lt, "<error>") == 0 || strcmp(rt, "<error>") == 0) {
    return;  // Skip further checking for this expression
}
```

**Benefits:**
- **Multiple errors reported**: User sees all problems at once
- **Better debugging**: Don't stop at first error
- **Cascading prevention**: Error type prevents false errors downstream

**Example:**
```c
func test() -> int {
    x := "hello" + 5;      // Error 1: string + int
    y := x + 10;           // Error 2: x is error type, but still reports
    return(y + z);         // Error 3: z undeclared
}
```

All three errors are reported, not just the first one.

---

### Q19: How do you handle function parameters in the symbol table?

**Answer:**
Function parameters are handled in two ways:

**1. Attached to Function Symbol:**
- Parameters stored as linked list in `Symbol->params`
- Allows checking function call arguments
- Each parameter is a `Symbol` with `kind = SYM_PARAM`

**2. Inserted into Function Scope:**
- Parameters also inserted into function's symbol table
- Allows parameters to be used in function body
- Lookup finds parameters as regular symbols

**Implementation:**
```c
case NODE_FUNC_DECL: {
    // Create function scope
    SymTable *fnScope = symtable_create(node->name, curScope);
    
    // Get parameter list
    AST *param = node->child;
    Symbol *funcSym = symtable_lookup(curScope, node->name);
    
    // Bind parameters to function symbol
    bind_function_params(funcSym, param);
    
    // Insert parameters into function scope
    for (AST *pp = param; pp; pp = pp->sibling) {
        symtable_insert(fnScope, pp->name, pp->typeName, SYM_PARAM, pp->lineno);
    }
}
```

**Structure:**
```
Function Symbol ("add")
├── name: "add"
├── typeName: "int"
├── params: → Parameter Symbol ("x")
│             ├── name: "x"
│             ├── typeName: "int"
│             └── next: → Parameter Symbol ("y")
│                          └── ...
└── ...

Function Scope ("add")
└── symbols: → Parameter Symbol ("x")
              └── next: → Parameter Symbol ("y")
```

**Usage:**
- Function call checking: Traverse `funcSym->params` to check arguments
- Function body: Lookup parameters in function scope like regular variables

---

### Q20: What is the purpose of the `extra` field in AST nodes? How is it used in semantic analysis?

**Answer:**
The `extra` field is an **auxiliary pointer** used for additional data:

**Common Uses:**
1. **Function Bodies**: `NODE_FUNC_DECL->extra` points to `NODE_FUNC_BODY`
2. **Alternative child lists**: When a node needs multiple child lists

**In Semantic Analysis:**
```c
case NODE_FUNC_DECL: {
    // node->child = parameter list
    // node->extra = function body
    
    AST *param = node->child;      // Parameters
    AST *body = node->extra;       // Function body
    
    // Process parameters
    for (AST *pp = param; pp; pp = pp->sibling) {
        symtable_insert(fnScope, pp->name, ..., SYM_PARAM, ...);
    }
    
    // Process body
    if (body && body->child) {
        for (AST *st = body->child; st; st = st->sibling) {
            // Process statements in body
        }
    }
}
```

**Why `extra` instead of `child->sibling`?**
- **Clarity**: Separates parameters (child) from body (extra)
- **Structure**: Function has two distinct parts
- **Flexibility**: Can have different traversal strategies

**AST Structure:**
```
NODE_FUNC_DECL ("add")
├── child → NODE_PARAM ("x")
│           └── sibling → NODE_PARAM ("y")
└── extra → NODE_FUNC_BODY
            └── child → NODE_VAR_DECL ("result")
                        └── sibling → NODE_ASSIGN
```

---

### Q21: How would you extend the semantic analyzer to support inheritance checking?

**Answer:**
Inheritance checking would be added in **Pass A**:

**Current Support:**
- Grammar supports `class MyClass isa BaseClass { ... }`
- AST includes `NODE_CLASS_INHERIT_LIST`
- Inheritance list is stored but not validated

**Extension for Inheritance Checking:**

**1. Inheritance Validation:**
```c
case NODE_CLASS_DECL: {
    // Check inheritance list
    AST *inheritList = get_inherit_list(node);
    for (AST *base = inheritList; base; base = base->sibling) {
        Symbol *baseClass = symtable_lookup(globalTable, base->name);
        if (!baseClass || baseClass->kind != SYM_CLASS) {
            error("Class '%s' inherits from undefined class '%s'",
                  node->name, base->name);
        }
    }
    // ... rest of class processing
}
```

**2. Circular Inheritance Detection:**
```c
static int check_inheritance_cycle(const char *className, const char *baseName, 
                                    Set *visited) {
    if (strcmp(className, baseName) == 0) {
        return 1;  // Cycle detected
    }
    if (set_contains(visited, baseName)) {
        return 1;  // Already visited
    }
    set_add(visited, baseName);
    // Check base's inheritance
    // ...
}
```

**3. Attribute/Method Inheritance:**
- Inherited members accessible in derived class
- Lookup would check class scope, then base class scopes
- Overriding rules (if method overridden, signature must match)

**Current Status:**
- Inheritance syntax is parsed
- Inheritance checking is **not implemented** (would be enhancement)

---

### Q22: Explain the difference between Pass A and Pass B in terms of what information they need and produce.

**Answer:**

**Pass A (Symbol Table Construction):**

**Needs:**
- AST with all declaration nodes
- Current scope context

**Produces:**
- Complete symbol table hierarchy
- Scope structure (global → class → function)
- Symbol entries with names, types, kinds, locations

**Information Flow:**
```
AST (declarations) → Pass A → Symbol Tables
```

**What Pass A Does:**
- Creates scopes
- Inserts symbols
- Links scopes (parent-child)
- Detects duplicates

**Pass B (Semantic Checking):**

**Needs:**
- AST with all nodes (declarations + usage)
- Symbol tables (from Pass A)
- Current scope context
- Expected return type (for return validation)

**Produces:**
- Semantic error reports
- Type information for expressions
- Validation results

**Information Flow:**
```
AST (all nodes) + Symbol Tables → Pass B → Error Reports
```

**What Pass B Does:**
- Looks up identifiers
- Checks types
- Validates operations
- Reports errors

**Key Difference:**
- **Pass A**: "What is declared?" → Builds symbol tables
- **Pass B**: "How is it used?" → Checks usage against declarations

**Why Separate:**
- Pass A must complete before Pass B can look up symbols
- Clear separation of concerns
- Easier to debug and maintain

---

### Q23: How do you handle the case where a variable is used before it's declared?

**Answer:**
This is detected in **Pass B** during identifier lookup:

**Detection:**
```c
case NODE_ID: {
    Symbol *s = symtable_lookup(curScope, expr->name);
    if (!s) {
        sem_error(expr->lineno,
                  "Identifier '%s' used before declaration",
                  expr->name);
        return "<error>";
    }
    return s->typeName;
}
```

**Lookup Behavior:**
- Searches current scope first
- Then parent scopes up to global
- Returns `NULL` if not found anywhere
- Error reported with line number

**Example:**
```c
func test() -> int {
    x := y + 5;      // ❌ Error: 'y' used before declaration
    local y : int;
    return(x);
}
```

**Why This Happens:**
- In our language, declarations can appear anywhere in function body
- Some languages require declarations at top (would prevent this)
- Our analyzer catches it and reports error

**Note:**
- This is a **semantic error**, not syntax error
- Syntax is valid: `x := y + 5;` is grammatically correct
- Semantic check: `y` must exist in symbol table

---

### Q24: What would happen if you tried to do semantic checking in a single pass instead of two?

**Answer:**
Single-pass semantic analysis would face several problems:

**Problem 1: Forward References**
```c
func test() -> int {
    x := y + 5;      // Uses 'y' before it's declared
    local y : int;
    return(x);
}
```
- In single pass, when checking `x := y + 5`, `y` isn't in symbol table yet
- Would incorrectly report "y undeclared" even though it's declared later

**Problem 2: Function Call Before Declaration**
```c
func caller() -> int {
    return(callee());  // Calls 'callee' before it's declared
}

func callee() -> int {
    return(5);
}
```
- Single pass would report "callee undefined" even though it exists

**Problem 3: Circular Dependencies**
- Harder to detect without complete symbol table

**Solution with Two Passes:**
1. **Pass A**: Build complete symbol table (all declarations known)
2. **Pass B**: Check all usage (can safely look up any identifier)

**Trade-offs:**
- **Two passes**: More memory (store symbol tables), but accurate
- **Single pass**: Less memory, but misses some errors or requires complex forward declaration handling

**Our Choice:**
- Two passes for accuracy and clarity
- Standard approach in production compilers
- Worth the extra traversal for correctness

---

### Q25: How do you ensure that the semantic analyzer correctly handles nested function calls and complex expressions?

**Answer:**
The semantic analyzer uses **recursive type resolution**:

**Recursive Resolution:**
```c
static const char *resolve_type_of_expr(SymTable *curScope, AST *expr) {
    switch (expr->kind) {
        case NODE_BINARY_OP: {
            // Recursively resolve left and right operands
            const char *lt = resolve_type_of_expr(curScope, expr->child);
            const char *rt = resolve_type_of_expr(curScope, 
                                                  expr->child->sibling);
            // Check compatibility and return result type
            return resultType;
        }
        case NODE_FUNCTION_CALL: {
            // Lookup function
            Symbol *fn = symtable_lookup(curScope, expr->name);
            // Recursively check each argument
            AST *arg = expr->child;
            while (arg) {
                const char *argType = resolve_type_of_expr(curScope, arg);
                // Validate against parameter
                arg = arg->sibling;
            }
            return fn->typeName;  // Return function's return type
        }
        // ... other cases
    }
}
```

**Complex Expression Example:**
```c
result := add(multiply(x, y), divide(a, b)) + 5;
```

**Resolution Order:**
1. `multiply(x, y)` → recursively resolve `x`, `y`, check function call
2. `divide(a, b)` → recursively resolve `a`, `b`, check function call
3. `add(..., ...)` → recursively resolve both arguments (results from steps 1, 2)
4. `... + 5` → recursively resolve left (result from step 3) and right (5)
5. Final type checked for assignment

**Key Points:**
- **Depth-first**: Resolves innermost expressions first
- **Bottom-up**: Type information flows from leaves to root
- **Error propagation**: Errors in sub-expressions prevent false errors in parent

---

## Summary

These 25 questions cover:
- ✅ Two-pass architecture and rationale
- ✅ Symbol table structure and scoping
- ✅ Type checking for all operations
- ✅ Function call validation
- ✅ Error detection and reporting
- ✅ AST traversal strategies
- ✅ Special cases (arrays, inheritance, forward references)
- ✅ Design decisions and trade-offs

All answers are based on the actual implementation in `semantic.c` and `symbol_table.c`.



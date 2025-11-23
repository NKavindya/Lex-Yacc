# TMA03 - Viva Questions and Answers

## Comprehensive Q&A for TMA03 (Lexical, Syntax, and Semantic Analysis)

This document contains 15 potential viva questions for TMA03 covering lexical analysis, syntax analysis (LL(1) parsing), and semantic analysis, with detailed explanatory answers based on the actual implementation.

---

### Q1: What is lexical analysis and what are its main responsibilities?

**Answer:**

Lexical analysis (also called scanning or tokenization) is the **first phase** of compilation. It reads the source code character by character and groups them into meaningful units called **tokens**.

**Main Responsibilities:**

1. **Token Recognition**: Identify and classify tokens (keywords, identifiers, operators, literals, punctuation)
2. **Lexeme Extraction**: Extract the actual text (lexeme) for each token
3. **Location Tracking**: Record line and column numbers for error reporting
4. **Whitespace Handling**: Skip whitespace and comments (they don't become tokens)
5. **Symbol Table Building**: Record identifiers and literals in a symbol table
6. **Error Detection**: Detect lexical errors (malformed numbers, unterminated comments, unknown characters)

**In Our Implementation:**

- **File**: `scanner.l` (Flex specification)
- **Output**: Token stream with type, lexeme, and location
- **Artifacts**: `lexer_tokens.txt` (all tokens), `lexer_symbols.txt` (symbol table), `lexer_errors.txt` (lexical errors)

**Example:**
```
Source: "x := 5 + 3.14;"
Tokens: ID("x"), ASSIGN(":="), INT_LIT(5), PLUS("+"), FLOAT_LIT(3.14), SEMICOLON(";")
```

---

### Q2: Explain the regular expressions used in your lexical analyzer for recognizing identifiers, integers, and floats.

**Answer:**

**Identifiers:**
```
ID_START        [A-Za-z_]
ID_PART         [A-Za-z0-9_]
ID              {ID_START}{ID_PART}*
```

- **ID_START**: First character must be a letter (A-Z, a-z) or underscore
- **ID_PART**: Subsequent characters can be letters, digits, or underscores
- **ID**: One start character followed by zero or more part characters
- **Examples**: `x`, `myVar`, `_temp`, `count123` ✅
- **Invalid**: `123abc` (starts with digit), `my-var` (contains hyphen) ❌

**Integers:**
```
INT_LITERAL     0|[1-9][0-9]*
```

- **Pattern 1**: `0` - single zero
- **Pattern 2**: `[1-9][0-9]*` - non-zero digit followed by zero or more digits
- **Examples**: `0`, `5`, `123`, `9999` ✅
- **Invalid**: `0123` (leading zero not allowed for non-zero numbers) ❌

**Floats:**
```
EXP             [eE][+-]?{DIGIT}+
FLOAT_LITERAL   ({INT_LITERAL}"."{DIGIT}+({EXP})?)|({INT_LITERAL}{EXP})
```

- **Two forms**:
  1. `integer.fraction[exponent]` - e.g., `3.14`, `2.5e10`, `1.0E-5`
  2. `integer[exponent]` - e.g., `5e3`, `10E-2`
- **Fraction**: `.` followed by one or more digits
- **Exponent**: `e` or `E`, optional `+` or `-`, one or more digits
- **Examples**: `3.14`, `2.5e10`, `1.0E-5`, `5e3` ✅
- **Invalid**: `.5` (missing integer part), `3.` (missing fraction), `3e` (missing exponent digits) ❌

**Why These Patterns?**
- **Identifiers**: Must start with letter/underscore to distinguish from numbers
- **Integers**: No leading zeros (except `0` itself) to avoid ambiguity with octal
- **Floats**: Two forms cover all scientific notation cases

---

### Q3: How does your lexical analyzer handle comments? Explain the finite state machine approach.

**Answer:**

Our lexer handles **two types of comments** using Flex's state machine feature:

**1. Single-line Comments (`//`):**
```c
"//".*                { /* skip single-line comments */ }
```
- Pattern: `//` followed by any characters until end of line
- **No state needed**: Simple pattern matching
- Automatically ends at newline

**2. Block Comments (`/* ... */`):**
```c
%x COMMENT

"/*"                  {
                        comment_start_line = current_line;
                        comment_start_column = token_start_column;
                        BEGIN(COMMENT);  // Enter COMMENT state
                      }

<COMMENT>"*/"         { BEGIN(INITIAL); }  // Exit to normal state
<COMMENT>\n           { current_line++; current_column = 1; }
<COMMENT>.            { /* consume comment content */ }
<COMMENT><<EOF>>      {
                        // Error: unterminated comment
                        lex_support_record_error(...);
                        BEGIN(INITIAL);
                        return 0;
                      }
```

**Finite State Machine:**

```
[INITIAL State]
    |
    | "/*"
    v
[COMMENT State]
    |
    | "*/" → [INITIAL State] (comment closed)
    | "\n" → [COMMENT State] (update line counter)
    | .    → [COMMENT State] (consume character)
    | EOF  → [ERROR] (unterminated comment)
```

**Why State Machine?**
- Block comments can span multiple lines
- Need to track when we're inside a comment vs. normal code
- Can detect unterminated comments (error recovery)
- Prevents comment content from being tokenized

**Error Handling:**
- If EOF reached while in COMMENT state → unterminated comment error
- Records starting location for error message

---

### Q4: What is the difference between LL and LR parsing? Why did you choose LL(1) for this project?

**Answer:**

**LL (Left-to-right, Leftmost derivation):**
- **Direction**: Reads input left-to-right
- **Derivation**: Builds parse tree top-down (from start symbol to terminals)
- **Lookahead**: Uses 1 token lookahead (LL(1))
- **Grammar Requirements**: No left recursion, FIRST/FOLLOW sets must be disjoint
- **Implementation**: Recursive descent or table-driven
- **Example**: `A → B C` means "if we see A, try to match B then C"

**LR (Left-to-right, Rightmost derivation):**
- **Direction**: Reads input left-to-right
- **Derivation**: Builds parse tree bottom-up (from terminals to start symbol)
- **Lookahead**: Usually 1 token (LALR(1))
- **Grammar Requirements**: More flexible, can handle left recursion
- **Implementation**: Shift-reduce parser (stack-based)
- **Example**: `A → B C` means "if we see B C, reduce to A"

**Key Differences:**

| Aspect | LL(1) | LR(1) |
|--------|-------|-------|
| Parse direction | Top-down | Bottom-up |
| Left recursion | ❌ Not allowed | ✅ Allowed |
| Grammar flexibility | Less flexible | More flexible |
| Error detection | Early (at start of production) | Late (after shift) |
| Implementation | Recursive descent | Stack-based |

**Why LL(1) for This Project?**

1. **TMA02 Requirement**: Assignment explicitly requires LL(1) grammar transformation
2. **Educational Value**: Demonstrates understanding of grammar transformations (left recursion removal, FIRST/FOLLOW sets)
3. **Predictive Parsing**: Easy to understand and implement (recursive descent)
4. **Error Recovery**: Can detect errors early in production

**Our Implementation:**
- **Grammar**: Transformed to LL(1) structure (right-recursive, no left recursion)
- **Parser Generator**: Bison (LR parser generator) - but grammar is LL(1) structured
- **Why Bison?**: Robust error recovery, production-quality tool, can parse LL(1) grammars efficiently

---

### Q5: Explain how you removed left recursion from the expression grammar. Show the transformation.

**Answer:**

**Original Left-Recursive Grammar:**
```
expr → expr AND expr | expr OR expr | relExpr
arithExpr → arithExpr + term | arithExpr - term | term
term → term * factor | term / factor | factor
```

**Problem**: Left recursion causes infinite loop in top-down parsing:
```
parse_expr() {
    parse_expr();  // Calls itself immediately → infinite recursion!
}
```

**Transformation Rule:**
For `A → A α | β`, transform to:
```
A → β A'
A' → α A' | ϵ
```

**Applied Transformations:**

**1. Expression Grammar:**
```
Original:
expr → expr AND expr | expr OR expr | relExpr

Transformed:
expr → relExpr exprPrime
exprPrime → AND relExpr exprPrime | OR relExpr exprPrime | ϵ
```

**2. Arithmetic Expression:**
```
Original:
arithExpr → arithExpr + term | arithExpr - term | term

Transformed:
arithExpr → term arithExprPrime
arithExprPrime → addOp term arithExprPrime | ϵ
addOp → + | - | or
```

**3. Term:**
```
Original:
term → term * factor | term / factor | factor

Transformed:
term → factor termPrime
termPrime → multOp factor termPrime | ϵ
multOp → * | / | and
```

**AST Reconstruction:**

Since right-recursive grammar produces right-associative parse but we need left-associative semantics, we reconstruct the AST:

```c
expr:
      relExpr exprPrime
      {
          if ($2) {
              // Build left-associative tree from right-recursive parse
              AST *op = $2;
              AST *left = $1;
              AST *right = op->child ? op->child->sibling : NULL;
              op->child = left;
              if (right) {
                  op->child->sibling = right;
              }
              $$ = op;
          } else {
              $$ = $1;
          }
      }
```

**Example Parse:**
```
Input: a AND b AND c

Parse (right-recursive):
expr → relExpr(a) exprPrime
exprPrime → AND relExpr(b) exprPrime
exprPrime → AND relExpr(c) exprPrime
exprPrime → ϵ

AST (reconstructed left-associative):
     AND
    /   \
   AND   c
  /   \
 a     b
```

---

### Q6: What are FIRST and FOLLOW sets? Explain their role in LL(1) parsing.

**Answer:**

**FIRST Set:**
- **Definition**: `FIRST(α)` is the set of terminals that can begin strings derived from `α`
- **Purpose**: Helps parser decide which production to choose
- **Calculation**:
  - If `α` starts with terminal `a`, then `a ∈ FIRST(α)`
  - If `α` starts with nonterminal `A`, then `FIRST(A) ⊆ FIRST(α)`
  - If `α` can derive `ϵ`, then `ϵ ∈ FIRST(α)`

**FOLLOW Set:**
- **Definition**: `FOLLOW(A)` is the set of terminals that can appear immediately after `A` in some sentential form
- **Purpose**: Helps parser decide when to use `ϵ` production
- **Calculation**:
  - If `S → ... A β`, then `FIRST(β) - {ϵ} ⊆ FOLLOW(A)`
  - If `S → ... A` or `S → ... A β` where `ϵ ∈ FIRST(β)`, then `FOLLOW(S) ⊆ FOLLOW(A)`
  - `$` (end of input) is in `FOLLOW(S)` where `S` is start symbol

**LL(1) Condition:**

For each production `A → α₁ | α₂ | ... | αₙ`:
1. **FIRST sets must be disjoint**: `FIRST(αᵢ) ∩ FIRST(αⱼ) = ∅` for all i ≠ j
2. **If `ϵ` production exists**: `FIRST(αⱼ) ∩ FOLLOW(A) = ∅` for all j where `ϵ ∉ FIRST(αⱼ)`

**Example from Our Grammar:**

**FIRST(exprPrime):**
```
exprPrime → AND relExpr exprPrime | OR relExpr exprPrime | ϵ

FIRST(AND relExpr exprPrime) = {AND}
FIRST(OR relExpr exprPrime) = {OR}
FIRST(ϵ) = {ϵ}

FIRST(exprPrime) = {AND, OR, ϵ}
```

**FOLLOW(exprPrime):**
```
expr → relExpr exprPrime

Since exprPrime can be empty, FOLLOW(exprPrime) = FOLLOW(expr)
FOLLOW(expr) = {RPAREN, SEMICOLON, COMMA, THEN, ELSE, RBRACE}

FOLLOW(exprPrime) = {RPAREN, SEMICOLON, COMMA, THEN, ELSE, RBRACE}
```

**Verification:**
- `{AND} ∩ {OR} = ∅` ✅
- `{AND, OR} ∩ FOLLOW(exprPrime) = ∅` ✅ (no conflict)
- **Result**: LL(1) condition satisfied ✅

**Role in Parsing:**
```c
parse_exprPrime() {
    if (lookahead == AND) {
        match(AND);
        parse_relExpr();
        parse_exprPrime();
    } else if (lookahead == OR) {
        match(OR);
        parse_relExpr();
        parse_exprPrime();
    } else if (lookahead ∈ FOLLOW(exprPrime)) {
        // Use ϵ production
        return;
    } else {
        error("Unexpected token");
    }
}
```

---

### Q7: How does your parser handle syntax errors? Explain the error recovery strategy.

**Answer:**

Our parser uses **panic-mode recovery** with synchronization points:

**Error Detection:**
```c
void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", current_line, s);
}
```

**Error Recovery Strategy:**

**1. Error Productions:**
```yacc
statement:
      assignStat SEMICOLON
    | IF ( expr ) THEN statBlock ELSE statBlock SEMICOLON
    | ...
    | error SEMICOLON
      {
          log_production("statement -> error ;");
          yyerror("Recovering from statement error");
          yyerrok;  // Reset error state
          $$ = NULL;
      }
```

**2. Synchronization Points:**
- **Semicolons (`;`)**: End of statements
- **Braces (`}`, `)`)**: End of blocks/expressions
- **Keywords**: `IF`, `WHILE`, `RETURN`, etc.

**3. Recovery Mechanism:**
```c
When error detected:
1. Report error with line number
2. Discard tokens until synchronization point found
3. Reset error state (yyerrok)
4. Continue parsing from synchronization point
5. Report multiple errors in one pass
```

**Example:**
```
Input: x := 5 + ; y := 10;

Parse:
1. x := 5 + ... (expecting expression after +)
2. Encounter ; (syntax error)
3. Report: "Syntax error at line 1: unexpected ';'"
4. Use error SEMICOLON production
5. Reset error state
6. Continue with: y := 10;
```

**Why Panic-Mode?**
- **Simple**: Easy to implement
- **Effective**: Recovers from most errors
- **Multiple errors**: Can report many errors in one pass
- **Standard**: Used in many production compilers

**Limitations:**
- May skip valid code after error
- Can produce cascading false errors
- Not as sophisticated as phrase-level recovery

---

### Q8: What is an Abstract Syntax Tree (AST)? How is it different from a parse tree?

**Answer:**

**Parse Tree (Concrete Syntax Tree):**
- **Represents**: Exact structure of grammar productions
- **Contains**: All terminals and nonterminals
- **Structure**: Mirrors grammar rules exactly
- **Size**: Larger (includes punctuation, keywords)
- **Purpose**: Shows how input was parsed

**Example Parse Tree:**
```
        statement
       /    |     \
  assignStat  ;   (empty)
   /   |   \
variable := expr
  |        |
  x      arithExpr
         /   |   \
    arithExpr + term
        |        |
      term    factor
        |        |
      factor   5
        |
       3
```

**Abstract Syntax Tree (AST):**
- **Represents**: Semantic structure of program
- **Contains**: Only essential nodes (no punctuation, keywords)
- **Structure**: Simplified, semantic-focused
- **Size**: Smaller, more compact
- **Purpose**: Used for semantic analysis and code generation

**Example AST:**
```
      ASSIGN
     /      \
   ID(x)   BINARY_OP(+)
          /          \
    INT_LIT(3)   INT_LIT(5)
```

**Key Differences:**

| Aspect | Parse Tree | AST |
|--------|------------|-----|
| Nodes | All grammar symbols | Only semantic nodes |
| Terminals | Included | Omitted (stored in node data) |
| Structure | Grammar structure | Semantic structure |
| Size | Large | Compact |
| Use | Parsing verification | Semantic analysis, codegen |

**In Our Implementation:**

**AST Node Structure:**
```c
typedef struct AST {
    NodeKind kind;        // NODE_ASSIGN, NODE_BINARY_OP, etc.
    char *name;           // Identifier name, operator, etc.
    char *typeName;       // Type information (from semantic analysis)
    int lineno;           // Source location
    struct AST *child;    // First child
    struct AST *sibling;  // Next sibling
    struct AST *extra;    // Additional pointer (e.g., function body)
} AST;
```

**AST Construction:**
- Built during parsing using semantic actions
- Each grammar rule creates appropriate AST nodes
- Children linked via `child` and `sibling` pointers
- Used by semantic analyzer and code generator

---

### Q9: Explain the two-pass architecture of semantic analysis. Why not use a single pass?

**Answer:**

**Two-Pass Architecture:**

**Pass A (Symbol Table Construction):**
- **Purpose**: Build complete symbol tables
- **Traverses**: AST to find all declarations
- **Creates**: Scopes (global, class, function)
- **Inserts**: Symbols (classes, functions, variables, parameters, attributes)
- **Detects**: Duplicate declarations
- **Output**: Complete symbol table hierarchy

**Pass B (Semantic Checking):**
- **Purpose**: Validate usage against declarations
- **Traverses**: AST again to check all usage
- **Uses**: Symbol tables from Pass A
- **Checks**: Type compatibility, scope rules, function calls, return types
- **Reports**: Semantic errors
- **Output**: Error reports, type information

**Why Two Passes?**

**1. Forward References:**
```c
func test() -> int {
    x := y + 5;      // Uses 'y' before declaration
    local y : int;
    return(x);
}
```
- **Single pass**: When checking `x := y + 5`, `y` isn't declared yet → false error
- **Two passes**: Pass A declares `y`, Pass B can safely look it up ✅

**2. Function Call Before Declaration:**
```c
func caller() -> int {
    return(callee());  // Calls 'callee' before it's declared
}

func callee() -> int {
    return(5);
}
```
- **Single pass**: Would report "callee undefined" even though it exists
- **Two passes**: Pass A declares all functions, Pass B can check calls ✅

**3. Separation of Concerns:**
- **Pass A**: "What is declared?" → Declaration checking
- **Pass B**: "How is it used?" → Usage checking
- Clear separation makes code easier to understand and maintain

**4. Efficiency:**
- Build symbol tables once (Pass A)
- Check usage many times (Pass B)
- More efficient than rebuilding tables during checking

**Trade-offs:**
- **Two passes**: More memory (store symbol tables), but accurate
- **Single pass**: Less memory, but misses some errors or requires complex forward declaration handling

**Our Choice:**
- Two passes for accuracy and clarity
- Standard approach in production compilers
- Worth the extra traversal for correctness

---

### Q10: How does your symbol table handle scoping? Explain with an example of nested scopes.

**Answer:**

Our symbol table uses **hierarchical scoping** with parent-child relationships:

**Structure:**
```c
typedef struct SymTable {
    char *scopeName;
    SymTable *parent;      // Pointer to enclosing scope
    Symbol *symbols;       // Linked list of symbols in this scope
    // ... other fields
} SymTable;
```

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
    // Search current scope first
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
    attribute x : int;        // In class scope
    func method(y : int) -> int {  // y in function scope
        local z : float;       // z in function scope
        return(x + y + z);    // x from class, y from function, z from function
    }
}
```

**Lookup Process for `x` in function:**
1. Check function scope → not found
2. Check class scope (parent) → found `x : int` ✅

**Lookup Process for `y` in function:**
1. Check function scope → found `y : int` ✅ (stops here)

**Lookup Process for `z` in function:**
1. Check function scope → found `z : float` ✅ (stops here)

**Variable Shadowing:**
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

- Inner scope can declare same name as outer scope (shadowing allowed)
- Lookup always finds the **nearest** declaration
- Same scope cannot redeclare (duplicate error)

---

### Q11: How do you perform type checking for binary operations? What types are compatible?

**Answer:**

Type checking for binary operations is performed in **Pass B** using `resolve_type_of_expr()`:

**Arithmetic Operations** (`+`, `-`, `*`, `/`):
```c
case NODE_BINARY_OP:
    if (operator is +, -, *, /) {
        const char *lt = resolve_type_of_expr(scope, left);
        const char *rt = resolve_type_of_expr(scope, right);
        
        if (is_numeric_type(lt) && is_numeric_type(rt)) {
            // Result: float if either operand is float, otherwise int
            if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0) {
                return "float";
            }
            return "int";
        } else {
            error("Arithmetic operands must be numeric");
            return "<error>";
        }
    }
```

**Rules:**
- Both operands must be numeric (`int` or `float`)
- Result type: `float` if either operand is `float`, otherwise `int`
- Error if operands are strings or other types

**Relational Operations** (`==`, `<>`, `<`, `>`, `<=`, `>=`):
- Operands must be same type OR both numeric
- Result type: always `int` (boolean result: 0 = false, non-zero = true)
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
5 < 3.14        // ✅ int < float → int (both numeric)
"a" < "b"       // ✅ string < string → int
5 and 3         // ✅ int and int → int
5 and 3.14      // ❌ Error: int and float (logical ops require int)
```

**Type Compatibility Matrix:**

| Operation | Left Type | Right Type | Result | Valid? |
|-----------|-----------|------------|--------|--------|
| `+` | int | int | int | ✅ |
| `+` | int | float | float | ✅ |
| `+` | float | float | float | ✅ |
| `+` | string | int | - | ❌ |
| `==` | int | int | int | ✅ |
| `==` | int | float | int | ✅ |
| `==` | string | string | int | ✅ |
| `and` | int | int | int | ✅ |
| `and` | int | float | - | ❌ |

---

### Q12: How are function calls validated? What aspects are checked?

**Answer:**

Function call validation checks **three aspects**:

**1. Function Existence:**
```c
Symbol *fn = symtable_lookup(curScope, functionName);
if (!fn || fn->kind != SYM_FUNC) {
    sem_error(lineno, "Call to undefined function '%s'", functionName);
    return "<error>";
}
```

**2. Argument Count:**
```c
int argCount = 0, paramCount = 0;
for (AST *a = expr->child; a; a = a->sibling) argCount++;
for (Symbol *p = fn->params; p; p = p->next) paramCount++;

if (argCount != paramCount) {
    sem_error(lineno,
              "Call to '%s' with wrong number of arguments (expected %d, got %d)",
              functionName, paramCount, argCount);
    return "<error>";
}
```

**3. Argument Types:**
```c
AST *arg = expr->child;
Symbol *param = fn->params;
int argIndex = 1;
while (arg && param) {
    const char *argType = resolve_type_of_expr(curScope, arg);
    if (strcmp(argType, param->typeName) != 0) {
        // Allow int → float promotion
        if (!(strcmp(param->typeName, "float") == 0 && strcmp(argType, "int") == 0)) {
            sem_error(arg->lineno,
                      "Argument %d type mismatch: parameter '%s' expects %s, got %s",
                      argIndex, param->name, param->typeName, argType);
        }
    }
    arg = arg->sibling;
    param = param->next;
    argIndex++;
}
```

**Example:**
```c
func add(x : int, y : float) -> float { ... }

add(5, 3.14);        // ✅ Correct: 2 args, types match
add(5);              // ❌ Error: wrong number of arguments (expected 2, got 1)
add(5, 3.14, 10);    // ❌ Error: wrong number of arguments (expected 2, got 3)
add(5, "hello");     // ❌ Error: argument 2 type mismatch (y expects float, got string)
add(5, 10);          // ✅ Correct: int promoted to float for parameter y
```

**Special Cases:**
- **Type Promotion**: `int` arguments can be passed to `float` parameters
- **Nested Calls**: Recursively validates nested function calls
- **Error Propagation**: If function not found, returns error type to prevent cascading errors

---

### Q13: What is the difference between syntax errors and semantic errors? Give examples of each.

**Answer:**

**Syntax Error:**
- **Definition**: Violates grammar rules
- **Detected**: During parsing (syntax analysis phase)
- **Cause**: Invalid token sequence, missing punctuation, unmatched brackets
- **Example**: Missing semicolon, unmatched parentheses, invalid operator sequence

**Examples:**
```c
x := 5        // ❌ Syntax: missing semicolon
if (x > 5 {   // ❌ Syntax: missing closing parenthesis
x := + 5      // ❌ Syntax: invalid operator sequence (+ followed by number without operand)
func add(     // ❌ Syntax: incomplete function declaration
```

**Semantic Error:**
- **Definition**: Grammatically correct but meaningless or invalid
- **Detected**: During semantic analysis phase
- **Cause**: Type mismatch, undeclared variable, wrong function arguments, scope violations
- **Example**: Can't add string to int, variable not declared, function called with wrong arguments

**Examples:**
```c
x := "hello" + 5;        // ❌ Semantic: can't add string to int (syntax is valid)
y := z;                  // ❌ Semantic: z not declared (syntax is valid)
func add(x : int) -> int { return(x); }
add(5, 10);              // ❌ Semantic: wrong number of arguments (syntax is valid)
if ("hello") { ... }      // ❌ Semantic: condition must be numeric (syntax is valid)
```

**Key Differences:**

| Aspect | Syntax Error | Semantic Error |
|--------|--------------|----------------|
| **Question** | "Is this valid according to the grammar?" | "Does this make sense? Are types compatible?" |
| **Detection** | Parser | Semantic Analyzer |
| **Example** | Missing `;` | Type mismatch |
| **Recovery** | Panic-mode (skip to sync point) | Continue analysis (report all errors) |
| **Output** | `yyerror()` | `sem_error()` |

**In Our Compiler:**
- **Syntax errors**: Reported by parser (`yyerror()`) → `stderr`
- **Semantic errors**: Reported by semantic analyzer (`sem_error()`) → `semantic_errors.txt`

**Processing Order:**
1. **Lexical Analysis**: Token errors (malformed numbers, unknown characters)
2. **Syntax Analysis**: Grammar errors (missing semicolons, unmatched brackets)
3. **Semantic Analysis**: Type/scope errors (type mismatches, undeclared variables)

**Note**: Syntax errors prevent semantic analysis (no AST built), but semantic errors don't prevent code generation (though we skip codegen if semantic errors exist).

---

### Q14: How do you handle control flow statements (if, while) in semantic analysis?

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

// Usage in Pass B:
case NODE_IF: {
    AST *cond = p->child;
    AST *thenBlock = cond ? cond->sibling : NULL;
    AST *elseBlock = thenBlock ? thenBlock->sibling : NULL;
    
    check_condition(cond, scope, "IF");
    
    // Recursively check then and else blocks
    if (thenBlock) semantic_passB_visit(thenBlock, scope, currentReturn);
    if (elseBlock) semantic_passB_visit(elseBlock, scope, currentReturn);
    break;
}

case NODE_WHILE: {
    AST *cond = p->child;
    AST *body = cond ? cond->sibling : NULL;
    
    check_condition(cond, scope, "WHILE");
    
    // Recursively check loop body
    if (body) semantic_passB_visit(body, scope, currentReturn);
    break;
}
```

**Why Numeric?**
- Conditions are evaluated as true/false
- Numeric values: `0` = false, non-zero = true
- Strings cannot be meaningfully evaluated as boolean
- Relational expressions return `int` (0 or 1), which is numeric

**Examples:**
```c
if (x > 5) { ... }           // ✅ x > 5 is numeric (int)
if (x + y) { ... }           // ✅ x + y is numeric
if ("hello") { ... }         // ❌ Error: string condition
if (x == "test") { ... }     // ✅ x == "test" is int (relational result)
while (count < 10) { ... }   // ✅ count < 10 is numeric
while ("loop") { ... }       // ❌ Error: string condition
while (1) { ... }            // ✅ 1 is numeric (int literal)
```

**Type Checking Flow:**
1. **Resolve condition type**: Call `resolve_type_of_expr()` on condition
2. **Check numeric**: Verify type is `int` or `float`
3. **Report error**: If not numeric, report semantic error
4. **Check body**: Recursively validate statements in then/else/while body

**Special Cases:**
- **Relational expressions**: `x > 5` returns `int` (numeric) ✅
- **Logical expressions**: `x and y` returns `int` (numeric) ✅
- **Arithmetic expressions**: `x + y` returns numeric ✅
- **String literals**: `"hello"` is string ❌
- **String comparisons**: `"a" < "b"` returns `int` (numeric) ✅

---

### Q15: Explain how your semantic analyzer handles return statements. What validations are performed?

**Answer:**

Return statement validation checks **three aspects**:

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
    AST *expr = p->child;
    const char *exprType = NULL;
    
    if (expr) {
        exprType = resolve_type_of_expr(scope, expr);
    }
    
    const char *expectedReturn = currentReturn;  // from function declaration
    
    if (strcmp(expectedReturn, "void") == 0) {
        // Void function: should not return a value
        if (exprType && strcmp(exprType, "<nil>") != 0 && 
            strcmp(exprType, "<void>") != 0 && strcmp(exprType, "<error>") != 0) {
            sem_error(p->lineno,
                      "Void functions should not return a value");
        }
    } else {
        // Non-void function: must return compatible type
        if (!exprType || strcmp(exprType, "<error>") == 0) {
            // Error already reported in expression resolution
        } else if (strcmp(exprType, expectedReturn) != 0) {
            // Allow int → float promotion
            if (!(strcmp(expectedReturn, "float") == 0 && strcmp(exprType, "int") == 0)) {
                sem_error(p->lineno,
                          "Return type mismatch: expected %s, got %s",
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

func test() -> int {
    if (x > 5) {
        return(10);      // ✅ int returns int
    }
    // ❌ Error: function must return value (no return in else path)
}
```

**Type Promotion:**
- `int` can be returned from `float` function (promoted to float)
- `float` cannot be returned from `int` function (loss of precision)

**Error Cases:**
1. **Void function with value**: `return(5);` in void function
2. **Type mismatch**: `return("hello");` in int function
3. **Missing return**: Function with return type but no return statement (not checked in current implementation, but could be added)

---

## Summary

These 15 questions cover:
- ✅ Lexical analysis (tokens, regular expressions, finite state machines)
- ✅ Syntax analysis (LL(1) parsing, grammar transformations, FIRST/FOLLOW sets)
- ✅ Semantic analysis (two-pass architecture, symbol tables, type checking)
- ✅ Error handling (syntax vs semantic errors, error recovery)
- ✅ Implementation details (AST, scoping, function calls, control flow)

All answers are based on the actual implementation in `scanner.l`, `parser.y`, `semantic.c`, and `symbol_table.c`.


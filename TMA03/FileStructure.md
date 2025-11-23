# TMA03 File Structure Documentation

This document explains every file in the TMA03 compiler project, what it does, and how it fits into the overall compilation pipeline. This is written for someone completely new to compiler implementation.

---

## Overview: How a Compiler Works (TMA03 Focus)

TMA03 focuses on **Semantic Analysis** - the phase that checks if a program makes sense beyond just syntax. A compiler translates source code through several phases:

1. **Lexical Analysis (Scanner)**: Breaks source code into tokens (words, numbers, operators)
2. **Syntax Analysis (Parser)**: Checks if tokens form valid program structure and builds an Abstract Syntax Tree (AST)
3. **Semantic Analysis** ⭐: Checks meaning (types, scopes, declarations, type compatibility) - **This is TMA03's focus**
4. **Code Generation**: Produces assembly/machine code (not in TMA03)

TMA03 implements phases 1-3, with emphasis on semantic analysis.

---

## Source Files (Core Implementation)

### 1. `main.c` - The Compiler Driver

**Purpose**: The main entry point that orchestrates all compiler phases.

**What it does**:
- Takes a source file as command-line argument
- Opens the source file and passes it to the parser
- Calls the parser (`yyparse()`) which internally uses the lexer to build an AST
- Prints the AST structure to console
- Runs semantic analysis in **two separate passes**:
  - **Pass A**: Builds symbol tables (collects all declarations: classes, functions, variables)
  - **Pass B**: Performs type checking and semantic validation (checks if types match, variables are declared, etc.)
- Writes output files:
  - `symbol_table.txt`: All symbol tables (scopes, variables, functions, their types)
  - `semantic_errors.txt`: All semantic errors found (type mismatches, undeclared variables, etc.)

**Key Code Flow**:
```c
1. Open source file → yyin = fopen(argv[1], "r")
2. Parse → yyparse() (builds AST)
3. Print AST → ast_print(astRoot, 0)
4. Pass A → semantic_passA(astRoot) (build symbol tables)
5. Write symbol tables → symtable_print_all(globalTable, symout)
6. Pass B → semantic_passB(astRoot) (check types, validate semantics)
7. Write errors → semantic_errors.txt
```

**For beginners**: Think of this as the "manager" of a quality control process. It doesn't do the checking itself, but coordinates the lexer (reads code), parser (builds structure), and semantic analyzer (checks meaning) to ensure the program is correct.

**Difference from DesignProject**: TMA03 focuses only on semantic analysis, so it doesn't include code generation. The main function is simpler - it just runs lexing, parsing, and semantic analysis.

---

### 2. `scanner.l` - Lexical Analyzer (Lexer)

**Purpose**: Converts raw source code text into a stream of tokens.

**What it does**:
- Reads source code character by character
- Recognizes patterns using regular expressions:
  - **Keywords**: `class`, `func`, `if`, `while`, `return`, `read`, `write`, etc.
  - **Identifiers**: Variable/function names (e.g., `myVariable`, `calculateSum`)
  - **Numbers**: 
    - Integers: `42`, `100`
    - Floats: `3.14`, `2.5e10` (with scientific notation)
  - **Operators**: 
    - Arithmetic: `+`, `-`, `*`, `/`
    - Relational: `==`, `<>`, `<`, `>`, `<=`, `>=`
    - Logical: `and`, `or`, `not` (or `&&`, `||`)
    - Assignment: `:=`
  - **Punctuation**: `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`, `:`, `.`, `=>`
- Skips whitespace (spaces, tabs) and comments:
  - Line comments: `// comment until end of line`
  - Block comments: `/* comment spanning multiple lines */`
- Tracks line numbers (`current_line`) for error reporting
- Returns tokens to the parser one at a time

**Key Features**:
- Uses **Flex** (Fast Lexical Analyzer Generator) to automatically generate a finite-state machine
- `%option noyywrap`: Tells Flex we don't need to handle multiple input files
- `YY_USER_ACTION`: Macro that runs before each token is returned - updates location tracking
- State machine for block comments: Uses `BEGIN(COMMENT)` to enter comment mode, `BEGIN(INITIAL)` to exit

**Example**:
Input: `func add(x : int) -> int { return(x + 1); }`

Tokens produced:
```
FUNC
ID("add")
LPAREN
ID("x")
COLON
ID("int")  // Note: type names are treated as IDs
RPAREN
ARROW
ID("int")
LBRACE
RETURN
LPAREN
ID("x")
PLUS
INT_LIT(1)
RPAREN
SEMICOLON
RBRACE
```

**For beginners**: This is like a word processor that highlights different parts of speech. It reads the raw text and categorizes everything: "This is a keyword, this is a variable name, this is a number, this is an operator." The parser then uses these categories to understand the structure.

---

### 3. `parser.y` - Syntax Analyzer (Parser)

**Purpose**: Verifies that tokens form a syntactically valid program and builds an Abstract Syntax Tree (AST).

**What it does**:
- Receives token stream from lexer
- Uses **grammar rules** (written in BNF-like notation) to recognize program structure
- Each grammar rule has a **semantic action** (code in `{ }`) that constructs AST nodes
- Builds a tree structure (AST) representing the program's syntax
- Handles syntax errors (reports them, but continues parsing if possible)

**Grammar Structure** (LL(1) compatible):
- **Top-level**: `prog` → can contain classes, implementations, or functions (in any order)
- **Classes**: `classDecl` → `class` name, optional inheritance (`isa`), body with attributes
- **Functions**: `funcDef` → function header (name, parameters, return type) + body with statements
- **Statements**: 
  - Assignments: `variable := expression;`
  - Control flow: `if (condition) then ... else ...`, `while (condition) ...`
  - I/O: `read(variable);`, `write(expression);`
  - Return: `return(expression);`
- **Expressions**: 
  - Arithmetic: `a + b`, `x * y`
  - Relational: `a == b`, `x < y`
  - Logical: `a and b`, `x or y`
  - With proper operator precedence

**Key Features**:
- Uses **Bison** (Yacc) to generate a bottom-up parser
- `%locations`: Enables source location tracking (`@1.first_line` gives line number)
- `%union`: Defines what values tokens/nonterminals can carry (integers, floats, strings, AST nodes)
- `%type`: Specifies which nonterminals carry AST nodes
- **LL(1) Structure**: Grammar uses right-recursive forms (no left recursion):
  - `expr → relExpr exprPrime` (not `expr → expr AND expr`)
  - `arithExpr → term arithExprPrime` (not `arithExpr → arithExpr + term`)
  - This makes the grammar LL(1) compatible

**AST Building Example**:
For `x := a + b;`, the parser:
1. Recognizes `assignStat → variable ASSIGN expr`
2. Creates `NODE_ASSIGN` node
3. Creates `NODE_ID` node for `x` (left side)
4. Creates `NODE_BINARY_OP` node for `+` with children `a` and `b` (right side)
5. Attaches both to the assignment node

**For beginners**: This is like a grammar checker that also builds a sentence diagram. It ensures "I go store" becomes "I go to the store" - checking that tokens follow the language's rules. The AST is like a sentence diagram showing the structure: subject, verb, object, and how they relate.

**Output**: AST (stored in `astRoot` global variable), which is then used by semantic analyzer

---

### 4. `ast.h` and `ast.c` - Abstract Syntax Tree

**Purpose**: Defines and implements the data structure for representing program structure.

**What it does**:

**`ast.h`** (Header file - declarations):
- Defines `NodeKind` enum: All possible types of AST nodes
  - Program structure: `NODE_PROGRAM`, `NODE_CLASS_DECL`, `NODE_FUNC_DECL`
  - Statements: `NODE_ASSIGN`, `NODE_IF`, `NODE_WHILE`, `NODE_RETURN`, `NODE_READ`, `NODE_WRITE`
  - Expressions: `NODE_BINARY_OP`, `NODE_UNARY_OP`, `NODE_FUNCTION_CALL`
  - Literals: `NODE_INT_LITERAL`, `NODE_FLOAT_LITERAL`, `NODE_STRING_LITERAL`
  - Identifiers: `NODE_ID`
  - Types: `NODE_TYPE`, `NODE_VAR_DECL`, `NODE_PARAM`
- Defines `AST` struct: The tree node structure
  - `kind`: What type of node (from `NodeKind` enum)
  - `name`: Identifier name or operator symbol (e.g., `"add"`, `"+"`, `"and"`)
  - `typeName`: Type information (e.g., `"int"`, `"float"`, `"void"`)
  - `intValue`, `floatValue`: For literal nodes
  - `lineno`: Source line number (for error reporting)
  - `child`: Pointer to first child node
  - `sibling`: Pointer to next sibling (for lists)
  - `extra`: Auxiliary pointer (used for function bodies, etc.)

**`ast.c`** (Implementation):
- `ast_new()`: Creates a new AST node with given kind, name, and line number
- `ast_new_int()`, `ast_new_float()`, `ast_new_string()`: Convenience functions for literal nodes
- `ast_append_child()`: Adds a child node to a parent
- `ast_append_sibling()`: Adds a sibling node (for building lists)
- `ast_print()`: Recursively prints the AST tree (for debugging)
- `ast_free()`: Recursively frees all memory used by the AST

**Tree Structure Example**:
For `func add(x : int) -> int { return(x + 1); }`:

```
NODE_PROGRAM
└── NODE_FUNC_DECL ("add", returnType: "int")
    ├── NODE_PARAM ("x", type: "int")  [child - parameter list]
    └── NODE_FUNC_BODY  [extra - function body]
        └── NODE_RETURN
            └── NODE_BINARY_OP ("+")
                ├── NODE_ID ("x")
                └── NODE_INT_LITERAL (1)
```

**For beginners**: The AST is like a family tree for your program. Each node represents a part of the code (a function, a statement, an expression), and the tree shows how they're related. The parser builds this tree, and the semantic analyzer walks through it to check if everything makes sense.

---

### 5. `symbol_table.h` and `symbol_table.c` - Symbol Table Management

**Purpose**: Manages symbol tables that track all declared names (variables, functions, classes) and their properties (types, scopes, locations).

**What it does**:

**`symbol_table.h`** (Header file):
- Defines `SymKind` enum: Types of symbols
  - `SYM_VAR`: Local variable
  - `SYM_FUNC`: Function
  - `SYM_CLASS`: Class
  - `SYM_PARAM`: Function parameter
  - `SYM_ATTR`: Class attribute
- Defines `Symbol` struct: Represents one symbol entry
  - `name`: Symbol name (e.g., `"x"`, `"add"`, `"MyClass"`)
  - `typeName`: Type (e.g., `"int"`, `"float"`, `"void"`, or class name)
  - `kind`: What kind of symbol (from `SymKind`)
  - `lineno`: Line where declared
  - `size`, `offset`: Memory layout info (for code generation, not used in TMA03)
  - `next`: Pointer to next symbol in same scope (linked list)
  - `params`: For functions, pointer to parameter list
- Defines `SymTable` struct: Represents a scope (a symbol table)
  - `scopeName`: Name of scope (e.g., `"global"`, `"add"`, `"MyClass"`)
  - `parent`: Pointer to parent scope (for nested scopes)
  - `symbols`: Linked list of symbols in this scope
  - `next`: For global registry of all scopes
  - `next_offset`, `frame_size`: Memory layout (for code generation)

**`symbol_table.c`** (Implementation):
- `symtable_create()`: Creates a new symbol table (scope)
- `symtable_insert()`: Inserts a symbol into a table (returns 1 if duplicate, 0 on success)
- `symtable_lookup()`: Searches for a symbol, climbing up parent scopes if not found
- `symtable_add_param()`: Adds a parameter to a function symbol
- `symtable_find_scope()`: Finds a scope by name in the global registry
- `symtable_print_all()`: Recursively prints all symbol tables (for debugging)
- `symtable_type_size()`: Calculates memory size for types (for code generation)

**Scope Hierarchy Example**:
```
global scope
├── class MyClass
│   └── class scope
│       ├── attribute x : int
│       └── func method()
│           └── function scope
│               ├── parameter y : int
│               └── local variable z : float
└── func standalone()
    └── function scope
        └── local variable a : int
```

**For beginners**: Symbol tables are like a phone book for your program. When you declare `int x;`, the compiler writes "x is an integer" in the symbol table. Later, when you use `x`, the compiler looks it up to see what type it is. Scopes are like different phone books: one for the whole program (global), one for each function (local), etc. If you can't find a name in the local phone book, you check the global one.

---

### 6. `semantic.c` - Semantic Analyzer

**Purpose**: Performs semantic analysis - checks if the program makes sense beyond syntax (types match, variables are declared, function calls are correct, etc.).

**What it does**:

**Two-Pass Architecture**:

**Pass A - Symbol Table Construction** (`semantic_passA()`):
- Traverses the AST
- For each declaration node:
  - Creates symbol table entries
  - Creates new scopes for classes and functions
  - Inserts symbols (variables, parameters, attributes) into appropriate scopes
  - Links function parameters to function symbols
- Detects duplicate declarations (same name in same scope)
- Builds the complete symbol table hierarchy

**Pass B - Semantic Checking** (`semantic_passB()`):
- Traverses the AST again
- For each use of an identifier:
  - Looks it up in symbol tables (climbing scope hierarchy)
  - Reports error if not found (undeclared variable)
- For each expression:
  - Determines the type of the expression
  - Checks type compatibility (e.g., can't add string to int)
- For assignments:
  - Checks that left and right sides have compatible types
  - Allows `int` to `float` promotion
- For function calls:
  - Checks that function exists
  - Checks that argument count matches parameter count
  - Checks that argument types match parameter types
- For return statements:
  - Checks that return type matches function's declared return type
  - Checks that void functions don't return values
- For control flow:
  - Checks that `if` and `while` conditions are numeric (not strings)

**Key Functions**:
- `semantic_passA()`: Entry point for Pass A
- `semantic_passA_build()`: Recursively builds symbol tables
- `semantic_passB()`: Entry point for Pass B
- `semantic_passB_visit()`: Recursively checks semantics
- `resolve_type_of_expr()`: Determines the type of an expression
- `check_assignment()`: Validates assignment type compatibility
- `check_condition()`: Validates control flow conditions
- `sem_error()`: Reports semantic errors with line numbers

**Error Reporting**:
- All errors include source line numbers
- Errors are deduplicated (same error reported only once)
- Written to `semantic_errors.txt`

**Example Semantic Checks**:
```c
int x;
x := 5;        // ✅ OK: int assigned to int
x := 3.14;     // ✅ OK: float promoted to int (allowed)
x := "hello";  // ❌ Error: type mismatch (string to int)

func add(a : int, b : int) -> int { ... }
add(5, 10);        // ✅ OK: correct arguments
add(5);            // ❌ Error: wrong number of arguments
add(5, "hi");      // ❌ Error: argument type mismatch

y := x + z;    // ❌ Error: 'z' undeclared
```

**For beginners**: Semantic analysis is like a spell-checker that also checks grammar and meaning. Syntax checking says "This sentence has correct grammar" (parser), but semantic checking says "But does it make sense? Is 'x' declared? Can you add a number to a string? Does this function exist?" It's the difference between "Colorless green ideas sleep furiously" (syntactically correct but meaningless) and "The cat sat on the mat" (both syntactically and semantically correct).

---

## Documentation Files

### 7. `Report.md` - TMA02 Syntax Analysis Report

**Purpose**: Comprehensive documentation answering all TMA02 questions about syntax analysis.

**Contents**:
- **(a) Grammar Transformation to LL(1)**:
  - Removal of EBNF notations
  - Analysis of ambiguities and left recursions
  - Modifications to remove left recursion
  - Complete LL(1) grammar productions
- **(b) FIRST and FOLLOW Sets**:
  - Complete FIRST sets for all nonterminals
  - Complete FOLLOW sets for all nonterminals
  - LL(1) condition verification
- **(c) Predictive Parser Implementation**:
  - Implementation method (recursive-descent structure)
  - Grammar structure for predictive parsing
  - Derivation logging mechanism
  - Example derivation output
- **(d) Syntax Error Detection and Recovery**:
  - Error detection mechanisms
  - Panic-mode recovery strategy
  - Error reporting
  - Example error recovery
- **(e) Test Files for All Syntax Structures**:
  - Test coverage documentation
  - Syntax structures tested

**For beginners**: This is the "textbook" that explains how the parser works, why certain design decisions were made, and how the grammar was transformed to be LL(1) compatible.

---

### 8. `BUILD_AND_RUN.md` - Build and Run Instructions

**Purpose**: Step-by-step instructions for building and running the TMA03 compiler.

**Contents**:
- Prerequisites (installing bison, flex, gcc)
- Step-by-step build process
- How to run the compiler
- How to view output files
- Quick rebuild commands
- Cleanup commands

**For beginners**: This is the "user manual" - follow these steps to compile and run the compiler.

---

## Test Files (`tests/` directory)

### 9. `test_pass.src`, `test01.src`, `test02.src`, `test03.src`, `test04.src` - Valid Test Programs

**Purpose**: Test programs that should compile without semantic errors (or with expected errors).

**What they test**:
- `test_pass.src`: Minimal valid program (class with attribute)
- `test01.src`: Function with local variables and return
- `test02.src`: Mixed arithmetic expressions
- `test03.src`: Control flow statements (if, while)
- `test04.src`: Function calls and parameters

**For beginners**: These are like "practice exercises" - programs that demonstrate different language features and should work correctly.

---

### 10. `error01.src`, `error02.src` - Error Test Programs

**Purpose**: Test programs with intentional semantic errors to verify error detection.

**What they test**:
- Undeclared variables
- Type mismatches
- Wrong function argument counts
- Wrong function argument types
- Invalid operations (e.g., adding string to int)

**For beginners**: These are like "trick questions" - programs that look okay but have errors. The semantic analyzer should catch all these errors and report them.

---

## Generated Files (Created During Build/Run)

These files are automatically generated and should not be edited manually:

- `parser.tab.c`, `parser.tab.h`: Generated by Bison from `parser.y`
- `lex.yy.c`: Generated by Flex from `scanner.l`
- `compiler`: The executable compiler (generated by gcc)
- `symbol_table.txt`: Output file with all symbol tables
- `semantic_errors.txt`: Output file with all semantic errors

---

## How Files Work Together

**Compilation Flow**:

```
1. main.c
   ├── Opens source file
   ├── Calls parser (parser.y)
   │   ├── Parser calls lexer (scanner.l)
   │   │   └── scanner.l reads source, returns tokens
   │   └── parser.y builds AST using grammar rules
   │       └── Uses ast.c/ast.h to create AST nodes
   ├── Calls semantic_passA (semantic.c)
   │   └── Uses symbol_table.c/symbol_table.h to build symbol tables
   ├── Calls semantic_passB (semantic.c)
   │   └── Uses symbol_table.c to lookup symbols and check types
   └── Writes output files (symbol_table.txt, semantic_errors.txt)
```

**Data Flow**:
```
Source Code (text)
    ↓
scanner.l (lexical analysis)
    ↓
Token Stream
    ↓
parser.y (syntax analysis)
    ↓
AST (Abstract Syntax Tree)
    ↓
semantic.c Pass A (build symbol tables)
    ↓
Symbol Tables
    ↓
semantic.c Pass B (type checking)
    ↓
Semantic Errors (if any)
```

**For beginners**: Think of it like a factory assembly line:
1. **scanner.l** = Raw material processor (breaks code into tokens)
2. **parser.y** = Structure builder (builds the AST tree)
3. **semantic.c Pass A** = Inventory manager (records what's declared)
4. **semantic.c Pass B** = Quality control (checks if everything is correct)
5. **main.c** = Factory manager (coordinates everything)

---

## Summary

TMA03 implements a **semantic analyzer** for a programming language. The key components are:

- **Lexical Analysis** (`scanner.l`): Reads source code, produces tokens
- **Syntax Analysis** (`parser.y`): Builds AST from tokens
- **Semantic Analysis** (`semantic.c`): Two passes - builds symbol tables, then checks types and semantics
- **Supporting Code** (`ast.c`, `symbol_table.c`): Data structures for AST and symbol tables
- **Driver** (`main.c`): Coordinates all phases

The compiler reads a source file, parses it, builds symbol tables, performs semantic checks, and reports any errors found. This is the foundation for a complete compiler - the next step (Design Project) adds code generation to produce executable assembly code.


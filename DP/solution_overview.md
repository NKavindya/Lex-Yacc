# Solution Overview

## (a) Grammar Transformation to LL(1)

### Issues Identified and Resolved

1. **EBNF Notations Removed**:
   - `{{ ... }}` (zero or more) → Right-recursive list productions
   - `[[ ... ]]` (optional) → Optional productions with epsilon

2. **Left Recursion Eliminated**:
   - `arithExpr -> arithExpr addOp term | term` 
     → `arithExpr -> term arithExprPrime` where `arithExprPrime -> addOp term arithExprPrime | epsilon`
   - `term -> term multOp factor | factor`
     → `term -> factor termPrime` where `termPrime -> multOp factor termPrime | epsilon`

3. **Ambiguities Resolved**:
   - `expr -> arithExpr | relExpr`: Handled by parsing `arithExpr` first, then checking if `relOp` follows
   - `variable` vs `functionCall`: Both start with `idnestList id`. After parsing these, check if next token is `(` (functionCall) or other (variable)

### Complete Transformed Grammar

See `grammar_transformation.md` for the complete transformed grammar with all productions.

## (b) FIRST and FOLLOW Sets

All FIRST and FOLLOW sets have been calculated and documented in `first_follow_sets.md`.

Key observations:
- Most productions are LL(1) compatible
- Two cases require lookahead (handled in implementation):
  - `expr -> arithExpr | relExpr`
  - `variable` vs `functionCall`

## (c) Predictive Parser Implementation

### Type: Recursive Descent (Top-Down Predictive)

**Implementation Files**:
- `parser_rd.h` - Token definitions and function declarations
- `parser_rd.c` - Complete recursive descent parser implementation
- `main_rd.c` - Main program that drives the parser

**Features**:
1. **Grammar Implementation**: Each non-terminal has a corresponding parsing function
2. **Derivation Tracking**: Each production writes its derivation step to a file
3. **Lexical Integration**: Uses the lexical analyzer from TMA #1
4. **Error Reporting**: Reports syntax errors with line numbers
5. **Error Recovery**: Implements panic mode recovery

**Derivation Output Format**:
```
non_terminal -> production
```

Example:
```
prog -> classOrImplOrFuncList
classOrImplOrFuncList -> classOrImplOrFunc classOrImplOrFuncList
classOrImplOrFunc -> classDecl
classDecl -> class id classInheritOpt { memberDeclList } ;
...
```

## (d) Syntax Checking and Solution Structure

### Overall Architecture

```
┌─────────────┐
│ Source File │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Lexer      │  (lexer.l)
│  - Tokenize │
│  - Symbol   │
│    Table    │
└──────┬──────┘
       │ Tokens
       ▼
┌─────────────┐
│   Parser    │  (parser_rd.c)
│  - Recursive│
│    Descent  │
│  - Derivation│
│  - Errors   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Output    │
│  - Derivation│
│  - Errors   │
│  - Symbol   │
│    Table    │
└─────────────┘
```

### Component Roles

1. **Lexical Analyzer** (`lexer.l`):
   - **Role**: Tokenizes source code
   - **Output**: Token stream with type, lexeme, and location
   - **Integration**: Called by parser via `yylex()`
   - **Symbol Table**: Installs identifiers during tokenization

2. **Recursive Descent Parser** (`parser_rd.c`):
   - **Role**: Validates syntax according to grammar
   - **Method**: Top-down predictive parsing
   - **Functions**: One function per non-terminal
   - **Derivation**: Writes each production application to file
   - **Error Handling**: Reports errors and recovers

3. **Symbol Table** (`symtab.c`):
   - **Role**: Stores all identifiers
   - **Operations**: Install, lookup, print
   - **Output**: Printed at end of parsing

4. **Main Program** (`main_rd.c`):
   - **Role**: Orchestrates parsing process
   - **Tasks**: 
     - Opens input and derivation files
     - Initializes parser
     - Calls parse function
     - Prints results

5. **Error Recovery**:
   - **Strategy**: Panic mode
   - **Method**: Skip tokens until synchronization point (semicolon, closing brace)
   - **Reporting**: Accumulates errors, reports at end

### Syntax Checking Process

1. **Initialization**: 
   - Open source file
   - Open derivation file
   - Initialize parser state
   - Get first token

2. **Parsing**:
   - Call `parse_prog()` (start symbol)
   - Each function:
     - Checks current token
     - Applies appropriate production
     - Writes derivation step
     - Recursively calls sub-functions
     - Matches terminals

3. **Error Handling**:
   - On mismatch: report error
   - Skip to sync point
   - Continue parsing

4. **Completion**:
   - Write derivation file
   - Print symbol table
   - Report error count
   - Exit with status

## (e) Test Files

Test suite covers all syntax structures:

1. **test01_class.src**: Basic class declaration with attributes
2. **test02_inheritance.src**: Class inheritance (isa)
3. **test03_function.src**: Function definition
4. **test04_constructor.src**: Constructor definition
5. **test05_if_while.src**: Control structures (if-else, while)
6. **test06_arrays.src**: Array declarations and indexing
7. **test07_expressions.src**: Complex expressions with operators
8. **test08_implement.src**: Implementation blocks
9. **test09_nested_calls.src**: Function calls
10. **test10_complex.src**: Complex class with methods and nested structures

Each test file exercises different aspects of the grammar to ensure comprehensive coverage.

## Error Recovery Strategy

**Chosen Strategy**: Panic Mode Recovery

**Why**:
- Simple to implement
- Effective for most syntax errors
- Allows parser to continue and find multiple errors
- Synchronization points (semicolons, braces) are natural recovery points

**Implementation**:
- When error detected: report with location
- Skip tokens until synchronization point found
- Continue parsing from sync point
- Accumulate error count
- Report summary at end

**Synchronization Points**:
- Semicolon (`;`)
- Closing brace (`}`)
- Closing parenthesis (`)`)
- End of file

## Build and Usage

See `README.md` for build instructions and usage examples.


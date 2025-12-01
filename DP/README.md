# Syntax Analyzer Implementation

This directory contains the implementation of a syntax analyzer (parser) for the programming language specified in TMA #2.

## Files

### Core Implementation
- **parser_rd.h** - Header file for recursive descent parser with token definitions
- **parser_rd.c** - Recursive descent parser implementation
- **main_rd.c** - Main program that drives the parser
- **lexer.l** - Lexical analyzer (from TMA #1)
- **common.h** - Common data structures (Token, SymbolEntry)
- **symtab.h**, **symtab.c** - Symbol table implementation

### Documentation
- **grammar_transformation.md** - Documentation of grammar transformation to LL(1)
- **first_follow_sets.md** - FIRST and FOLLOW sets for the transformed grammar
- **README.md** - This file

### Build System
- **Makefile** - Build configuration

### Test Files
- **tests/test01_class.src** - Class declaration
- **tests/test02_inheritance.src** - Class inheritance
- **tests/test03_function.src** - Function definition
- **tests/test04_constructor.src** - Constructor
- **tests/test05_if_while.src** - Control structures
- **tests/test06_arrays.src** - Array handling
- **tests/test07_expressions.src** - Complex expressions
- **tests/test08_implement.src** - Implementation blocks
- **tests/test09_nested_calls.src** - Function calls
- **tests/test10_complex.src** - Complex class with methods

## Building

```bash
make
```

This will build:
- `parser_rd` - Recursive descent parser (main implementation)
- `parser_yacc` - Yacc-based parser (alternative implementation)

## Usage

```bash
./parser_rd <source_file> [derivation_file]
```

Example:
```bash
./parser_rd tests/test01_class.src derivation_steps.txt
```

The parser will:
1. Parse the source file
2. Write derivation steps to `derivation_steps.txt` (or specified file)
3. Print syntax errors if any
4. Display symbol table at the end

## Solution Overview

### (a) Grammar Transformation

The original grammar was transformed to LL(1) by:
1. **Removing EBNF notations**: Replaced `{{...}}` with right-recursive list productions, `[[...]]` with optional productions
2. **Eliminating left recursion**: Converted left-recursive `arithExpr` and `term` to right-recursive forms using helper non-terminals
3. **Resolving ambiguities**: Handled `expr -> arithExpr | relExpr` and `variable` vs `functionCall` using lookahead

See `grammar_transformation.md` for details.

### (b) FIRST and FOLLOW Sets

All FIRST and FOLLOW sets have been calculated and documented in `first_follow_sets.md`.

### (c) Predictive Parser Implementation

**Type**: Recursive descent parser (top-down predictive)

**Features**:
- Implements all grammar productions as recursive functions
- Tracks derivation steps and writes them to a file
- Integrates with lexical analyzer from TMA #1
- Handles error recovery by skipping to synchronization points

**Derivation Tracking**: Each production writes its derivation step to the output file in the format:
```
non_terminal -> production
```

### (d) Syntax Checking and Solution Structure

**Components**:

1. **Lexical Analyzer** (`lexer.l`): 
   - Tokenizes input source code
   - Returns tokens with type, lexeme, and location
   - Installs identifiers in symbol table

2. **Recursive Descent Parser** (`parser_rd.c`):
   - Implements grammar as recursive functions
   - Each non-terminal has a corresponding parsing function
   - Uses `match()` to consume expected tokens
   - Reports syntax errors with line numbers

3. **Symbol Table** (`symtab.c`):
   - Stores all identifiers encountered
   - Provides lookup and installation functions
   - Printed at end of parsing

4. **Derivation Tracker**:
   - Writes each production application to file
   - Shows the derivation tree in leftmost derivation order

5. **Error Recovery**:
   - Reports syntax errors with location
   - Skips to synchronization points (semicolon, closing braces)
   - Continues parsing after errors

**Error Recovery Strategy**: Panic mode recovery - when an error is detected, skip tokens until a synchronization point (semicolon, closing brace, etc.) is found, then continue parsing.

### (e) Test Files

Test files cover:
- Class declarations with attributes
- Inheritance (isa)
- Function definitions
- Constructors
- Control structures (if-else, while)
- Arrays
- Complex expressions
- Implementation blocks
- Function calls
- Nested structures

## Error Handling

The parser implements panic mode error recovery:
- When a syntax error is detected, it reports the error with line number
- Skips tokens until a synchronization point is found
- Continues parsing from the synchronization point
- Accumulates error count and reports at the end

## Output

The parser produces:
1. **Derivation file**: Step-by-step derivation showing how the source program is derived from the start symbol
2. **Error messages**: Syntax errors with line numbers
3. **Symbol table**: All identifiers found during parsing
4. **Exit status**: 0 on success, non-zero on syntax errors


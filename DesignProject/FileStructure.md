# Design Project File Structure Documentation

This document explains every file in the compiler project, what it does, and how it fits into the overall compilation pipeline. This is written for someone completely new to compiler implementation.

---

## Overview: How a Compiler Works

A compiler translates source code into machine code through several phases:
1. **Lexical Analysis (Scanner)**: Breaks source code into tokens (words, numbers, operators)
2. **Syntax Analysis (Parser)**: Checks if tokens form valid program structure
3. **Semantic Analysis**: Checks meaning (types, scopes, declarations)
4. **Code Generation**: Produces assembly/machine code

Our compiler implements all four phases.

---

## Chosen Machine Architecture

### Simple Register Machine (Hypothetical Architecture)

Our compiler targets a **hypothetical simple register machine** designed specifically for educational purposes. This is **not** a real-world architecture like x86, ARM, MIPS, or RISC-V, but rather a simplified model that demonstrates all key compiler concepts without the complexity of real processors.

**Architecture Type**: RISC-like (Reduced Instruction Set Computer)

**Key Characteristics:**
- **Registers**: 7 general-purpose registers (R1-R7) + 1 dedicated return register (R0)
- **Word Size**: 8 bytes (64 bits)
- **Memory Model**: Stack-based with frame pointer (FP) and stack pointer (SP)
- **Addressing Modes**: 
  - Direct register addressing: `R1`, `R2`, etc.
  - Stack-relative addressing: `[FP-offset]` for local variables and parameters
  - Immediate values: `#42` for constants
- **Instruction Format**: Fixed-length 4-byte instructions
- **Instruction Set**: Simple, high-level instructions (LOAD, STORE, ADD, SUB, JMP, etc.)

**Why This Architecture?**
1. **Educational Focus**: Simplifies learning without real architecture complexity
2. **Portability**: Not tied to any specific processor (works on any platform)
3. **Completeness**: Still demonstrates register allocation, stack frames, control flow, function calls
4. **Clarity**: Easier to understand than real assembly languages (x86, ARM, etc.)

**Comparison with Real Architectures:**
- **x86/x86-64**: CISC, complex instructions, variable-length, used in PCs
- **ARM**: RISC, power-efficient, used in mobile devices and Apple Silicon
- **MIPS**: RISC, educational, simple but real architecture
- **RISC-V**: RISC, open-source, growing adoption
- **Our Machine**: RISC-like, hypothetical, educational, demonstrates compiler concepts

**Note**: While real compilers target architectures like x86, ARM, or RISC-V, our simplified architecture allows students to focus on compiler design principles rather than architecture-specific details.

---

## Source Files (Core Implementation)

### 1. `main.c` - The Compiler Driver
**Purpose**: The main entry point that orchestrates all compiler phases.

**What it does**:
- Opens the source file provided as command-line argument
- Initializes the lexical analyzer support system
- Calls the parser (`yyparse()`) which internally uses the lexer
- After parsing, builds an Abstract Syntax Tree (AST)
- Runs semantic analysis in two passes:
  - **Pass A**: Builds symbol tables (declarations)
  - **Pass B**: Performs type checking and semantic validation
- If no semantic errors, generates assembly code
- Writes all output files (tokens, symbol tables, errors, assembly)

**Key Functions**:
- `main()`: Entry point, coordinates all phases
- Opens output files: `lexer_tokens.txt`, `symbol_table.txt`, `semantic_errors.txt`, `codegen.asm`

**For beginners**: Think of this as the "conductor" of an orchestra - it doesn't play music itself, but tells each section (lexer, parser, semantic analyzer, code generator) when to start and what to do.

---

### 2. `scanner.l` - Lexical Analyzer (Lexer)
**Purpose**: Converts raw source code text into a stream of tokens.

**What it does**:
- Reads source code character by character
- Recognizes patterns using regular expressions:
  - Keywords: `class`, `func`, `if`, `while`, etc.
  - Identifiers: variable/function names (letters, digits, underscores)
  - Numbers: integers (`42`) and floats (`3.14`, `2.5e10`)
  - Operators: `+`, `-`, `:=`, `==`, etc.
  - Punctuation: `(`, `)`, `{`, `}`, `;`, etc.
- Skips whitespace and comments (`//` and `/* */`)
- Tracks line and column numbers for error reporting
- Records every token and symbol in the lexical symbol table
- Reports lexical errors (unknown characters, malformed numbers, unterminated comments)

**Key Features**:
- Uses Flex (Lex) to generate a finite-state automaton
- `YY_USER_ACTION`: Automatically updates location tracking
- `RETURN_KEYWORD()`: Macro to return reserved word tokens
- `RETURN_TOKEN()`: Macro to return operator/punctuation tokens
- State machine for block comments (handles multi-line comments)

**For beginners**: This is like a spell-checker that also categorizes words. It reads "func add(x : int)" and produces: [FUNC] [ID:add] [(] [ID:x] [:] [INTEGER] [)] - a list of categorized tokens.

**Output**: Token stream passed to parser, plus `lexer_tokens.txt` and `lexer_symbols.txt`

---

### 3. `parser.y` - Syntax Analyzer (Parser)
**Purpose**: Verifies that tokens form a syntactically valid program according to the grammar rules.

**What it does**:
- Receives token stream from lexer
- Uses grammar rules (written in BNF-like notation) to build an Abstract Syntax Tree (AST)
- Each grammar rule has an action that constructs AST nodes
- Logs every production rule used to `derivation_steps.txt` (shows how the program was parsed)
- Handles syntax errors with recovery (skips to next statement on error)
- Builds the complete AST representing the program structure

**Grammar Structure**:
- Top-level: `prog` → can contain classes, implementations, or functions
- Classes: `classDecl` → class name, inheritance list, body with attributes
- Functions: `funcDef` → function header (name, parameters, return type) + body
- Statements: assignments, if-then-else, while loops, read/write, return
- Expressions: arithmetic, relational, logical operations

**Key Features**:
- Uses Bison (Yacc) to generate a bottom-up parser
- `%locations`: Tracks source locations for error reporting
- `log_production()`: Records each grammar rule application
- Error recovery: `error SEMICOLON` rule allows parser to skip bad statements

**For beginners**: This is like a grammar checker. It ensures "I go store" becomes "I go to the store" - checking that tokens follow the language's rules. The AST is like a sentence diagram showing the structure.

**Output**: AST (in memory), `derivation_steps.txt`

---

### 4. `ast.h` and `ast.c` - Abstract Syntax Tree
**Purpose**: Data structure representing the program's structure in memory.

**What it does**:
- Defines `AST` structure: a tree where each node represents a program construct
- Node types: `NODE_PROGRAM`, `NODE_FUNC_DECL`, `NODE_ASSIGN`, `NODE_IF`, etc.
- Each node stores:
  - `kind`: What type of construct (function, statement, expression)
  - `name`: Identifier name (for variables, functions)
  - `typeName`: Type information (for variables, return types)
  - `intValue`/`floatValue`: Literal values
  - `lineno`: Source line number
  - `child`: First child node (for hierarchical structures)
  - `sibling`: Next sibling (for lists: parameters, statements)
  - `extra`: Auxiliary pointer (e.g., function body stored in `extra`)

**Key Functions**:
- `ast_new()`: Creates a new AST node
- `ast_append_child()`: Adds a child node
- `ast_append_sibling()`: Adds to a sibling list
- `ast_print()`: Pretty-prints the AST (for debugging)
- `ast_free()`: Frees all memory

**For beginners**: The AST is like a family tree for your program. The root is the program, children are classes/functions, grandchildren are statements, and so on. It's easier to analyze and transform than raw text.

**Example AST**:
```
PROGRAM
  FUNC_DECL name=add
    PARAM name=a type=int
    PARAM name=b type=int
    EXTRA: (function body)
      ASSIGN
        ID name=result
        BINOP +
          ID name=a
          ID name=b
```

---

### 5. `symbol_table.h` and `symbol_table.c` - Symbol Table Management
**Purpose**: Maintains information about all declared identifiers (variables, functions, classes) and their attributes.

**What it does**:
- Creates hierarchical symbol tables (one per scope: global, class, function)
- Each symbol stores:
  - `name`: Identifier name
  - `typeName`: Type (int, float, class name)
  - `kind`: What it is (variable, function, parameter, attribute, class)
  - `lineno`: Declaration line
  - `size`: Memory size in bytes
  - `offset`: Stack frame offset (for code generation)
  - `params`: For functions, list of parameters

**Key Functions**:
- `symtable_create()`: Creates a new scope
- `symtable_insert()`: Adds a symbol to current scope (checks for duplicates)
- `symtable_lookup()`: Searches current scope and parent scopes (for name resolution)
- `symtable_type_size()`: Calculates memory size for types
- `symtable_print_all()`: Dumps all symbol tables to file

**Memory Layout**:
- Calculates stack frame offsets for local variables and parameters
- Aligns all data to 8-byte boundaries
- Tracks total frame size for each function

**For beginners**: This is like a phone book. When you see "x" in code, you look it up to find: "x is an integer variable declared at line 5, stored at offset 16 in the stack frame."

**Output**: `symbol_table.txt` (shows all scopes and symbols)

---

### 6. `semantic.c` - Semantic Analyzer
**Purpose**: Performs semantic analysis: type checking, scope resolution, and validation of program meaning.

**What it does**:
- **Pass A (Build Symbol Tables)**:
  - Traverses AST to find all declarations
  - Creates symbol table entries for classes, functions, variables, parameters
  - Builds nested scopes (global → class → function)
  - Checks for duplicate declarations in same scope

- **Pass B (Semantic Checking)**:
  - Type checking: Ensures operations are valid (e.g., can't add string to int)
  - Scope resolution: Verifies identifiers are declared before use
  - Function call validation: Checks argument count and types match parameters
  - Return type checking: Ensures return statements match function return type
  - Assignment checking: Verifies left and right sides are compatible types

**Key Functions**:
- `semantic_passA()`: First traversal - builds symbol tables
- `semantic_passB()`: Second traversal - performs checks
- `resolve_type_of_expr()`: Determines the type of an expression
- `check_assignment()`: Validates assignment operations
- `sem_error()`: Reports semantic errors with line numbers

**Error Types Detected**:
- Undeclared identifier
- Type mismatch (int + string, etc.)
- Wrong number of function arguments
- Return type mismatch
- Duplicate declarations

**For beginners**: This is like a proofreader that checks meaning, not just grammar. "The cat barks" is grammatically correct but semantically wrong (cats don't bark). Similarly, "x + 'hello'" is syntactically valid but semantically invalid (can't add int to string).

**Output**: `semantic_errors.txt`

---

### 7. `codegen.h` and `codegen.c` - Code Generator
**Purpose**: Translates the AST into assembly code for the target machine.

**What it does**:
- Traverses the AST and emits assembly instructions
- Manages register allocation (7 general-purpose registers R1-R7, plus R0 for return value)
- Generates function prologues/epilogues (save/restore frame pointer, allocate stack space)
- Converts expressions to arithmetic operations
- Generates control flow (if-then-else, while loops) using labels and jumps
- Handles function calls (push arguments, call, get return value)

**Register Management**:
- `cg_alloc_reg()`: Allocates an available register
- `cg_free_reg()`: Returns register to pool
- Linear scan allocation (simple but effective for this compiler)

**Memory Access**:
- Local variables: `[FP-offset]` (Frame Pointer minus offset)
- Parameters: Also accessed via `[FP-offset]`
- Stack-based calling convention

**Key Functions**:
- `codegen_generate()`: Main entry point, generates code for entire program
- `cg_generate_function()`: Generates code for one function
- `cg_generate_expr()`: Generates code for expressions
- `cg_generate_statement()`: Generates code for statements
- `cg_generate_if()` / `cg_generate_while()`: Control flow generation

**For beginners**: This is like translating a recipe from English to another language. The AST says "add a and b", and the code generator writes "LOAD R1, [FP-8] ; a" followed by "ADD R1, R1, R2" to perform the addition in assembly.

**Output**: `codegen.asm` (assembly code)

---

### 8. `machine_code.h` and `machine_code.c` - Machine Code Generator
**Purpose**: Converts assembly code into binary machine code (relocatable and absolute).

**What it does:**
- **Relocatable Code Generation**: Converts assembly to relocatable object code
  - Encodes instructions as binary opcodes
  - Tracks label references and addresses that need relocation
  - Creates relocation table for unresolved references
- **Absolute Code Generation**: Converts relocatable code to absolute machine code
  - Resolves all label addresses
  - Applies base address (default: 0x1000)
  - Produces executable binary code

**Two-Pass Process:**
1. **Pass 1**: Collects all labels and their addresses
2. **Pass 2**: Generates machine code with relocation information

**Instruction Encoding:**
- Each instruction is 4 bytes
- Format: `[Opcode (1 byte)] [Operand1 (1 byte)] [Operand2 (1 byte)] [Operand3/Address (1 byte)]`
- Opcodes: LOAD=0x01, STORE=0x02, ADD=0x10, JMP=0x30, etc.

**Key Functions:**
- `generate_relocatable_code()` - Converts assembly to relocatable object code
- `generate_absolute_code()` - Converts relocatable to absolute code

**For beginners**: This is like converting human-readable assembly into the actual binary numbers that a processor understands. Relocatable code can be moved in memory, absolute code is fixed at a specific address.

**Outputs:**
- `codegen_reloc.obj` - Relocatable machine code (object file format)
- `codegen_abs.bin` - Absolute machine code (ready to execute)

---

### 9. `lexer_support.h` and `lexer_support.c` - Lexical Analysis Support
**Purpose**: Provides infrastructure for tracking tokens, symbols, and errors during lexical analysis.

**What it does**:
- Maintains three linked lists:
  1. **Symbol table**: All identifiers and literals seen (with occurrence counts)
  2. **Token stream**: Every token with its location
  3. **Error list**: All lexical errors encountered
- Provides functions to record and dump this information

**Key Functions**:
- `lex_support_init()`: Initializes data structures
- `lex_support_record_token()`: Records a token in the stream
- `lex_support_record_symbol()`: Records a symbol (identifier/literal)
- `lex_support_record_error()`: Records a lexical error
- `lex_support_dump_tokens()`: Writes token stream to file
- `lex_support_dump_symbols()`: Writes symbol table to file
- `lex_support_dump_errors()`: Writes errors to file
- `lex_support_finalize()`: Cleans up memory

**For beginners**: This is like a secretary taking notes during a meeting. It records everything that happens during lexical analysis so you can review it later.

**Output**: `lexer_tokens.txt`, `lexer_symbols.txt`, `lexer_errors.txt`

---

## Generated Files (Build Artifacts)

These files are automatically generated during the build process:

### 10. `parser.tab.c` and `parser.tab.h`
**Generated by**: `bison -d parser.y`
**Purpose**: C code implementing the parser (state machine, parsing tables)
- `parser.tab.c`: Parser implementation
- `parser.tab.h`: Token definitions and function declarations

### 11. `lex.yy.c`
**Generated by**: `flex scanner.l`
**Purpose**: C code implementing the lexical analyzer (finite-state automaton)

### 12. `compiler` (executable)
**Generated by**: `gcc` linking all object files
**Purpose**: The final compiler executable

---

## Output Files (Generated During Compilation)

These files are created when you run the compiler:

### 13. `lexer_tokens.txt`
**Contents**: Complete token stream with line:column positions
**Format**: `line:col TOKEN_NAME lexeme`

### 14. `lexer_symbols.txt`
**Contents**: Lexical symbol table (all identifiers and literals)
**Format**: `lexeme kind first=line:col count=N`

### 15. `lexer_errors.txt`
**Contents**: All lexical errors (unknown characters, malformed numbers, etc.)

### 16. `derivation_steps.txt`
**Contents**: Parser derivation (which grammar rules were applied)
**Format**: One production rule per line

### 17. `symbol_table.txt`
**Contents**: Semantic symbol tables (all scopes, symbols, types, offsets)
**Format**: Hierarchical listing of scopes and their symbols

### 18. `semantic_errors.txt`
**Contents**: All semantic errors (type mismatches, undeclared identifiers, etc.)
**Format**: `Semantic Error [line N]: description`

### 19. `codegen.asm`
**Contents**: Generated assembly code (only if no semantic errors)
**Format**: Assembly instructions for target machine

### 20. `codegen_reloc.obj`
**Contents**: Relocatable machine code (only if no semantic errors)
**Format**: Binary object file with relocation information
- Address | Opcode | Operands | Relocation Info
- Includes relocation table for unresolved references

### 21. `codegen_abs.bin`
**Contents**: Absolute machine code (only if no semantic errors)
**Format**: Binary executable code with all addresses resolved
- Base address: 0x1000 (default)
- All labels and jumps resolved to absolute addresses

---

## Test Files

### `tests/` Directory
Contains test programs to validate the compiler:

- **`test_pass.src`**: Simple valid program (class with attribute)
- **`test01.src`**: Valid function with arithmetic
- **`test02.src`**: Valid function with float operations
- **`test03.src`**: Invalid program (undeclared variable)
- **`test04.src`**: Invalid program (type mismatch)
- **`error01.src`**: Multiple semantic errors (duplicate params, type errors)
- **`error02.src`**: Extensive error test cases

Each test has corresponding `.log` and `.asm` files showing compiler output.

---

## Documentation Files

### `Report.md`
**Purpose**: Final project report documenting:
- Register allocation scheme
- Memory usage scheme
- Code generation phases and semantic actions
- Test results

### `BUILD_AND_RUN.md`
**Purpose**: Instructions for building and running the compiler

### `FileStructure.md` (this file)
**Purpose**: Complete documentation of all project files

---

## Compilation Pipeline Flow

```
Source Code (test01.src)
    ↓
[Lexer] scanner.l → Token Stream
    ↓
[Parser] parser.y → AST (in memory)
    ↓
[Semantic Pass A] semantic.c → Symbol Tables
    ↓
[Semantic Pass B] semantic.c → Type Checking
    ↓
[Code Generator] codegen.c → Assembly (codegen.asm)
    ↓
[Machine Code Generator] machine_code.c → Relocatable Code (codegen_reloc.obj)
    ↓
[Machine Code Generator] machine_code.c → Absolute Code (codegen_abs.bin)
```

**Data Flow**:
1. `main.c` opens source file
2. `scanner.l` (via `lex.yy.c`) produces tokens
3. `parser.y` (via `parser.tab.c`) builds AST using `ast.c` (AST serves as IR - Intermediate Representation)
4. `semantic.c` uses `symbol_table.c` to analyze AST
5. `codegen.c` uses symbol tables to generate assembly
6. `machine_code.c` converts assembly to relocatable machine code
7. `machine_code.c` converts relocatable code to absolute machine code
8. All phases write output files

---

## Key Concepts for Beginners

### Token vs Symbol
- **Token**: A categorized piece of text (e.g., `FUNC`, `ID:add`, `LPAREN`)
- **Symbol**: An identifier or literal value (e.g., `add`, `42`, `3.14`)

### AST vs Symbol Table
- **AST**: Represents program structure (what statements exist, how they're nested)
- **Symbol Table**: Represents declarations (what variables/functions exist, their types)

### Scope
- **Global scope**: Top-level declarations
- **Function scope**: Local variables and parameters
- **Class scope**: Attributes and methods

### Stack Frame
- Memory allocated on the stack for a function's local variables
- Each variable gets an offset from the frame pointer (FP)
- Example: `local x : int` might be at `[FP-8]`

### Register Allocation
- Registers are fast memory locations in the CPU
- We have 7 general-purpose registers (R1-R7)
- Code generator allocates registers as needed for calculations
- Registers are freed when no longer needed

---

## Summary

This compiler implements a complete compilation pipeline:
- **Lexical Analysis**: Text → Tokens (`scanner.l`, `lexer_support.c`)
- **Syntax Analysis**: Tokens → AST (`parser.y`, `ast.c`)
- **Semantic Analysis**: AST → Validated AST + Symbol Tables (`semantic.c`, `symbol_table.c`)
- **Code Generation**: AST → Assembly (`codegen.c`)

All phases are coordinated by `main.c`, which also handles file I/O and error reporting.


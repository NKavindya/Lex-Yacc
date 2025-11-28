# Architecture and Code Generation Explained

## Part 1: Computer Architectures Explained

### What is x86/x86-64 Architecture?

**x86** (also called **IA-32** or **i386**) and **x86-64** (also called **AMD64** or **Intel 64**) are instruction set architectures (ISAs) used in most personal computers and servers today.

#### x86 (32-bit)
- **Full Name**: Intel 8086 architecture (hence "x86")
- **Bit Width**: 32-bit (processes 32 bits of data at a time)
- **Registers**: 8 general-purpose registers (EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP)
- **Memory**: Can address up to 4 GB of RAM (2³² bytes)
- **Examples**: Intel Pentium, AMD Athlon, early Intel Core processors
- **Status**: Legacy, but still supported

#### x86-64 (64-bit)
- **Full Name**: Extended x86 architecture for 64-bit computing
- **Bit Width**: 64-bit (processes 64 bits of data at a time)
- **Registers**: 16 general-purpose registers (RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8-R15)
- **Memory**: Can address up to 16 exabytes of RAM (2⁶⁴ bytes) - practically unlimited
- **Examples**: Modern Intel Core i3/i5/i7/i9, AMD Ryzen, all modern processors
- **Status**: Current standard for desktop and server computing

**Why "x86"?**
- The name comes from the Intel 8086 processor (1978)
- Later processors (80186, 80286, 80386, 80486) all ended in "86"
- The "x" represents the variable number (8086, 80386, etc.)
- x86-64 extends x86 to 64-bit while maintaining backward compatibility

**Key Characteristics:**
- **CISC (Complex Instruction Set Computer)**: Many complex instructions
- **Variable-length instructions**: Instructions can be 1-15 bytes long
- **Backward compatible**: x86-64 can run 32-bit x86 programs
- **Widely used**: Most desktop/laptop computers use x86-64

---

### Other Computer Architectures

#### 1. ARM Architecture
- **Full Name**: Advanced RISC Machine (originally Acorn RISC Machine)
- **Bit Width**: 32-bit (ARMv7) and 64-bit (ARMv8-A)
- **Registers**: 16 general-purpose registers (32-bit) or 31 (64-bit)
- **Characteristics**:
  - **RISC (Reduced Instruction Set Computer)**: Simple, fixed-length instructions
  - **Power efficient**: Uses less power than x86
  - **Widely used**: Smartphones (iPhone, Android), tablets, embedded systems, Apple Silicon (M1/M2/M3)
- **Examples**: Apple M1/M2/M3, Qualcomm Snapdragon, Samsung Exynos, Raspberry Pi

#### 2. RISC-V
- **Full Name**: RISC Five (5th generation RISC architecture)
- **Bit Width**: 32-bit, 64-bit, and 128-bit variants
- **Registers**: 32 general-purpose registers
- **Characteristics**:
  - **Open-source**: Free and open instruction set (unlike x86/ARM which are proprietary)
  - **Modular**: Can be extended with custom instructions
  - **Growing adoption**: Used in embedded systems, research, and some commercial products
- **Examples**: SiFive processors, some embedded devices

#### 3. MIPS
- **Full Name**: Microprocessor without Interlocked Pipelined Stages
- **Bit Width**: 32-bit and 64-bit variants
- **Registers**: 32 general-purpose registers
- **Characteristics**:
  - **RISC architecture**: Simple, fixed-length instructions
  - **Educational**: Often used in computer architecture courses
  - **Declining**: Less common in new products
- **Examples**: Some routers, embedded systems, educational processors

#### 4. PowerPC
- **Full Name**: Performance Optimization With Enhanced RISC - Performance Computing
- **Bit Width**: 32-bit and 64-bit variants
- **Registers**: 32 general-purpose registers
- **Characteristics**:
  - **RISC architecture**: Used in older Apple Macs (pre-2006)
  - **High performance**: Good for servers and workstations
  - **Limited use**: Mostly legacy systems now
- **Examples**: Older Apple Macs, some IBM servers, game consoles (PlayStation 3, Xbox 360)

#### 5. SPARC
- **Full Name**: Scalable Processor Architecture
- **Bit Width**: 32-bit and 64-bit variants
- **Registers**: 32 general-purpose registers (with register windows)
- **Characteristics**:
  - **RISC architecture**: Used in high-end servers
  - **Register windows**: Unique feature for efficient function calls
  - **Limited use**: Mostly Oracle servers
- **Examples**: Oracle SPARC servers

---

### Architecture Comparison Table

| Architecture | Type | Bit Width | Primary Use | Examples |
|--------------|------|-----------|-------------|----------|
| **x86-64** | CISC | 64-bit | Desktop/Laptop/Server | Intel Core, AMD Ryzen |
| **x86** | CISC | 32-bit | Legacy systems | Intel Pentium, AMD Athlon |
| **ARM** | RISC | 32/64-bit | Mobile/Embedded/Apple Silicon | iPhone, Android, M1/M2/M3 |
| **RISC-V** | RISC | 32/64-bit | Embedded/Research | SiFive, embedded devices |
| **MIPS** | RISC | 32/64-bit | Education/Embedded | Routers, educational |
| **PowerPC** | RISC | 32/64-bit | Legacy/Server | Old Macs, IBM servers |
| **SPARC** | RISC | 32/64-bit | High-end servers | Oracle servers |

---

### CISC vs RISC

**CISC (Complex Instruction Set Computer)** - x86/x86-64:
- **Many instructions**: Hundreds of different instructions
- **Complex instructions**: Single instruction can do multiple operations
- **Variable length**: Instructions vary in size (1-15 bytes)
- **Memory-to-memory**: Can operate directly on memory
- **Example**: `ADD [memory_address], value` (adds directly to memory)

**RISC (Reduced Instruction Set Computer)** - ARM, RISC-V, MIPS:
- **Few instructions**: Dozens of simple instructions
- **Simple instructions**: Each instruction does one thing
- **Fixed length**: All instructions same size (usually 32 bits)
- **Load-store**: Must load to register, operate, then store
- **Example**: `LOAD R1, [memory_address]` then `ADD R1, R1, value` then `STORE R1, [memory_address]`

---

## Part 2: Our Compiler's Code Generation

### What Does Our Compiler Generate?

Our compiler generates **assembly code for a simple register machine**, NOT real x86 or ARM assembly. This is a **hypothetical/toy architecture** designed for educational purposes.

#### Why a Simple Register Machine?

1. **Educational**: Easier to understand than real architectures
2. **Portable**: Not tied to any specific real processor
3. **Simplified**: Focuses on compiler concepts, not architecture details
4. **Complete**: Still demonstrates all code generation concepts

#### Our Target Architecture

**Register Machine Specifications:**
- **Registers**: 7 general-purpose registers (R1-R7) + 1 return register (R0)
- **Memory**: Stack-based with frame pointer (FP) and stack pointer (SP)
- **Word Size**: 8 bytes (64 bits)
- **Instruction Set**: Simple, high-level instructions

**Instruction Set:**
```
Data Movement:
  LOAD  reg, [address]     ; Load from memory to register
  STORE reg, [address]      ; Store from register to memory
  LOADI reg, immediate      ; Load immediate integer value
  LOADF reg, immediate      ; Load immediate float value
  MOV   reg1, reg2          ; Copy register to register

Arithmetic:
  ADD   reg1, reg1, reg2    ; reg1 = reg1 + reg2
  SUB   reg1, reg1, reg2    ; reg1 = reg1 - reg2
  MUL   reg1, reg1, reg2    ; reg1 = reg1 * reg2
  DIV   reg1, reg1, reg2    ; reg1 = reg1 / reg2

Comparison:
  CMPEQ reg1, reg1, reg2    ; reg1 = (reg1 == reg2) ? 1 : 0
  CMPNE reg1, reg1, reg2    ; reg1 = (reg1 != reg2) ? 1 : 0
  CMPLT reg1, reg1, reg2    ; reg1 = (reg1 < reg2) ? 1 : 0
  CMPGT reg1, reg1, reg2    ; reg1 = (reg1 > reg2) ? 1 : 0
  CMPLE reg1, reg1, reg2    ; reg1 = (reg1 <= reg2) ? 1 : 0
  CMPGE reg1, reg1, reg2    ; reg1 = (reg1 >= reg2) ? 1 : 0

Logical:
  AND   reg1, reg1, reg2    ; reg1 = reg1 AND reg2
  OR    reg1, reg1, reg2    ; reg1 = reg1 OR reg2
  NOT   reg1, reg1         ; reg1 = NOT reg1
  NEG   reg1, reg1         ; reg1 = -reg1

Control Flow:
  JMP   label               ; Unconditional jump
  JZ    reg, label          ; Jump if register is zero
  CALL  label               ; Call function
  RET                       ; Return from function

Stack Operations:
  PUSH  reg                 ; Push register onto stack
  POP   reg                 ; Pop from stack to register

I/O:
  READ  reg                 ; Read input to register
  WRITE reg                 ; Write register to output
```

#### Example Generated Assembly

**Source Code:**
```c
func add(a : int, b : int) -> int {
    local result : int;
    result := a + b;
    return(result);
}
```

**Generated Assembly:**
```asm
; Auto-generated assembly

FUNC_add:
    PUSH RA              ; Save return address
    PUSH FP               ; Save old frame pointer
    MOV FP, SP            ; Set new frame pointer
    SUB SP, SP, #24       ; Reserve 24 bytes for locals
    LOAD R1, [FP-8]       ; Load parameter 'a' from stack
    LOAD R2, [FP-16]      ; Load parameter 'b' from stack
    ADD R1, R1, R2        ; R1 = a + b
    STORE R1, [FP-24]     ; Store result to local variable
    LOAD R1, [FP-24]      ; Load result back
    MOV R0, R1            ; Move result to return register
    JMP FUNC_add_END      ; Jump to function end
FUNC_add_END:
    MOV SP, FP            ; Restore stack pointer
    POP FP                ; Restore old frame pointer
    POP RA                ; Restore return address
    RET                   ; Return to caller
```

**Explanation:**
- **Function Prologue**: Saves return address (RA) and frame pointer (FP), sets up new stack frame
- **Local Storage**: Reserves space on stack for local variables
- **Parameter Access**: Parameters accessed via `[FP-offset]` (negative offset from frame pointer)
- **Expression Evaluation**: Loads operands, performs operation, stores result
- **Return**: Moves result to R0 (return register), jumps to epilogue
- **Function Epilogue**: Restores stack pointer and frame pointer, returns to caller

---

## Part 3: File-by-File Explanation

### Source Files and Their Roles

#### 1. `main.c` - Compiler Driver
**Purpose**: Main entry point that orchestrates all compiler phases.

**What it does:**
1. **Initialization**: Opens source file, initializes lexer support
2. **Lexical Analysis**: Calls parser (which uses lexer internally)
3. **Syntax Analysis**: Parser builds AST
4. **Semantic Analysis**: Two passes (Pass A: symbol tables, Pass B: type checking)
5. **Code Generation**: If no errors, generates assembly code
6. **Output Files**: Writes all artifacts to files

**Outputs Created:**
- `derivation_steps.txt` - Parser derivation log
- `lexer_tokens.txt` - All tokens recognized
- `lexer_symbols.txt` - Lexical symbol table
- `lexer_errors.txt` - Lexical errors
- `symbol_table.txt` - Semantic symbol tables
- `semantic_errors.txt` - Semantic errors
- `codegen.asm` - Generated assembly code (if no semantic errors)

**For beginners**: This is the "conductor" - it doesn't do the work itself, but coordinates all phases.

---

#### 2. `scanner.l` - Lexical Analyzer
**Purpose**: Converts source code text into tokens.

**What it does:**
- Reads source code character by character
- Recognizes patterns (keywords, identifiers, numbers, operators)
- Skips whitespace and comments
- Tracks line/column numbers
- Records tokens and symbols

**Outputs:**
- Token stream (passed to parser)
- `lexer_tokens.txt` - List of all tokens with locations
- `lexer_symbols.txt` - Symbol table (identifiers, literals)
- `lexer_errors.txt` - Lexical errors (malformed numbers, unknown characters)

**Example Input**: `func add(x : int) -> int { return(x); }`
**Example Output**: `[FUNC] [ID:add] [(] [ID:x] [:] [INTEGER] [)] [ARROW] [INTEGER] [{] [RETURN] [(] [ID:x] [)] [;] [}]`

---

#### 3. `parser.y` - Syntax Analyzer
**Purpose**: Verifies tokens form valid program structure and builds AST.

**What it does:**
- Receives token stream from lexer
- Applies grammar rules to build Abstract Syntax Tree (AST)
- Logs derivation steps
- Handles syntax errors with recovery

**Outputs:**
- AST (in memory, passed to semantic analyzer)
- `derivation_steps.txt` - Shows which grammar rules were applied

**Example Derivation:**
```
prog -> funcDef
funcDef -> funcHead funcBody
funcHead -> FUNC ID ( fParams ) => returnType
fParams -> ID : type
...
```

---

#### 4. `ast.h` and `ast.c` - Abstract Syntax Tree
**Purpose**: Data structure representing program structure.

**What it does:**
- Defines AST node structure (kind, name, type, children, siblings)
- Provides functions to create, manipulate, and print AST nodes
- Used by parser to build tree, semantic analyzer to traverse, code generator to generate code

**Outputs:**
- AST structure (in memory)
- AST printed to console (for debugging)

---

#### 5. `symbol_table.h` and `symbol_table.c` - Symbol Table Management
**Purpose**: Manages scopes and symbol information.

**What it does:**
- Creates and manages symbol tables (scopes)
- Inserts symbols (variables, functions, classes, parameters)
- Looks up symbols in scope hierarchy
- Calculates memory offsets for variables
- Tracks frame sizes for stack allocation

**Outputs:**
- Symbol tables (in memory, used by semantic analyzer and code generator)
- `symbol_table.txt` - Text representation of all symbol tables

**Example Output:**
```
Scope: global
  Symbol: add (FUNC, int, line 1)
    Parameters:
      x (PARAM, int, line 1)
    Scope: add
      Symbol: result (VAR, int, line 2, offset: 24)
```

---

#### 6. `semantic.c` - Semantic Analyzer
**Purpose**: Performs semantic analysis (type checking, scope validation).

**What it does:**
- **Pass A**: Builds symbol tables (declarations)
- **Pass B**: Checks types, validates usage, reports semantic errors
- Validates function calls, assignments, return statements
- Checks control flow conditions

**Outputs:**
- Symbol tables (built in Pass A)
- `semantic_errors.txt` - All semantic errors found

**Example Errors:**
```
Semantic Error [line 3]: Type mismatch: left is int, right is string
Semantic Error [line 5]: Call to undefined function 'foo'
Semantic Error [line 7]: Return type mismatch: expected int, got float
```

---

#### 7. `codegen.h` and `codegen.c` - Code Generator
**Purpose**: Generates assembly code from AST.

**What it does:**
- Traverses AST
- Allocates registers (R1-R7)
- Generates instructions for expressions, statements, functions
- Manages stack frames (prologue/epilogue)
- Handles function calls, control flow, I/O

**Outputs:**
- `codegen.asm` - Generated assembly code

**Key Functions:**
- `codegen_generate()` - Main entry point
- `cg_generate_function()` - Generates function prologue/epilogue
- `cg_generate_statement()` - Generates code for statements
- `cg_generate_expr()` - Generates code for expressions
- `cg_alloc_reg()` / `cg_free_reg()` - Register allocation

---

#### 8. `lexer_support.h` and `lexer_support.c` - Lexer Support
**Purpose**: Utility functions for lexer to record tokens, symbols, and errors.

**What it does:**
- Records tokens with locations
- Records symbols (identifiers, literals)
- Records lexical errors
- Dumps records to files

**Outputs:**
- `lexer_tokens.txt` - All tokens
- `lexer_symbols.txt` - Symbol table
- `lexer_errors.txt` - Lexical errors

---

### Compilation Pipeline Summary

```
Source Code (test.src)
    ↓
[Lexical Analysis] scanner.l
    ↓
Token Stream
    ↓
[Syntax Analysis] parser.y
    ↓
Abstract Syntax Tree (AST)
    ↓
[Semantic Analysis Pass A] semantic.c
    ↓
Symbol Tables
    ↓
[Semantic Analysis Pass B] semantic.c
    ↓
Type-Checked AST
    ↓
[Code Generation] codegen.c
    ↓
Assembly Code (codegen.asm)
```

---

### Output Files Summary

| File | Created By | Purpose |
|------|------------|---------|
| `lexer_tokens.txt` | Lexer | List of all tokens with locations |
| `lexer_symbols.txt` | Lexer | Lexical symbol table (identifiers, literals) |
| `lexer_errors.txt` | Lexer | Lexical errors (malformed numbers, etc.) |
| `derivation_steps.txt` | Parser | Grammar rules applied during parsing |
| `symbol_table.txt` | Semantic Analyzer | Semantic symbol tables (scopes, types) |
| `semantic_errors.txt` | Semantic Analyzer | Semantic errors (type mismatches, etc.) |
| `codegen.asm` | Code Generator | Generated assembly code (if no errors) |

---

### How to Run the Compiler

```bash
# Compile the compiler
cd DesignProject
flex scanner.l
bison -d parser.y
gcc -o compiler lex.yy.c parser.tab.c main.c ast.c symbol_table.c semantic.c codegen.c lexer_support.c

# Run on a source file
./compiler tests/test01.src

# Check outputs
cat lexer_tokens.txt      # See all tokens
cat lexer_symbols.txt     # See lexical symbols
cat symbol_table.txt      # See semantic symbol tables
cat semantic_errors.txt   # See semantic errors (if any)
cat codegen.asm           # See generated assembly (if no errors)
```

---

## Summary

1. **Architectures**: x86/x86-64 are CISC architectures used in most PCs. ARM is RISC used in mobile devices. Our compiler targets a simple educational register machine.

2. **Code Generation**: Our compiler generates assembly code for a hypothetical register machine (not real x86/ARM), designed for educational purposes.

3. **File Roles**: Each file has a specific role in the compilation pipeline - lexer (tokens), parser (AST), semantic analyzer (type checking), code generator (assembly).

4. **Outputs**: The compiler produces multiple output files documenting each phase - tokens, symbols, symbol tables, errors, and finally assembly code.

This design allows students to understand compiler concepts without getting bogged down in the complexities of real processor architectures, while still demonstrating all the key phases of compilation.


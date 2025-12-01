# Build Guide - Complete Compiler with Machine Code Generation

## Quick Start

### Step 1: Navigate to DP Directory
```bash
cd /home/nkavindya/EntgraRepo/Uni/Lex-Yacc/DP
```

### Step 2: Clean Previous Builds (Optional)
```bash
make clean
```

### Step 3: Build the Complete Compiler
```bash
make compiler
```

This will build the `compiler` executable that includes all phases:
- Lexical Analysis
- Syntax Analysis  
- Semantic Analysis
- Code Generation (IR, Assembly, Relocatable, Absolute)

### Step 4: Run the Compiler
```bash
./compiler <source_file>
```

Example:
```bash
./compiler tests/test03_function.src
```

## Generated Output Files

After running the compiler, you'll get these files:

### Lexical Analysis Outputs
- **`lexer_tokens.txt`** - Complete token stream with line:column positions
- **`lexer_symbols.txt`** - Lexical symbol table (identifiers and literals)
- **`lexer_errors.txt`** - Lexical errors (if any)

### Syntax Analysis Outputs
- **`derivation_steps.txt`** - Parser derivation (grammar rules applied)

### Semantic Analysis Outputs
- **`symbol_table.txt`** - Hierarchical symbol tables (all scopes, symbols, types, offsets)
- **`semantic_errors.txt`** - Semantic errors (if any)

### Code Generation Outputs (Machine Code)
- **`codegen.ir`** - Intermediate Representation (3AC - Three Address Code)
- **`codegen.asm`** - x86-32 Assembly Code (human-readable)
- **`codegen.reloc`** - Relocatable Machine Code (with relocation table)
- **`codegen.abs`** - Absolute Machine Code (with fixed addresses)

## Viewing Machine Code Generation

### 1. View Assembly Code (Human-Readable)
```bash
cat codegen.asm
```

This shows x86-32 assembly instructions like:
```asm
_add:
    push EBP
    mov EBP, ESP
    sub ESP, 4
    mov EAX, DWORD PTR [EBP+8]
    mov ECX, DWORD PTR [EBP+12]
    add EAX, ECX
    mov DWORD PTR [EBP-4], EAX
    mov EAX, DWORD PTR [EBP-4]
_add_END:
    mov ESP, EBP
    pop EBP
    ret
```

### 2. View Intermediate Representation
```bash
cat codegen.ir
```

Shows 3AC format:
```
function add:
  prologue
    t0 = load x
    t1 = load y
    t2 = t0 +, t1
    store result, t2
    t3 = load result
    return t3
  epilogue
```

### 3. View Relocatable Machine Code
```bash
cat codegen.reloc
```

Shows relocatable object format with relative addresses and relocation table.

### 4. View Absolute Machine Code
```bash
cat codegen.abs
```

Shows absolute machine code with fixed addresses (starting at 0x00401000).

## Testing Different Source Files

The `tests/` directory contains various test cases:

```bash
# Simple function
./compiler tests/test03_function.src

# Control structures (if/while)
./compiler tests/test05_if_while.src

# Function calls
./compiler tests/test09_nested_calls.src

# Complex expressions
./compiler tests/test07_expressions.src
```

## Troubleshooting

### Build Errors

If you get build errors:

1. **Check dependencies**:
   ```bash
   which flex bison gcc
   ```

2. **Clean and rebuild**:
   ```bash
   make clean
   make compiler
   ```

3. **Check for missing files**:
   ```bash
   ls -la *.c *.h *.l *.y
   ```

### Runtime Errors

If the compiler fails to run:

1. **Check if executable exists**:
   ```bash
   ls -l compiler
   ```

2. **Check semantic errors**:
   ```bash
   cat semantic_errors.txt
   ```
   Code generation is skipped if semantic errors exist.

3. **Check input file**:
   ```bash
   cat tests/test03_function.src
   ```

## Makefile Targets

```bash
make compiler      # Build complete compiler (recommended)
make clean         # Remove all generated files
make all           # Build all components (parser_rd, semantic_analyzer, compiler)
make test          # Run test suite (for recursive descent parser)
```

## Architecture

The compiler targets **x86-32 architecture** and uses:
- **Calling Convention**: cdecl (caller cleans stack)
- **Word Size**: 4 bytes (32-bit)
- **Registers**: EAX, EBX, ECX, EDX, ESI, EDI (EBP/ESP reserved)
- **Stack Frame**: Standard x86 stack frame with EBP as frame pointer

## Example: Complete Workflow

```bash
# 1. Build
cd /home/nkavindya/EntgraRepo/Uni/Lex-Yacc/DP
make compiler

# 2. Compile a source file
./compiler tests/test03_function.src

# 3. View all outputs
echo "=== Tokens ===" && cat lexer_tokens.txt
echo "=== Symbol Table ===" && cat symbol_table.txt
echo "=== Assembly ===" && cat codegen.asm
echo "=== IR ===" && cat codegen.ir
echo "=== Relocatable ===" && cat codegen.reloc
echo "=== Absolute ===" && cat codegen.abs
```

## Notes

- Code generation only occurs if there are **no semantic errors**
- If `semantic_errors.txt` has content, code generation is skipped
- All output files are overwritten on each run
- The compiler uses the semantic analyzer's symbol table for code generation


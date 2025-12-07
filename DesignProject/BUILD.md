# Build Instructions

## Prerequisites

- `flex` (lexical analyzer generator)
- `bison` (parser generator)
- `gcc` (C compiler)
- `nasm` (optional, for assembling generated assembly code)

## Build Process

### Step 1: Generate Lexer
```bash
flex scanner.l
```
Generates `lex.yy.c` from `scanner.l`.

### Step 2: Generate Parser
```bash
bison -d parser.y
```
Generates `parser.tab.c` and `parser.tab.h` from `parser.y`.

### Step 3: Compile
```bash
gcc -o compiler lex.yy.c parser.tab.c main.c ast.c symbol_table.c semantic.c codegen.c lexer_support.c
```

## Complete Build Command

```bash
flex scanner.l && bison -d parser.y && gcc -o compiler lex.yy.c parser.tab.c main.c ast.c symbol_table.c semantic.c codegen.c lexer_support.c
```

## Usage

```bash
./compiler <source_file.src>
```

The compiler generates:
- `lexer_tokens.txt` - token trace
- `lexer_symbols.txt` - lexical symbol table
- `lexer_errors.txt` - lexical errors
- `derivation_steps.txt` - grammar rule applications
- `syntax_error.txt` - syntax errors
- `semantic_errors.txt` - semantic errors
- `symbol_table.txt` - symbol table dump
- `codegen.ir` - intermediate representation
- `codegen.asm` - x86-32 assembly code
- `codegen.reloc` - relocatable machine code
- `codegen.abs` - absolute machine code


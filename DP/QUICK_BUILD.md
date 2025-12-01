# Quick Build Guide - Machine Code Generation

## Build the Compiler

```bash
cd /home/nkavindya/EntgraRepo/Uni/Lex-Yacc/DP
make clean
make compiler
```

## Run the Compiler

```bash
./compiler tests/test_simple.src
```

Or use the automated script:
```bash
./build_and_test.sh
```

## View Generated Machine Code Files

After successful compilation, you'll get these files:

### 1. Assembly Code (Human-Readable)
```bash
cat codegen.asm
```
Shows x86-32 assembly instructions.

### 2. Intermediate Representation
```bash
cat codegen.ir
```
Shows 3AC (Three Address Code) format.

### 3. Relocatable Machine Code
```bash
cat codegen.reloc
```
Shows relocatable object file with relocation table.

### 4. Absolute Machine Code
```bash
cat codegen.abs
```
Shows absolute machine code with fixed addresses.

## Important Notes

- **Code generation only happens if there are NO semantic errors**
- Check `semantic_errors.txt` - if it has content, code generation is skipped
- All output files are in the current directory
- The compiler targets **x86-32 architecture**

## Troubleshooting

If codegen files are not generated:
1. Check `semantic_errors.txt` for semantic errors
2. Check `derivation_steps.txt` to see if parsing succeeded
3. Verify the source file syntax is correct

## Example Output

When successful, you should see:
```
=== Phase 1: Lexical Analysis ===
Lexer output files generated:
  - lexer_tokens.txt (X tokens)
  - lexer_symbols.txt (X symbols)

=== Phase 2: Syntax Analysis ===
Derivation steps written to derivation_steps.txt

=== Phase 3: Semantic Analysis ===
Symbol table written to symbol_table.txt

=== Phase 4: Code Generation ===
IR written to codegen.ir
Assembly written to codegen.asm
Relocatable code written to codegen.reloc
Absolute code written to codegen.abs
```


# Compiler Build and Run Instructions

## Prerequisites
Make sure you have `bison`, `flex`, and `gcc` installed:
```bash
sudo apt-get install bison flex gcc
```

## Step 1: Navigate to Project Directory
```bash
cd /home/nkavindya/EntgraRepo/Lex-Yacc/DesignProject
```

## Step 2: Generate Parser Files (from parser.y)
```bash
bison -d parser.y
```
This creates `parser.tab.c` and `parser.tab.h`.

## Step 3: Generate Lexer Files (from scanner.l)
```bash
flex scanner.l
```
This creates `lex.yy.c`.

## Step 4: Compile the Compiler
```bash
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c lexer_support.c codegen.c -lfl
```
This creates the executable `compiler`.

## Step 5: Run the Compiler on a Test File
```bash
./compiler tests/test01.src
```

## Step 6: View Generated Outputs
After running, you'll have these files:
- `lexer_tokens.txt` - Token stream from lexical analysis
- `lexer_symbols.txt` - Symbol table from lexical phase
- `lexer_errors.txt` - Lexical errors (if any)
- `symbol_table.txt` - Semantic symbol table
- `semantic_errors.txt` - Semantic errors (if any)
- `derivation_steps.txt` - Parser derivation steps
- `codegen.ir` - Intermediate Representation (only if no semantic errors)
- `codegen.asm` - Generated x86 assembly code (only if no semantic errors)
- `codegen.reloc` - Relocatable machine code (only if no semantic errors)
- `codegen.abs` - Absolute machine code (only if no semantic errors)

View them with:
```bash
cat lexer_tokens.txt
cat symbol_table.txt
cat semantic_errors.txt
cat codegen.ir      # Intermediate Representation
cat codegen.asm     # x86 assembly code (MASM/NASM compatible)
cat codegen.reloc   # Relocatable machine code
cat codegen.abs     # Absolute machine code
```

**Note**: The generated `codegen.asm` contains assembly code for our Simple Register Machine architecture. This is a hypothetical/educational architecture designed for learning compiler concepts, not a real processor architecture like x86 or ARM.

## Step 7: Run All Tests
```bash
# Run compiler on all test files
for f in tests/*.src; do
    echo "==== Processing $f ===="
    ./compiler "$f"
    echo ""
done
```

## Quick Rebuild (if you modify source files)
```bash
cd /home/nkavindya/EntgraRepo/Lex-Yacc/DesignProject
bison -d parser.y && flex scanner.l && gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c lexer_support.c codegen.c -lfl
```

## Clean Generated Files
```bash
rm -f compiler parser.tab.c parser.tab.h lex.yy.c *.txt *.asm *.ir *.reloc *.abs
```

## Example: Complete Workflow
```bash
# 1. Build
cd /home/nkavindya/EntgraRepo/Lex-Yacc/DesignProject
bison -d parser.y
flex scanner.l
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c lexer_support.c codegen.c -lfl

# 2. Test on a valid program
./compiler tests/test01.src

# 3. Check outputs
ls -la *.txt *.asm *.ir *.reloc *.abs

# 4. View generated code
cat codegen.ir      # Intermediate Representation
cat codegen.asm     # x86 assembly code
cat codegen.reloc   # Relocatable machine code
cat codegen.abs     # Absolute machine code

# 5. Note: The generated assembly is for a Simple Register Machine architecture
#    This is a hypothetical architecture for educational purposes.
#    It is not real x86/ARM/MIPS assembly and cannot be assembled with standard assemblers.

# 5. Test on an invalid program (should show errors, no codegen)
./compiler tests/error01.src
cat semantic_errors.txt
```


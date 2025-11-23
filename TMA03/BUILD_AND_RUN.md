# TMA03 Build and Run Instructions

## Prerequisites
Make sure you have `bison`, `flex`, and `gcc` installed:
```bash
sudo apt-get install bison flex gcc
```

## Step 1: Navigate to TMA03 Directory
```bash
cd /home/nkavindya/EntgraRepo/Lex-Yacc/TMA03
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
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c -lfl
```
This creates the executable `compiler`.

## Step 5: Run the Compiler on a Test File
```bash
./compiler tests/test01.src
```

## Step 6: View Generated Outputs
After running, you'll have these files:
- `symbol_table.txt` - Semantic symbol table
- `semantic_errors.txt` - Semantic errors (if any)

View them with:
```bash
cat symbol_table.txt
cat semantic_errors.txt
```

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
cd /home/nkavindya/EntgraRepo/Lex-Yacc/TMA03
bison -d parser.y && flex scanner.l && gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c -lfl
```

## Clean Generated Files
```bash
rm -f compiler parser.tab.c parser.tab.h lex.yy.c *.txt
```

## Example: Complete Workflow
```bash
# 1. Build
cd /home/nkavindya/EntgraRepo/Lex-Yacc/TMA03
bison -d parser.y
flex scanner.l
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c -lfl

# 2. Test on a valid program
./compiler tests/test01.src

# 3. Check outputs
ls -la *.txt

# 4. View symbol table
cat symbol_table.txt

# 5. Test on an invalid program (should show errors)
./compiler tests/error01.src
cat semantic_errors.txt
```

## Note on Parser Type
This implementation uses **Bison (LR parser generator)**, but the grammar is structured as **LL(1) compatible**:
- All left recursion has been removed
- Right-recursive forms are used for expressions
- The grammar structure follows LL(1) principles
- Bison can parse LL(1) grammars efficiently

See `Report.md` for detailed explanation of the LL(1) grammar transformation.


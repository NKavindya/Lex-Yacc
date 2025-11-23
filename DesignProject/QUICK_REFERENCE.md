# Quick Reference Guide

## Commands to Run TMA03

```bash
# Navigate to TMA03 directory
cd /home/nkavindya/EntgraRepo/Lex-Yacc/TMA03

# Build the compiler
bison -d parser.y
flex scanner.l
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c -lfl

# Run on a test file
./compiler tests/test01.src

# View outputs
cat symbol_table.txt
cat semantic_errors.txt
```

See `TMA03/BUILD_AND_RUN.md` for detailed instructions.

## Commands to Run Design Project

```bash
# Navigate to DesignProject directory
cd /home/nkavindya/EntgraRepo/Lex-Yacc/DesignProject

# Build the compiler
bison -d parser.y
flex scanner.l
gcc -o compiler main.c parser.tab.c lex.yy.c ast.c semantic.c symbol_table.c lexer_support.c codegen.c -lfl

# Run on a test file
./compiler tests/test01.src

# View outputs
cat lexer_tokens.txt
cat symbol_table.txt
cat semantic_errors.txt
cat derivation_steps.txt
cat codegen.asm
```

See `BUILD_AND_RUN.md` for detailed instructions.

## LL(1) Grammar Structure - Proof

### Key Evidence:

1. **No Left Recursion in Expressions**:
   ```yacc
   expr → relExpr exprPrime        # NOT: expr → expr AND expr
   arithExpr → term arithExprPrime # NOT: arithExpr → arithExpr + term
   term → factor termPrime         # NOT: term → term * factor
   ```

2. **Right-Recursive Forms**:
   - All list productions use right-recursion
   - Prime nonterminals (`exprPrime`, `arithExprPrime`, `termPrime`) handle chains

3. **Derivation Logs Show LL(1) Structure**:
   ```bash
   ./compiler tests/test01.src
   cat derivation_steps.txt
   # Shows: expr -> relExpr exprPrime (right-recursive expansion)
   ```

4. **Grammar Analysis**:
   - See `TMA03/Report.md` for FIRST/FOLLOW sets
   - See `LL1_VERIFICATION.md` for verification steps

### Why Bison (LR) is Used:

- **Grammar Structure**: LL(1) compatible (right-recursive, no left recursion)
- **Parser Implementation**: Bison (LR parser generator)
- **Compatibility**: LL(1) grammars are a subset of LALR(1) - Bison can parse them
- **Benefits**: Robust error recovery, widely used, efficient

**Key Point**: The grammar is structured as LL(1), which is what TMA02 requires. The parser implementation (Bison) is a tool choice, not a grammar type.

## Documentation Files

- `Report.md` - Design Project main report
- `TMA03.md` - TMA03 semantic analysis documentation
- `TMA03/Report.md` - TMA02 syntax analysis report (LL(1) transformation)
- `GRAMMAR_COVERAGE.md` - Grammar coverage analysis
- `LL1_CONVERSION_GUIDE.md` - LL(1) conversion guide
- `LL1_VERIFICATION.md` - How to verify LL(1) structure
- `BUILD_AND_RUN.md` - Build instructions for Design Project
- `TMA03/BUILD_AND_RUN.md` - Build instructions for TMA03
- `FileStructure.md` - File structure explanation


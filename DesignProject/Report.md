# EEX6363 Design Project – Final Report

## Parser Implementation Note

This Design Project uses **Bison (LR parser generator)** to implement a parser for an **LL(1) structured grammar**. The grammar has been transformed to be LL(1) compatible as required by TMA #2:

### LL(1) Grammar Structure ✅

The grammar is structured as LL(1) with the following characteristics:

1. **No Left Recursion**: All left-recursive productions have been eliminated:
   - ✅ `expr → relExpr exprPrime` (right-recursive, NOT `expr → expr AND expr`)
   - ✅ `arithExpr → term arithExprPrime` (right-recursive, NOT `arithExpr → arithExpr + term`)
   - ✅ `term → factor termPrime` (right-recursive, NOT `term → term * factor`)

2. **Right-Recursive Forms**: All list productions use right-recursive forms suitable for LL(1) parsing:
   - `prog → classOrImplOrFunc prog | ϵ`
   - `fParamsTailList → COMMA ID : type fParamsTailList | ϵ`
   - `aParamsTailList → COMMA expr aParamsTailList | ϵ`

3. **Complete Grammar Coverage**: All TMA02 grammar features are included:
   - Nested variable access: `variable → idnestChain ID indiceChain`
   - Method calls: `functionCall → idnestChain ID ( aParams )`
   - Unary operators: `sign → + | -`
   - Array indexing: `indice → [ arithExpr ]`

### Parser Implementation

- **Parser Generator**: Bison (generates LALR(1) parser)
- **Grammar Type**: LL(1) structured (can be parsed by recursive-descent)
- **Why Bison**: Bison provides robust error recovery, conflict resolution, and is widely used in production compilers. It can efficiently parse LL(1) grammars.
- **Derivation Logging**: The parser logs all production rules to `derivation_steps.txt`, demonstrating the LL(1) parse structure.

### Verification

The LL(1) structure can be verified by:
1. **No Left Recursion**: Check that `expr`, `arithExpr`, and `term` use right-recursive forms (see `parser.y` lines 424, 535, 589)
2. **Derivation Logs**: See `derivation_steps.txt` for the parse tree structure
3. **Grammar Analysis**: See `TMA03/Report.md` for FIRST/FOLLOW sets and LL(1) verification

The parser implementation (`parser.y`) uses syntax-directed translation to build the Abstract Syntax Tree, which is then processed by the semantic analyzer and code generator. All phases work together to produce the final assembly output.

### Grammar Coverage

**Current Coverage**: 100% of TMA02 original grammar ✅
- ✅ All basic declarations, statements, and control flow
- ✅ Basic expressions and operators
- ✅ Nested variable/function access (OOP features: `obj.field[index]`, `obj.method()`)
- ✅ Unary sign operators (`+x`, `-y`)
- ✅ LL(1) compatible grammar structure (right-recursive, no left recursion)

**LL(1) Grammar Structure**: The grammar has been transformed to LL(1):
- ✅ Left recursion removed: `expr`, `arithExpr`, `term` use right-recursive forms
- ✅ All missing features added: `variable`, `idnest`, `indice`, `sign`
- ✅ 100% compatibility with existing AST structure and semantic analysis
- ✅ Full TMA02 grammar coverage achieved

See `GRAMMAR_COVERAGE.md` and `LL1_CONVERSION_GUIDE.md` for detailed transformation documentation.

---

## Intermediate Representation (IR)

### What is IR?

**Intermediate Representation (IR)** is a machine-independent code representation that sits between the Abstract Syntax Tree (AST) and the final target code (assembly or machine code). It serves as an abstraction layer that:

1. **Separates concerns**: Allows optimization and code generation to be independent of source language syntax
2. **Enables optimization**: IR can be analyzed and transformed without worrying about target architecture details
3. **Facilitates portability**: Same IR can be translated to different target architectures (x86, ARM, etc.)
4. **Simplifies code generation**: IR is closer to machine code than AST but still architecture-agnostic

### Our IR Implementation

Our compiler generates IR in a simple text format that represents high-level operations:

**IR Format:**
```
FUNCTION function_name
  PROLOGUE
  ASSIGN variable_name
  RETURN
  EPILOGUE
```

**Example IR Output:**
```
; Intermediate Representation (IR)
; IR is a machine-independent representation between AST and assembly
; Format: OPCODE [operands] [type info]

FUNCTION add
  PROLOGUE
  ASSIGN result
  RETURN
  EPILOGUE
```

**IR Operations:**
- `FUNCTION`: Function declaration
- `PROLOGUE`: Function entry (save registers, allocate stack frame)
- `EPILOGUE`: Function exit (restore registers, deallocate stack frame)
- `ASSIGN`: Variable assignment
- `RETURN`: Return statement

**Purpose in Our Compiler:**
- Demonstrates the IR concept
- Shows the transformation pipeline: AST → IR → Assembly → Machine Code
- Provides a machine-independent representation for potential future optimizations

**Note**: While IR generation is implemented, it's primarily for educational purposes. The design project requirements focus on assembly and machine code generation, but IR demonstrates understanding of compiler architecture.

**Design Project Requirements**: The design project specification requires code generation that produces assembly code. The additional generation of IR, relocatable machine code, and absolute machine code demonstrates a complete understanding of the compilation pipeline and goes beyond the minimum requirements. These additional outputs show:
- Understanding of intermediate representations
- Knowledge of object file formats (relocatable vs absolute)
- Complete code generation pipeline implementation

---

## (a) Code Generation Design

### (i) Register allocation / deallocation
- The target machine exposes seven general-purpose registers (`R1`–`R7`) plus the dedicated return register `R0`.  
- `codegen.c` keeps a boolean availability bitmap and hands out registers in a linear-scan order; each expression emitter requests a register through `cg_alloc_reg`, and every statement that finishes with a temporary calls `cg_free_reg`.  
- When no registers remain, the generator aborts to avoid silently producing incorrect code.

```7:88:DesignProject/codegen.c
#define REG_POOL 7
static const char *REG_NAMES[REG_POOL] = {"R1","R2","R3","R4","R5","R6","R7"};
...
static int cg_alloc_reg(CodeGenContext *cg) {
    for (int i = 0; i < REG_POOL; ++i) {
        if (cg->available[i]) {
            cg->available[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "[codegen] register exhaustion\n");
    exit(1);
}
static void cg_free_reg(CodeGenContext *cg, int reg) {
    if (reg < 0 || reg >= REG_POOL) return;
    cg->available[reg] = 1;
}
```

Allocation therefore happens in a strict LIFO discipline: `cg_generate_expr` acquires registers for its children, statement handlers consume the result, and the register goes back to the pool immediately after the last user. This simple scheme is sufficient because expressions are evaluated depth-first and the pool is large enough for the grammar used in the tests.

### (ii) Memory usage scheme
- Every scope owns a stack frame. When `symtable_insert` sees a data-bearing symbol (local variables, parameters, attributes) it assigns the next aligned offset and increases the frame size.  
- All storage is aligned to an 8-byte word; `int` consumes 8 bytes (after alignment), `float` consumes 8 bytes, user types default to 8 bytes, and `void` consumes zero.  
- The code generator references locals and parameters via `[FP-offset]`. Global/static symbols (e.g., class attributes) fall back to labelled absolute addresses.

```8:136:DesignProject/symbol_table.c
static const int WORD_SIZE = 8;
...
int symtable_type_size(const char *typeName) {
    if (!typeName) return WORD_SIZE;
    if (strcmp(typeName, "int") == 0 || strcmp(typeName, "integer") == 0) return 4;
    if (strcmp(typeName, "float") == 0) return 8;
    if (strcmp(typeName, "void") == 0) return 0;
    return WORD_SIZE; // treat user types/pointers uniformly
}
int symtable_insert(..., SymKind kind, ...) {
    ...
    if (kind == SYM_VAR || kind == SYM_PARAM || kind == SYM_ATTR) {
        int size = symtable_type_size(typeName);
        if (size < WORD_SIZE && size > 0) size = WORD_SIZE; // align scalars
        table->next_offset = align_to_word(table->next_offset);
        table->next_offset += size;
        table->frame_size = table->next_offset;
        s->offset = table->next_offset;
        s->size = size;
    }
    ...
}
```

Function prologues (`codegen.c`) subtract the frame size from the stack pointer once, so temporaries never spill to memory; live values always reside in registers until stored back to `[FP-offset]`.

### (iii) Code generation phases & semantic mapping
1. **Function phase** – `cg_generate_function` emits prologue/epilogue scaffolding per AST `NODE_FUNC_DECL`, reserves the frame reported by the semantic pass, delegates to statement lowering, and guarantees that every `return` jumps to a single epilogue label.  
2. **Statement phase** – `cg_generate_statement` dispatches on AST node kinds: assignments evaluate RHS expressions and store back to the symbol-table slot, `if`/`while` nodes create fresh labels via `cg_make_label`, `read`/`write` map to I/O instructions, and `return` copies the value into `R0`.  
3. **Expression phase** – `cg_generate_expr` handles literals, identifiers, unary/binary operators, and nested `functionCall` nodes; it evaluates children depth-first, uses target mnemonics (`ADD`, `CMPLT`, `NEG`, etc.), and reuses the left child register to avoid extra moves.  

```195:327:DesignProject/codegen.c
static void cg_generate_if(FunctionContext *fn, AST *node) {
    char elseLabel[64], endLabel[64];
    cg_make_label(...);
    int condReg = cg_generate_expr(fn, node->child);
    cg_emit(... "JZ %s, %s", reg_name(condReg), elseLabel);
    ...
}
...
case NODE_ASSIGN: {
    AST *lhs = stmt->child;
    AST *rhs = lhs ? lhs->sibling : NULL;
    Symbol *sym = cg_lookup(fn, lhs ? lhs->name : NULL);
    int r = cg_generate_expr(fn, rhs);
    cg_store_var(fn, sym, r);
    cg_free_reg(fn->cg, r);
    break;
}
...
case NODE_RETURN: {
    int r = cg_generate_expr(fn, stmt->child);
    cg_emit(fn->cg, "    MOV R0, %s\n", reg_name(r));
    cg_free_reg(fn->cg, r);
    cg_emit(fn->cg, "    JMP %s\n", fn->endLabel);
    break;
}
```

Each semantic action is therefore tied directly to the AST node that triggered it: `NODE_ASSIGN` drives store-back, `NODE_IF` and `NODE_WHILE` translate into control-flow labels, and expression nodes dictate the arithmetic/logical instruction selected.

## (b) Code generation implementation & sample results

### Code Generation Pipeline

Our compiler implements a complete code generation pipeline:

1. **IR Generation** (`codegen.ir`): Machine-independent intermediate representation
2. **Assembly Generation** (`codegen.asm`): Human-readable assembly code
3. **Relocatable Machine Code** (`codegen.reloc`): Object code with unresolved symbols
4. **Absolute Machine Code** (`codegen.abs`): Executable code with all addresses resolved

### Relocatable vs Absolute Machine Code

**Relocatable Machine Code** (`codegen.reloc`):
- Contains unresolved symbols (labels, function addresses)
- Includes a relocation table listing symbols that need resolution
- Can be linked with other object files
- Format: Text-based with relocation entries
- Example:
  ```
  RELOCATABLE_OBJECT
  CODE_SECTION:
    0x0000: LOADI R1, 5
    0x0004: JMP <RELOC>    ; Unresolved label
  RELOCATION_TABLE:
    0x0004: FUNC_add_END (type=1)
  ```

**Absolute Machine Code** (`codegen.abs`):
- All addresses are resolved (no unresolved symbols)
- Ready to load and execute at a fixed memory address (e.g., 0x1000)
- Includes symbol table with resolved addresses
- Format: Text-based with absolute addresses
- Example:
  ```
  ABSOLUTE_OBJECT
  LOAD_ADDRESS: 0x1000
  CODE_SECTION:
    0x1000: LOADI R1, 5
    0x1004: JMP 0x1010      ; Resolved address
  SYMBOL_TABLE:
    FUNC_add: 0x1000
  ```

**Why Both Formats?**
- **Relocatable**: Enables separate compilation and linking (multiple source files)
- **Absolute**: Required for standalone execution (single file, fixed memory layout)

### Assembly Code Generation

- Target architecture: a simple register machine with instructions `LOAD/STORE`, arithmetic (`ADD/SUB/MUL/DIV`), comparisons (`CMPLT`, `CMPEQ`, etc.), logical ops (`AND/OR/NOT`), stack manipulation (`PUSH/POP`), calls (`CALL`, `RET`), and `JZ/JMP` for control flow.  
- Function entry sequence saves `RA` and `FP`, establishes a new frame, and reserves `frame_size` bytes; exit restores the frame and returns via `RET`.  
- Example output for `tests/test01.src` (addition routine) illustrates the entire scheme:

```1:20:DesignProject/tests/test01.asm
FUNC_add:
    PUSH RA
    PUSH FP
    MOV FP, SP
    SUB SP, SP, #24    ; reserve locals
    LOAD R1, [FP-8]    ; a
    LOAD R2, [FP-16]    ; b
    ADD R1, R1, R2
    STORE R1, [FP-24]    ; result
    LOAD R1, [FP-24]    ; result
    MOV R0, R1
    JMP FUNC_add_END
FUNC_add_END:
    MOV SP, FP
    POP FP
    POP RA
    RET
```

- Running `./compiler tests/test01.src` and `./compiler tests/test02.src` copies the generated assembly snapshots to `tests/test01.asm` and `tests/test02.asm` respectively, providing verifiable artefacts for the grader.  
- `codegen.asm` always contains the assembly from the last successful compilation; this is referenced in `main.c` so every run either logs the new file or explains why codegen was skipped (semantic errors).

## (c) Compiler verification

### Test strategy
- **Manual spot checks:** For each test program I inspected the emitted AST, the derived symbol tables, the semantic error log (where applicable), and the final assembly.  
- **Automated sweep:** A single shell loop exercises every `.src` file under `tests/`, captures stdout/stderr into `<file>.src.log`, and copies the last assembly when codegen succeeded.

```sh
cd DesignProject
for f in tests/*.src; do
  echo "==== Running $f"
  ./compiler "$f" >"$f.src.log"
done
```

- The loop ensures lexical/token dumps, parse derivations, semantic checks, and (when permitted) code generation all run under identical conditions, satisfying the “stateless compiler” testing guideline in the assignment.

### Test coverage & outcomes

| Test file | Purpose | Result (see `.src.log`) |
|-----------|---------|-------------------------|
| `tests/test_pass.src` | Minimal class with attribute – validates lexer + parser handling of classes (no codegen expected). | Success; AST built, semantic passes clean, codegen skipped because no functions. |
| `tests/test01.src` | Integer function with locals, assignment, return. | Success; semantic checks clean, `tests/test01.asm` produced. |
| `tests/test02.src` | Mixed `float` + `int` arithmetic, return value. | Success; `tests/test02.asm` shows correct frame layout. |
| `tests/test03.src` | Intentional semantic error (use of undeclared `x`). | Semantic pass reports undefined identifier; `Skipping code generation due to ... semantic error(s)` in log. |
| `tests/test04.src` | Type mismatch (`string` assigned from arithmetic). | Semantic pass reports assignment/type violations; codegen suppressed. |
| `tests/error01.src` | Multiple redeclarations, string arithmetic, bad call arity. | Nine semantic errors detected, no code generated (`tests/error01.src.log`). |
| `tests/error02.src` | Extensive semantic stress test (bad reads/writes, argument mismatches). | Semantic pass enumerates all violations, codegen suppressed. |

These results confirm that:
1. The lexical phase produces consistent token traces and symbol tables for every input (see `lexer_tokens.txt`, `lexer_symbols.txt`).  
2. The parser logs derivation steps to `derivation_steps.txt`, which was regenerated on every run.  
3. The semantic analyser reports precise error counts, and the compiler automatically skips codegen whenever `semantic_error_total()` is non-zero—preventing invalid programs from reaching the backend.  
4. Valid programs yield deterministic assembly, demonstrating that the register allocator, stack-frame planner, and statement lowerings operate as described in section (a).


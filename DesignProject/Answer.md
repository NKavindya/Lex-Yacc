# Design Project: Compiler Construction Answers

## (a) High-Level Architecture

The compiler follows a classic multi-pass architecture, organized into the following phases:

1.  **Lexical Analysis (Scanner)**:
    - **Input**: Source code file (`.src`).
    - **Component**: `scanner.l` (Flex/Lex).
    - **Output**: Stream of tokens.
    - **Artifacts**: `lexer_tokens.txt`, `lexer_symbols.txt`.

2.  **Syntax Analysis (Parser)**:
    - **Input**: Stream of tokens.
    - **Component**: `parser.y` (Bison/Yacc).
    - **Output**: Abstract Syntax Tree (AST).
    - **Key Structures**: `AST` nodes defined in `ast.h`.

3.  **Semantic Analysis**:
    - **Input**: AST.
    - **Component**: `semantic.c`, `symbol_table.c`.
    - **Process**:
        - **Pass A**: Traverses AST to build Symbol Tables (scopes, variable/function declarations).
        - **Pass B**: Traverses AST to perform type checking and other semantic validations.
    - **Output**: Annotated AST / Symbol Table, `semantic_errors.txt`.

4.  **Code Generation (Backend)**:
    - **Input**: Validated AST and Symbol Table.
    - **Component**: `codegen.c`.
    - **Output**: Target Assembly Code (`codegen.asm`).

5.  **Driver**:
    - **Component**: `main.c`.
    - **Role**: Orchestrates the execution of all phases and handles file I/O.

## (b) Design Description

### [i] Register Allocation/Deallocation Scheme
The implementation uses a **simple pool-based register allocation scheme** with a greedy strategy.
- **Register Pool**: A fixed set of 7 general-purpose registers (`R1` to `R7`) is available.
- **Allocation (`cg_alloc_reg`)**: When a register is needed (e.g., to store an intermediate expression result), the allocator scans the pool from `R1` to `R7` and returns the first available register. If all registers are in use, the compiler terminates with a "register exhaustion" error. This is a basic "on-the-fly" allocation method suitable for simple expressions but lacks spilling capability (saving registers to memory when full).
- **Deallocation (`cg_free_reg`)**: Once an operation consuming a register is complete (e.g., after an `ADD` instruction uses a register as an operand), the register is immediately marked as available in the pool, allowing it to be reused for subsequent operations.

### [ii] Memory Usage Scheme
The compiler employs a **stack-based memory management scheme**.
- **Stack Layout**: The runtime uses a Frame Pointer (`FP`) and a Stack Pointer (`SP`).
  - **Local Variables**: Allocated in the current stack frame relative to `FP`. The symbol table calculates offsets for each variable. Access is typically via `[FP - offset]`.
  - **Function Call Frame**:
    - **Arguments**: Pushed onto the stack by the caller *before* the call.
    - **Return Address (`RA`)**: Saved on the stack (either by the `CALL` instruction or manually).
    - **Saved FP**: The caller's `FP` is saved to restore the context upon return.
    - **Locals**: Space is reserved by subtracting from `SP` (`SUB SP, SP, #frame_size`).
- **Temporary Variables**: Intermediate results of complex expressions are held in registers. If registers were to run out, a more complex scheme would spill them to the stack (though this implementation halts).
- **Function Calls**:
  - **Member Functions**: Not explicitly distinguished in the basic code generation, but the symbol table supports class scopes.
  - **Calculation of Memory**: The `SymTable` tracks `next_offset` and `frame_size`.
    - `int`: 4 bytes (aligned to 8 bytes/word).
    - `float`: 8 bytes.
    - `string`: Treated as a pointer (8 bytes).
    - The `frame_size` is the total bytes required for all local variables and parameters in a scope, aligned to the word size (8 bytes).

### [iii] Code Generation Phases
The code generation process transforms the Abstract Syntax Tree (AST) into target assembly code.

1.  **Function Declaration Phase (`cg_generate_function`)**:
    - **Mapping**: Maps `NODE_FUNC_DECL` to a function label and prologue/epilogue.
    - **Role**: Sets up the stack frame (`PUSH FP`, `MOV FP, SP`, `SUB SP...`) to prepare for local variable storage. It iterates over the function body to generate code for statements.
2.  **Statement Generation Phase (`cg_generate_statement`)**:
    - **Mapping**: Maps statement nodes (`NODE_ASSIGN`, `NODE_IF`, `NODE_WHILE`, `NODE_RETURN`, `NODE_WRITE`, `NODE_READ`) to corresponding assembly sequences.
    - **Role**: Controls the flow of execution.
        - `NODE_IF` / `NODE_WHILE`: Generates labels and conditional jumps (`JZ`, `JMP`) to implement control structures.
        - `NODE_ASSIGN`: Evaluates the RHS expression and stores the result into the LHS variable's memory location (`STORE`).
        - `NODE_RETURN`: Evaluates the return value, moves it to the result register (`R0`), and jumps to the function epilogue.
3.  **Expression Generation Phase (`cg_generate_expr`)**:
    - **Mapping**: Maps expression nodes (`NODE_BINARY_OP`, `NODE_INT_LITERAL`, `NODE_ID`, etc.) to arithmetic/logic instructions.
    - **Role**: Evaluates values into registers.
        - `NODE_BINARY_OP`: Recursively evaluates children, then emits an instruction (e.g., `ADD`, `MUL`) using the result registers.
        - `NODE_ID`: Loads a variable's value from memory (`[FP-offset]`) into a register.
        - `NODE_FUNCTION_CALL`: Pushes arguments, emits `CALL`, and retrieves the result from `R0`.

## (c) Code Generation Implementation and Test Results

The code generation scheme has been implemented in `codegen.c`. It successfully generates assembly code for valid source programs.

**Test Results for `test01.src`**:
*Input (`tests/test01.src`)*:
```go
func add(a : int, b : int) -> int {
    local result : int;
    result := a + b;
    return(result);
}
```

*Generated Assembly (`codegen.asm`)*:
```asm
; Auto-generated assembly

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
The generated code correctly sets up the frame, performs the addition, and handles the return value.

## (d) Compiler Verification

The compiler was tested against a suite of test cases, including valid and invalid programs.

- **Valid Programs**:
    - `test01.src`: Passed. Generated correct assembly for function definition and arithmetic.
    - `test02.src`: Passed. Verified float arithmetic and mixed types.
    - `test_pass.src`: Passed. Verified class declaration parsing.
- **Invalid Programs (Semantic Errors)**:
    - `error01.src`: Correctly identified 9 semantic errors (e.g., type mismatches, redeclarations). Code generation was skipped.
    - `error02.src`: Correctly identified 39 semantic errors.
    - `test03.src`: Correctly identified undefined variables.
    - `test04.src`: Correctly identified type mismatch in return statement.

The compiler correctly enforces semantic rules and generates code only for valid inputs.

---

## Additional Explanations

**What is the Code Generation Phase?**
The code generation phase is the final stage of the compilation process (backend). It takes the Intermediate Representation (IR) or Abstract Syntax Tree (AST) produced by the front-end (parser/semantic analyzer) and translates it into the target machine code or assembly language. Its goal is to produce semantically equivalent machine instructions that can be executed by the hardware or a virtual machine. This phase handles details like register allocation, instruction selection, and memory layout.

**What does "Finalize the Compiler" mean?**
Finalizing the compiler means integrating all the separate components—Lexical Analyzer (Scanner), Syntax Analyzer (Parser), Semantic Analyzer, and Code Generator—into a single, cohesive software pipeline. It involves:
1.  Ensuring data flows correctly from one phase to the next (Source -> Tokens -> AST -> Annotated AST -> Code).
2.  Handling errors gracefully at each stage (e.g., stopping code generation if semantic errors exist).
3.  Verifying the complete system against the requirements (TMAs) to ensure it correctly compiles valid programs and rejects invalid ones according to the language specification.

---

## (e) Architecture Explanation: x86 (0x84)

*Note: The term "0x84" in the query is likely a typo for **x86** architecture, a standard family of instruction set architectures. The following explanation focuses on x86.*

### What is x86 Architecture?
**x86** is a family of **Complex Instruction Set Computer (CISC)** architectures developed by Intel. It is the dominant architecture for personal computers and servers today.

**Key Characteristics:**
1.  **CISC Design**: x86 instructions can be complex and perform multiple low-level operations (like loading from memory, adding, and storing back to memory) in a single instruction. This contrasts with RISC (Reduced Instruction Set Computer) where instructions are simpler.
2.  **Variable Length Instructions**: Instructions can vary in length (from 1 byte to 15 bytes), allowing for compact code but making decoding more complex.
3.  **Backward Compatibility**: A defining feature of x86 is its ability to run software written for older generations (e.g., a program written for the 8086 from 1978 can still run on a modern Core i9).
4.  **Register Set**:
    - **General Purpose**: EAX (Accumulator), EBX (Base), ECX (Counter), EDX (Data), etc.
    - **Special Purpose**: EIP (Instruction Pointer), ESP (Stack Pointer), EBP (Base Pointer).
5.  **Memory Access**: Supports complex addressing modes (e.g., `[Base + Index * Scale + Displacement]`).

### Other Common Architectures
To understand x86 better, it helps to compare it with other major architectures:

| Feature | **x86** (Intel/AMD) | **ARM** (Mobile/Apple Silicon) | **MIPS** (Embedded/Academic) |
| :--- | :--- | :--- | :--- |
| **Type** | **CISC** (Complex) | **RISC** (Reduced) | **RISC** (Reduced) |
| **Instruction Length** | Variable (1-15 bytes) | Fixed (usually 4 bytes) | Fixed (4 bytes) |
| **Registers** | Few (~8-16 GPRs) | Many (16-31 GPRs) | Many (32 GPRs) |
| **Power Efficiency** | Generally Lower (High Perf) | Generally Higher (Mobile) | High |
| **Usage** | PCs, Servers, Laptops | Phones, Tablets, IoT, Macs | Routers, Consoles (legacy) |
| **Philosophy** | "Hardware should do more work per instruction" | "Software/Compiler should optimize simple instructions" | "Keep hardware simple and regular" |

---

## (f) Project Code Analysis

### Does this project generate Assembly?
**Yes.** The output of this compiler project is **Assembly Code**.
- **File**: `codegen.asm`
- **Format**: The generated code uses a simplified assembly language similar to a RISC-like instruction set (LOAD, STORE, ADD, JMP) but is designed to run on a hypothetical or simple virtual machine rather than raw x86 hardware directly. However, conceptually and structurally, it is assembly code.

### File Roles and Data Flow
Here is a detailed breakdown of what each file does in the compilation process:

#### 1. Lexical Analysis (The Scanner)
-   **`scanner.l`**: This is the **Lexer** definition file. It defines "Regular Expressions" to recognize words in your language (like `func`, `if`, `123`, `+`). It uses a tool called **Flex** to generate C code (`lex.yy.c`) that reads characters and outputs **Tokens**.
-   **`lexer_support.c`**: Helper functions to record tokens and errors for debugging.
-   **Output**: A stream of tokens (e.g., `ID:x`, `OP:+`, `INT:5`).

#### 2. Syntax Analysis (The Parser)
-   **`parser.y`**: This is the **Parser** grammar file. It defines the "Grammar Rules" (BNF) of the language (e.g., "A function has a name, parameters, and a body"). It uses a tool called **Bison** to generate C code (`parser.tab.c`) that takes tokens and builds a structure.
-   **`ast.c` / `ast.h`**: Defines the **Abstract Syntax Tree (AST)**. The parser builds this tree in memory. It's a hierarchical representation of the code (e.g., a "Function" node containing a "Statement List" node).
-   **Output**: An AST in memory.

#### 3. Semantic Analysis (The Validator)
-   **`semantic.c`**: This file walks through the AST to check for meaning.
    -   **Pass A**: Finds all variable and function declarations and builds the **Symbol Table**.
    -   **Pass B**: Checks types (e.g., ensuring you don't add a string to an integer) and scope (ensuring variables are declared before use).
-   **`symbol_table.c`**: Manages the "phone book" of variables (names, types, memory offsets).
-   **Output**: A validated AST and a populated Symbol Table.

#### 4. Code Generation (The Builder)
-   **`codegen.c`**: This is the **Backend**. It takes the validated AST and translates it into assembly instructions.
    -   It allocates registers (`R1`, `R2`...).
    -   It manages the stack (pushing/popping values).
    -   It writes text instructions like `ADD R1, R1, R2` to a file.
-   **Output**: The final `codegen.asm` file.

#### 5. The Driver
-   **`main.c`**: The boss. It calls the Lexer, then the Parser, then the Semantic Analyzer, and finally the Code Generator. It manages opening the input file and writing the output files.

### Summary of Output Files
When you run the compiler (e.g., `./compiler tests/test01.src`), it creates:
1.  **`lexer_tokens.txt`**: List of all words found.
2.  **`symbol_table.txt`**: List of all variables and functions found.
3.  **`codegen.asm`**: The final **Assembly Code** (the executable result).
4.  **`semantic_errors.txt`**: Any errors found (if the code was bad).

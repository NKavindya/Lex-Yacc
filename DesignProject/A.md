# Design Project: Compiler Construction Answers

## (a) Design Description

### [i] Register Allocation/Deallocation Scheme
I used a **simple pool method** for registers.
- **Register Pool**: We have 7 registers called `R1` to `R7`.
- **Allocation**: When I need a register (like for math result), I check the pool. I look from `R1` to `R7`. I take the first one that is free. If all registers are busy, the compiler stops and shows "register exhaustion" error. This is easy way but if we have too many calculations, it fails.
- **Deallocation**: When I finish using a register (like after `ADD` is done), I make it free again. So we can use it for next lines.

### [ii] Memory Usage Scheme
I used **stack memory** for this project.
- **Stack Layout**: We use Frame Pointer (`FP`) and Stack Pointer (`SP`).
  - **Local Variables**: These go on the stack. We find them using `FP` minus some number (offset).
  - **Function Call**:
    - First, we push arguments to stack.
    - Then we save Return Address (`RA`).
    - Then we save the old `FP`.
    - Finally, we move `SP` down to make space for local variables.
- **Temporary Variables**: I try to keep them in registers.
- **Memory Calculation**:
  - `int` takes 4 bytes.
  - `float` takes 8 bytes.
  - `string` is pointer, so 8 bytes.
  - I align everything to 8 bytes (word size).

### [iii] Code Generation Phases
This part changes the AST tree into assembly code.

1.  **Function Phase**:
    - It handles `NODE_FUNC_DECL`.
    - It writes the function label. Then it saves `FP` and makes space on stack (`SUB SP`). Then it goes to the body of function.
2.  **Statement Phase**:
    - It handles statements like `IF`, `WHILE`, `ASSIGN`.
    - For `IF` and `WHILE`, it makes labels and uses jump commands (`JZ`, `JMP`).
    - For `ASSIGN`, it calculates the value and saves it to memory (`STORE`).
    - For `RETURN`, it puts value in `R0` and jumps to end.
3.  **Expression Phase**:
    - It handles math and values.
    - `NODE_BINARY_OP`: It does the math (plus, minus) and puts result in register.
    - `NODE_ID`: It loads variable from memory to register (`LOAD`).
    - `NODE_FUNCTION_CALL`: It pushes arguments and calls the function.

## (b) Code Generation Implementation and Test Results

I wrote the code in `codegen.c`. It works for the test files I have.

**Test Results for `test01.src`**:
*Input code*:
```go
func add(a : int, b : int) -> int {
    local result : int;
    result := a + b;
    return(result);
}
```

*Generated Assembly*:
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
The code is correct. It makes stack frame, does the add, and returns.

## (c) Compiler Verification

I tested the compiler with many files.

- **Valid Programs**:
    - `test01.src`: It passed. Assembly is good.
    - `test02.src`: It passed. Float math works.
    - `test_pass.src`: It passed. Class declaration works.
- **Invalid Programs**:
    - `error01.src`: It found 9 errors. It stopped code generation.
    - `error02.src`: It found 39 errors.
    - `test03.src`: It found undefined variables.
    - `test04.src`: It found wrong return type.

The compiler checks rules and only makes code if everything is correct.

---

## Extra Explanations

**What is Code Generation Phase?**
This is the last part of compiler. It takes the AST from the parser and changes it to machine code (assembly). It decides which registers to use and how to use memory.

**What is Finalize the Compiler?**
This means putting all parts together. We connect Scanner, Parser, Semantic Analyzer, and Code Generator.
1. Data goes from Source to Tokens to AST to Code.
2. If there is error, we stop.
3. We check if it follows all rules from the TMAs.

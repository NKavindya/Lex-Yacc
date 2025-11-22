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


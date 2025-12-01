/* 
 * codegen.h - Code Generation Function Prototypes
 * 
 * Provides functions for generating code from the AST:
 * 1. Intermediate Representation (IR): 3-address code format
 * 2. Assembly Code: x86-32 assembly language
 * 3. Relocatable Machine Code: Object file format
 * 4. Absolute Machine Code: Executable format with fixed addresses
 * 
 * Code generation uses the symbol table for:
 * - Variable offsets in stack frames
 * - Function signatures and parameter information
 * - Type information for operations
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"           // Abstract Syntax Tree structures
#include "symbol_table.h"  // Symbol table for variable offsets and type information

/**
 * codegen_generate - Generate x86-32 assembly code from AST
 * 
 * Traverses the AST and generates x86-32 assembly code.
 * Uses the symbol table to determine variable offsets and function information.
 * 
 * @param root Root node of the AST
 * @param global Global symbol table
 * @param outPath Path to output file for assembly code
 * @return 0 on success, non-zero on error
 */
int codegen_generate(AST *root, SymTable *global, const char *outPath);

/**
 * codegen_generate_ir - Generate Intermediate Representation (IR) from AST
 * 
 * Generates 3-address code (three-address code) intermediate representation.
 * IR is machine-independent and easier to optimize than assembly.
 * Format: result = operand1 operator operand2
 * 
 * @param root Root node of the AST
 * @param global Global symbol table
 * @param outPath Path to output file for IR code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_ir(AST *root, SymTable *global, const char *outPath);

/**
 * codegen_generate_relocatable - Generate relocatable machine code from assembly
 * 
 * Assembles the assembly code into relocatable object file format.
 * Relocatable code can be loaded at any memory address (used for linking).
 * 
 * @param asmPath Path to input assembly file
 * @param outPath Path to output file for relocatable machine code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_relocatable(const char *asmPath, const char *outPath);

/**
 * codegen_generate_absolute - Generate absolute machine code from assembly
 * 
 * Assembles the assembly code into absolute executable format.
 * Absolute code has fixed memory addresses (ready to execute).
 * 
 * @param asmPath Path to input assembly file
 * @param outPath Path to output file for absolute machine code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_absolute(const char *asmPath, const char *outPath);

#endif  // CODEGEN_H

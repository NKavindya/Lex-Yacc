/* 
 * codegen.h - code generation function prototypes
 * 
 * provides functions for generating code from the AST:
 * 1. intermediate representation (IR): 3-address code format
 * 2. assembly code: x86-32 assembly language
 * 3. relocatable machine code: object file format
 * 4. absolute machine code: executable format with fixed addresses
 * 
 * code generation uses the symbol table for:
 * - variable offsets in stack frames
 * - function signatures and parameter information
 * - type information for operations
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"           // abstract syntax tree structures
#include "symbol_table.h"  // symbol table for variable offsets and TYPE information

/**
 * codegen_generate - generate x86-32 assembly code from AST
 * 
 * traverses the AST and generates x86-32 assembly code.
 * uses the symbol table to determine variable offsets and function information.
 * 
 * @param root root node of the AST
 * @param global global symbol table
 * @param outPath path to output file for assembly code
 * @return 0 on success, non-zero on error
 */
int codegen_generate(AST *root, SymTable *global, const char *outPath);

/**
 * codegen_generate_ir - generate intermediate representation (IR) from AST
 * 
 * generates 3-address code (three-address code) intermediate representation.
 * IR is machine-independent and easier to optimize than assembly.
 * format: result = operand1 operator operand2
 * 
 * @param root root node of the AST
 * @param global global symbol table
 * @param outPath path to output file for IR code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_ir(AST *root, SymTable *global, const char *outPath);

/**
 * codegen_generate_relocatable - generate relocatable machine code from assembly
 * 
 * assembles the assembly code into relocatable object file format.
 * relocatable code can be loaded at any memory address (used for linking).
 * 
 * @param asmPath path to input assembly file
 * @param outPath path to output file for relocatable machine code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_relocatable(const char *asmPath, const char *outPath);

/**
 * codegen_generate_absolute - generate absolute machine code from assembly
 * 
 * assembles the assembly code into absolute executable format.
 * absolute code has fixed memory addresses (ready to execute).
 * 
 * @param asmPath path to input assembly file
 * @param outPath path to output file for absolute machine code
 * @return 0 on success, non-zero on error
 */
int codegen_generate_absolute(const char *asmPath, const char *outPath);

#endif  // codegen_h

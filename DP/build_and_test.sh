#!/bin/bash

# Build and Test Script for Complete Compiler

echo "=========================================="
echo "Building Complete Compiler"
echo "=========================================="

# Clean previous build
echo "Cleaning previous build..."
make clean

# Build compiler
echo "Building compiler..."
make compiler

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "=========================================="
echo "Build Successful!"
echo "=========================================="
echo ""

# Test with a simple function
TEST_FILE="tests/test_simple.src"

if [ ! -f "$TEST_FILE" ]; then
    echo "Creating test file..."
    cat > "$TEST_FILE" << 'EOF'
func add ( x : integer , y : integer ) => integer {
    local result : integer ;
    result := x + y ;
    return ( result ) ;
} ;
EOF
fi

echo "=========================================="
echo "Running Compiler on: $TEST_FILE"
echo "=========================================="
echo ""

./compiler "$TEST_FILE"

echo ""
echo "=========================================="
echo "Generated Files:"
echo "=========================================="
ls -lh codegen.* 2>/dev/null || echo "No codegen files (check for semantic errors)"
ls -lh lexer_*.txt derivation_steps.txt symbol_table.txt semantic_errors.txt 2>/dev/null

echo ""
echo "=========================================="
echo "Viewing Machine Code Generation:"
echo "=========================================="
echo ""

if [ -f "codegen.asm" ]; then
    echo "--- Assembly Code (codegen.asm) ---"
    head -30 codegen.asm
    echo ""
fi

if [ -f "codegen.ir" ]; then
    echo "--- Intermediate Representation (codegen.ir) ---"
    cat codegen.ir
    echo ""
fi

if [ -f "codegen.reloc" ]; then
    echo "--- Relocatable Code (codegen.reloc) ---"
    head -20 codegen.reloc
    echo ""
fi

if [ -f "codegen.abs" ]; then
    echo "--- Absolute Code (codegen.abs) ---"
    head -20 codegen.abs
    echo ""
fi

echo "=========================================="
echo "Done!"
echo "=========================================="


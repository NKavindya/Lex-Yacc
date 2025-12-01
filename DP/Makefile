CC = gcc
CFLAGS = -Wall -g
LEX = flex
YACC = bison
YFLAGS = -d

# For recursive descent parser (syntax analyzer)
RD_PARSER = parser_rd
RD_OBJS = parser_rd.o lex_rd.yy.o symtab.o main_rd.o

# For semantic analyzer (AST + symbol tables + semantic checks)
SEM_PARSER = semantic_analyzer
SEM_OBJS = parser_sem.tab.o scanner_sem.yy.o ast.o symbol_table.o semantic.o main_sem.o

# For simple Yacc-based parser (optional)
YACC_PARSER = parser_yacc
YACC_OBJS = parser.tab.o lex_yacc.yy.o symtab.o

# Complete compiler (all phases)
COMPILER = compiler
COMPILER_OBJS = main_compiler.o parser_sem.tab.o scanner_sem.yy.o ast.o symbol_table.o semantic.o codegen.o symtab.o

all: $(RD_PARSER) $(SEM_PARSER) $(YACC_PARSER) $(COMPILER)

# Recursive descent parser
$(RD_PARSER): $(RD_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

parser_rd.o: parser_rd.c parser_rd.h common.h symtab.h
	$(CC) $(CFLAGS) -c parser_rd.c

main_rd.o: main_rd.c parser_rd.h
	$(CC) $(CFLAGS) -c main_rd.c

# Semantic analyzer (Bison + Flex + AST + semantic)
$(SEM_PARSER): $(SEM_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

parser_sem.tab.c parser_sem.tab.h: parser_sem.y
	$(YACC) $(YFLAGS) -o parser_sem.tab.c parser_sem.y

parser_sem.tab.o: parser_sem.tab.c parser_sem.tab.h ast.h
	$(CC) $(CFLAGS) -c parser_sem.tab.c -o parser_sem.tab.o

scanner_sem.yy.c: scanner_sem.l
	$(LEX) -o scanner_sem.yy.c scanner_sem.l

scanner_sem.yy.o: scanner_sem.yy.c ast.h
	$(CC) $(CFLAGS) -c scanner_sem.yy.c -o scanner_sem.yy.o

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

symbol_table.o: symbol_table.c symbol_table.h ast.h
	$(CC) $(CFLAGS) -c symbol_table.c

semantic.o: semantic.c semantic.h
	$(CC) $(CFLAGS) -c semantic.c

main_sem.o: main_sem.c ast.h symbol_table.h
	$(CC) $(CFLAGS) -c main_sem.c

# Complete compiler (all phases)
$(COMPILER): $(COMPILER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

main_compiler.o: main_compiler.c ast.h symbol_table.h codegen.h
	$(CC) $(CFLAGS) -c main_compiler.c

codegen.o: codegen.c codegen.h ast.h symbol_table.h
	$(CC) $(CFLAGS) -c codegen.c

# Yacc parser (original, optional)
$(YACC_PARSER): $(YACC_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

parser.tab.c parser.tab.h: parser.y
	$(YACC) $(YFLAGS) parser.y

parser.tab.o: parser.tab.c parser.tab.h common.h symtab.h
	$(CC) $(CFLAGS) -c parser.tab.c

# Lexer for recursive descent parser
lex_rd.yy.c: lexer.l
	$(LEX) -o lex_rd.yy.c lexer.l

lex_rd.yy.o: lexer.l common.h symtab.h parser_rd.h
	$(LEX) -o lex_rd.yy.c lexer.l
	$(CC) $(CFLAGS) -DUSE_RD_PARSER -c lex_rd.yy.c -o lex_rd.yy.o

# Lexer for Yacc parser
lex_yacc.yy.c: lexer.l
	$(LEX) -o lex_yacc.yy.c lexer.l

lex_yacc.yy.o: lexer.l common.h symtab.h parser.tab.h
	$(LEX) -o lex_yacc.yy.c lexer.l
	$(CC) $(CFLAGS) -c lex_yacc.yy.c -o lex_yacc.yy.o

# Symbol table for RD pipeline
symtab.o: symtab.c symtab.h common.h
	$(CC) $(CFLAGS) -c symtab.c

clean:
	rm -f *.o lex*.yy.c scanner*.yy.c parser*.tab.c parser*.tab.h $(RD_PARSER) $(SEM_PARSER) $(YACC_PARSER) $(COMPILER)
	rm -f lexer_tokens.txt lexer_symbols.txt lexer_errors.txt
	rm -f derivation_steps.txt symbol_table.txt semantic_errors.txt
	rm -f codegen.ir codegen.asm codegen.reloc codegen.abs

test: $(RD_PARSER)
	@echo "Testing recursive descent parser..."
	@for test in tests/*.src; do \
		echo "Testing $$test..."; \
		./$(RD_PARSER) $$test derivation_steps.txt 2>&1 | head -20; \
		echo ""; \
	done

.PHONY: all clean test


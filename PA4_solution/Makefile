SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build

CC = gcc
YACC = bison
LEX = flex

CFLAGS = -Wall -g -I$(BUILD_DIR) -I$(SRC_DIR) -I$(LIB_DIR)

TARGET = $(BUILD_DIR)/compiler

DRIVER_SRC    = $(SRC_DIR)/driver.c
CODEGEN_SRC   = $(SRC_DIR)/codegen.c
OPTIMIZER_SRC = $(SRC_DIR)/optimizer.c

AST_SRC       = $(LIB_DIR)/ast.c
ERRORS_SRC    = $(LIB_DIR)/errors.c
IR_GEN_SRC    = $(LIB_DIR)/ir_generator.c
IR_SRC        = $(LIB_DIR)/ir.c
SEMANTICS_SRC = $(LIB_DIR)/semantics.c
SYMTAB_SRC    = $(LIB_DIR)/symbol_table.c

PARSER_SRC    = $(LIB_DIR)/parser.y
LEXER_SRC     = $(LIB_DIR)/lexer.l

PARSER_C      = $(BUILD_DIR)/parser.tab.c
PARSER_H      = $(BUILD_DIR)/parser.tab.h
LEXER_C       = $(BUILD_DIR)/lex.yy.c


all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(PARSER_C) $(LEXER_C) \
           $(AST_SRC) $(ERRORS_SRC) $(IR_GEN_SRC) $(IR_SRC) $(SEMANTICS_SRC) $(SYMTAB_SRC) \
           $(DRIVER_SRC) $(CODEGEN_SRC) $(OPTIMIZER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(YACC) -d -o $(PARSER_C) $(PARSER_SRC)

$(LEXER_C): $(LEXER_SRC) $(PARSER_H)
	$(LEX) -o $(LEXER_C) $(LEXER_SRC)

clean:
	rm -rf $(BUILD_DIR)

test: $(TARGET)
	./$(TARGET) tests/input.txt

.PHONY: all clean test
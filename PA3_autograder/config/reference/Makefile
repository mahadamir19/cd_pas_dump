# config/reference/Makefile

# Compiler settings
CC = gcc
CXX = g++

# Autograder uses flattened structure, so paths are '.'
SRC_DIR = .
BUILD_DIR = build

# CRITICAL FIX: Added -I. so compiler finds 'ast.h' in the root
INCLUDES = -I. -I$(BUILD_DIR)

# Flags
# -Wno-write-strings cleans up C++ warnings about string literals
CFLAGS   = $(INCLUDES) -g -w -Wno-write-strings
CXXFLAGS = $(INCLUDES) -g -Wall -Wno-write-strings

# Source Files (Updated to look in . instead of lib/src)
LEXER_SRC  = $(SRC_DIR)/lexer.l
PARSER_SRC = $(SRC_DIR)/parser.y

AST_SRC       = $(SRC_DIR)/ast.cpp
DRIVER_SRC    = $(SRC_DIR)/driver.cpp
ERRORS_SRC    = $(SRC_DIR)/errors.cpp
SEMANTICS_SRC = $(SRC_DIR)/semantics.cpp
SYMTAB_SRC    = $(SRC_DIR)/symbol_table.cpp
IR_SRC        = $(SRC_DIR)/ir.cpp
IR_GEN_SRC    = $(SRC_DIR)/ir_generator.cpp

# Generated Files
LEXER_C  = $(BUILD_DIR)/lex.yy.c
PARSER_C = $(BUILD_DIR)/parser.tab.c
PARSER_H = $(BUILD_DIR)/parser.tab.h

# Object Files
OBJ_LEX      = $(BUILD_DIR)/lex.o
OBJ_PARSE    = $(BUILD_DIR)/parser.o
OBJ_AST      = $(BUILD_DIR)/ast.o
OBJ_DRIVER   = $(BUILD_DIR)/driver.o
OBJ_ERRORS   = $(BUILD_DIR)/errors.o
OBJ_SEM      = $(BUILD_DIR)/semantics.o
OBJ_SYMTAB   = $(BUILD_DIR)/symbol_table.o
OBJ_IR       = $(BUILD_DIR)/ir.o
OBJ_IR_GEN   = $(BUILD_DIR)/ir_generator.o

TARGET = $(BUILD_DIR)/compiler

# --- Rules ---

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Bison
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	bison -d -o $(PARSER_C) $(PARSER_SRC)

# Flex
$(LEXER_C): $(LEXER_SRC) $(PARSER_H)
	flex -o $(LEXER_C) $(LEXER_SRC)

# C Compilation
$(OBJ_PARSE): $(PARSER_C)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(OBJ_PARSE)

$(OBJ_LEX): $(LEXER_C)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(OBJ_LEX)

# C++ Compilation
$(OBJ_AST): $(AST_SRC)
	$(CXX) $(CXXFLAGS) -c $(AST_SRC) -o $(OBJ_AST)

$(OBJ_DRIVER): $(DRIVER_SRC)
	$(CXX) $(CXXFLAGS) -c $(DRIVER_SRC) -o $(OBJ_DRIVER)

$(OBJ_ERRORS): $(ERRORS_SRC)
	$(CXX) $(CXXFLAGS) -c $(ERRORS_SRC) -o $(OBJ_ERRORS)

$(OBJ_SEM): $(SEMANTICS_SRC)
	$(CXX) $(CXXFLAGS) -c $(SEMANTICS_SRC) -o $(OBJ_SEM)

$(OBJ_SYMTAB): $(SYMTAB_SRC)
	$(CXX) $(CXXFLAGS) -c $(SYMTAB_SRC) -o $(OBJ_SYMTAB)

$(OBJ_IR): $(IR_SRC)
	$(CXX) $(CXXFLAGS) -c $(IR_SRC) -o $(OBJ_IR)

$(OBJ_IR_GEN): $(IR_GEN_SRC)
	$(CXX) $(CXXFLAGS) -c $(IR_GEN_SRC) -o $(OBJ_IR_GEN)

# Link
$(TARGET): $(OBJ_PARSE) $(OBJ_LEX) $(OBJ_AST) $(OBJ_DRIVER) $(OBJ_ERRORS) $(OBJ_SEM) $(OBJ_SYMTAB) $(OBJ_IR) $(OBJ_IR_GEN)
	$(CXX) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
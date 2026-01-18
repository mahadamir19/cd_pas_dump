# config/reference/Makefile

CC = gcc
CXX = g++
YACC = bison
LEX = flex

# Autograder flattens directories, so we use current dir (.)
SRC_DIR = .
BUILD_DIR = build

# Include current dir for headers
CFLAGS   = -g -w -I$(SRC_DIR) -I$(BUILD_DIR) -Wno-write-strings
CXXFLAGS = -g -w -I$(SRC_DIR) -I$(BUILD_DIR) -Wno-write-strings

TARGET = $(BUILD_DIR)/compiler

# --- Source Files ---
# Core Harness (Provided in Reference)
DRIVER_SRC    = $(SRC_DIR)/driver.cpp
AST_SRC       = $(SRC_DIR)/ast.cpp
ERRORS_SRC    = $(SRC_DIR)/errors.cpp
IR_GEN_SRC    = $(SRC_DIR)/ir_generator.cpp
IR_SRC        = $(SRC_DIR)/ir.cpp
SEMANTICS_SRC = $(SRC_DIR)/semantics.cpp
SYMTAB_SRC    = $(SRC_DIR)/symbol_table.cpp

# Student Implementations (PA4)
CODEGEN_SRC   = $(SRC_DIR)/codegen.cpp
OPTIMIZER_SRC = $(SRC_DIR)/optimizer.cpp

# Grammar/Lexer
PARSER_SRC    = $(SRC_DIR)/parser.y
LEXER_SRC     = $(SRC_DIR)/lexer.l

# Generated
PARSER_C      = $(BUILD_DIR)/parser.tab.c
PARSER_H      = $(BUILD_DIR)/parser.tab.h
LEXER_C       = $(BUILD_DIR)/lex.yy.c

# Object Files
OBJS = $(BUILD_DIR)/driver.o \
       $(BUILD_DIR)/codegen.o \
       $(BUILD_DIR)/optimizer.o \
       $(BUILD_DIR)/ast.o \
       $(BUILD_DIR)/errors.o \
       $(BUILD_DIR)/ir_generator.o \
       $(BUILD_DIR)/ir.o \
       $(BUILD_DIR)/semantics.o \
       $(BUILD_DIR)/symbol_table.o \
       $(BUILD_DIR)/parser.tab.o \
       $(BUILD_DIR)/lex.yy.o

# --- Rules ---

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generic C++ Compile Rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# C Compile Rules for Flex/Bison outputs
$(BUILD_DIR)/parser.tab.o: $(PARSER_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.o: $(LEXER_C)
	$(CC) $(CFLAGS) -c $< -o $@

# Bison
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(YACC) -d -o $(PARSER_C) $(PARSER_SRC)

# Flex
$(LEXER_C): $(LEXER_SRC) $(PARSER_H)
	$(LEX) -o $(LEXER_C) $(LEXER_SRC)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
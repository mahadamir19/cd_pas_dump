# config/reference/Makefile

# Compilers
CC = gcc
CXX = g++
BISON = bison
FLEX = flex

# Directories (Flattened: Autograder puts everything in root)
SRC_DIR = .
BUILD_DIR = build

# Flags
# -I. ensures headers are found in root
# -Wno-write-strings suppresses the "ISO C++ forbids converting string constant" warnings
CFLAGS   = -I$(SRC_DIR) -I$(BUILD_DIR) -Wno-write-strings
CXXFLAGS = -I$(SRC_DIR) -I$(BUILD_DIR) -Wno-write-strings

# Sources
LEXER_SRC  = $(SRC_DIR)/lexer.l
PARSER_SRC = $(SRC_DIR)/parser.y
AST_SRC    = $(SRC_DIR)/ast.cpp
DRIVER_SRC = $(SRC_DIR)/driver.cpp

# Generated Files
LEXER_C  = $(BUILD_DIR)/lex.yy.c
PARSER_C = $(BUILD_DIR)/parser.tab.c
PARSER_H = $(BUILD_DIR)/parser.tab.h

# Object Files
OBJ_LEX    = $(BUILD_DIR)/lex.o
OBJ_PARSE  = $(BUILD_DIR)/parser.o
OBJ_AST    = $(BUILD_DIR)/ast.o
OBJ_DRIVER = $(BUILD_DIR)/driver.o

# Output Executable
TARGET = $(BUILD_DIR)/parser

# --- Rules ---

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 1. Run Bison (Generates .c and .h)
$(PARSER_C) $(PARSER_H): $(PARSER_SRC) | $(BUILD_DIR)
	$(BISON) -d -o $(PARSER_C) $(PARSER_SRC)

# 2. Run Flex (Generates .c)
# Depends on PARSER_H because lexer usually includes it
$(LEXER_C): $(LEXER_SRC) $(PARSER_H) | $(BUILD_DIR)
	$(FLEX) -o $(LEXER_C) $(LEXER_SRC)

# 3. Compile Parser (Use GCC -> Treats as C)
$(OBJ_PARSE): $(PARSER_C)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(OBJ_PARSE)

# 4. Compile Lexer (Use GCC -> Treats as C)
# This FIXES the "redefinition of yylineno" error
$(OBJ_LEX): $(LEXER_C)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(OBJ_LEX)

# 5. Compile AST (Use G++ -> Treats as C++)
$(OBJ_AST): $(AST_SRC)
	$(CXX) $(CXXFLAGS) -c $(AST_SRC) -o $(OBJ_AST)

# 6. Compile Driver (Use G++ -> Treats as C++)
$(OBJ_DRIVER): $(DRIVER_SRC)
	$(CXX) $(CXXFLAGS) -c $(DRIVER_SRC) -o $(OBJ_DRIVER)

# 7. Link Everything (Use G++)
$(TARGET): $(OBJ_PARSE) $(OBJ_LEX) $(OBJ_AST) $(OBJ_DRIVER)
	$(CXX) -o $(TARGET) $(OBJ_PARSE) $(OBJ_LEX) $(OBJ_AST) $(OBJ_DRIVER)

clean:
	rm -rf $(BUILD_DIR)
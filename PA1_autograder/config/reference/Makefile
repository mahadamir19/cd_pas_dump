# Compiler Config
CXX = g++
CC = gcc
LEX = flex

# Directories
BUILD_DIR = build
SRC_DIR = .

# Files
LEXER_SRC = $(SRC_DIR)/lexer.l
DRIVER_SRC = $(SRC_DIR)/driver.cpp

# Intermediate Objects
LEXER_C = $(BUILD_DIR)/lex.yy.c
LEXER_OBJ = $(BUILD_DIR)/lex.yy.o
DRIVER_OBJ = $(BUILD_DIR)/driver.o

# Final Executable
EXE = $(BUILD_DIR)/lexer.exe

all: $(EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 1. Generate C code from Flex
$(LEXER_C): $(LEXER_SRC) | $(BUILD_DIR)
	$(LEX) -o $(LEXER_C) $(LEXER_SRC)

# 2. Compile Lexer (AS C CODE)
# CRITICAL FIX: Added -I. so it can find 'tokens.h' in the root folder
$(LEXER_OBJ): $(LEXER_C)
	$(CC) -c $(LEXER_C) -o $(LEXER_OBJ) -I.

# 3. Compile Driver (AS C++ CODE)
$(DRIVER_OBJ): $(DRIVER_SRC) | $(BUILD_DIR)
	$(CXX) -c $(DRIVER_SRC) -o $(DRIVER_OBJ) -I.

# 4. Link them together
$(EXE): $(DRIVER_OBJ) $(LEXER_OBJ)
	$(CXX) $(DRIVER_OBJ) $(LEXER_OBJ) -o $(EXE)

clean:
	rm -rf $(BUILD_DIR)
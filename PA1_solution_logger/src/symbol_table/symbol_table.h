#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

typedef struct SymbolNode {
    char* token_type;
    char* lexeme;
    int line_no;
    struct SymbolNode* next;
} SymbolNode;

// Function Prototypes
void insert_symbol(const char* type, const char* lexeme, int line);
void print_symbol_table();
void free_symbol_table();

#endif
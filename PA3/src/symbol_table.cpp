#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Symbol* symbol_table_head = NULL;

void init_symbol_table() {
}

void push_scope() {
}

void pop_scope() {
}

int insert_symbol(char* name, char* type, SymbolKind kind) {
}

Symbol* lookup_symbol(char* name) {
}

Symbol* lookup_local_symbol(char* name) {
}

// for debugging
void print_symbol_table() {
}
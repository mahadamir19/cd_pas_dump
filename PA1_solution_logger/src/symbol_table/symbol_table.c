#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static SymbolNode* head = NULL;
static SymbolNode* tail = NULL;

void insert_symbol(const char* type, const char* lexeme, int line) {
    SymbolNode* newNode = (SymbolNode*)malloc(sizeof(SymbolNode));

    newNode->token_type = strdup(type);
    newNode->lexeme = strdup(lexeme);
    newNode->line_no = line;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void print_symbol_table() {
    SymbolNode* current = head;
    while (current != NULL) {
        printf("%d\t%s\t\t%s\n", current->line_no, current->token_type, current->lexeme);
        current = current->next;
    }
}

void free_symbol_table() {
    SymbolNode* current = head;
    while (current != NULL) {
        SymbolNode* temp = current;
        current = current->next;
        free(temp->token_type);
        free(temp->lexeme);
        free(temp);
    }
}
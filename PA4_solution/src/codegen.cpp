#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "codegen.h"
#include "ir.h"
#include "symbol_table.h" 

using namespace std;

bool is_valid_var(char *str) {
    if (!str) return false;

    if (isdigit(str[0])) return false;
    if (str[0] == '-' && isdigit(str[1])) return false;

    if (str[0] == '"') return false;

    return true;
}

void generate_mips(IRInst *ir_head, char *output_filename)
{
    FILE *out = fopen(output_filename, "w");

    fprintf(out, ".data\n");

    vector<string> seen_vars;
    IRInst *scan = ir_head;

    while (scan) {
        char* candidates[3] = {NULL, NULL, NULL};

        switch (scan->op) {
            case IR_ADD: case IR_SUB: case IR_MUL: case IR_DIV:
            case IR_ASSIGN: case IR_ARRAY_LOAD: case IR_ARRAY_STORE:
            case IR_EQ: case IR_NEQ: case IR_LT: case IR_GT: case IR_LTE: case IR_GTE:
            case IR_AND: case IR_OR:
                candidates[0] = scan->arg1;
                candidates[1] = scan->arg2;
                candidates[2] = scan->result;
                break;

            case IR_IFZ:
                candidates[0] = scan->arg1; 
                break;

            case IR_READ: case IR_PRINT: case IR_PARAM:
                candidates[0] = scan->arg1;
                break;

            case IR_CALL:
                candidates[0] = scan->result;
                break;

            case IR_RET:
                candidates[0] = scan->result;
                break;
                
            default:
                break;
        }

        for (int i = 0; i < 3; i++) {
            char* var = candidates[i];
            
            if (is_valid_var(var)) {
                string s(var);
                
                if (find(seen_vars.begin(), seen_vars.end(), s) == seen_vars.end()) {
                    fprintf(out, "%s: .word 0\n", var);
                    seen_vars.push_back(s);
                }
            }
        }

        scan = scan->next;
    }
    
    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");

    IRInst* current = ir_head;
    while (current)
    {
        current = current->next;
    }

    // exit syscall
    fprintf(out, "li $v0, 10\n");
    fprintf(out, "syscall\n");

    fclose(out);
}
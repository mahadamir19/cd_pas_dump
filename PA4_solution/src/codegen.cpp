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

void emit_load(FILE *out, char *arg, const char *reg) {
    if (!arg) return;
    
    if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1]))) {
        fprintf(out, "    li %s, %s\n", reg, arg);
    } else {
        fprintf(out, "    lw %s, %s\n", reg, arg);
    }
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
            case IR_AND: case IR_OR: case IR_MOD:
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
        switch (current->op)
        {
        case IR_ADD:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    add $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_SUB:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    sub $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_MUL:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    mul $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_DIV:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    div $t0, $t1\n"); 
                fprintf(out, "    mflo $t2\n");     
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_MOD:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    div $t0, $t1\n");
                fprintf(out, "    mfhi $t2\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_ASSIGN:
                emit_load(out, current->arg1, "$t0");
                fprintf(out, "    sw $t0, %s\n", current->result);
                break;

            case IR_LABEL:
                fprintf(out, "%s:\n", current->result);
                break;

            case IR_GOTO:
                fprintf(out, "    j %s\n", current->result);
                break;

            case IR_IFZ: 
                emit_load(out, current->arg1, "$t0");
                fprintf(out, "    beqz $t0, %s\n", current->result);
                break;

            case IR_EQ:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    seq $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_NEQ:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    sne $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_LT:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    slt $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;
            
            case IR_GT:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    sgt $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;
            
            case IR_LTE:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    sle $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_GTE:
                emit_load(out, current->arg1, "$t0");
                emit_load(out, current->arg2, "$t1");
                fprintf(out, "    sge $t2, $t0, $t1\n");
                fprintf(out, "    sw $t2, %s\n", current->result);
                break;

            case IR_FUNC_START:
                fprintf(out, "%s:\n", current->result);
                break;

            case IR_RET:
                if (current->result) {
                    emit_load(out, current->result, "$v0");
                }
                fprintf(out, "    jr $ra\n");
                break;

            case IR_PARAM:
                emit_load(out, current->arg1, "$t0");
                fprintf(out, "    subu $sp, $sp, 4\n");
                fprintf(out, "    sw $t0, ($sp)\n");
                break;

            case IR_CALL:
                fprintf(out, "    jal %s\n", current->arg1);
                if (current->result) {
                    fprintf(out, "    sw $v0, %s\n", current->result);
                }
                break;

            case IR_PRINT:
                emit_load(out, current->arg1, "$a0");
                fprintf(out, "    li $v0, 1\n");
                fprintf(out, "    syscall\n");
                fprintf(out, "    li $a0, 10\n");
                fprintf(out, "    li $v0, 11\n");
                fprintf(out, "    syscall\n");
                break;

            case IR_READ:
                fprintf(out, "    li $v0, 5\n");
                fprintf(out, "    syscall\n");
                fprintf(out, "    sw $v0, %s\n", current->result); 
                break;

            case IR_POP_PARAM:
                fprintf(out, "    lw $t0, ($sp)\n");
                fprintf(out, "    addiu $sp, $sp, 4\n");
                
                fprintf(out, "    sw $t0, %s\n", current->result);
                break;

            default:
                break;
        }
        current = current->next;
    }

    // exit syscall
    fprintf(out, "li $v0, 10\n");
    fprintf(out, "syscall\n");

    fclose(out);
}
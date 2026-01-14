#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "optimizer.h"
#include "ir.h"


static int is_constant_int(char *s) {
    if (!s || *s == '\0') return 0;
    if (*s == '-') s++; 
    while (*s) {
        if (!isdigit(*s)) return 0;
        s++;
    }
    return 1;
}

static IRInst* delete_instruction(IRInst *head, IRInst *to_delete) {
    if (head == to_delete) {
        return head->next; 
    }

    IRInst *prev = head;
    while (prev && prev->next != to_delete) {
        prev = prev->next;
    }

    if (prev) {
        prev->next = to_delete->next;
    }

    if (to_delete->arg1) free(to_delete->arg1);
    if (to_delete->arg2) free(to_delete->arg2);
    if (to_delete->result) free(to_delete->result);
    free(to_delete);

    return prev ? prev->next : NULL;
}


void optimize_ir(IRInst *head) {
    int changed = 0;

    do {
        changed = 0;
        
        int folded = constant_folding(head);
        int eliminated = dead_code_elimination(head);
        
        changed = folded + eliminated;
        
    } while (changed > 0);
}


int constant_folding(IRInst *head) {
    int changed = 0;
    IRInst *current = head;

    while (current) {
        int is_arithmetic = (current->op == IR_ADD || current->op == IR_SUB || 
                             current->op == IR_MUL || current->op == IR_DIV);

        if (is_arithmetic && is_constant_int(current->arg1) && is_constant_int(current->arg2)) {
            
            int v1 = atoi(current->arg1);
            int v2 = atoi(current->arg2);
            int res = 0;
            int valid = 1;

            switch (current->op) {
                case IR_ADD: res = v1 + v2; break;
                case IR_SUB: res = v1 - v2; break;
                case IR_MUL: res = v1 * v2; break;
                case IR_DIV: 
                    if (v2 != 0) res = v1 / v2; 
                    else valid = 0; 
                    break;
                default: valid = 0; break;
            }

            if (valid) {
                current->op = IR_ASSIGN;
                
                free(current->arg1);
                free(current->arg2);

                char buffer[32];
                sprintf(buffer, "%d", res);
                current->arg1 = strdup(buffer);
                current->arg2 = NULL;

                changed++;
            }
        }
        current = current->next;
    }
    return changed;
}


int dead_code_elimination(IRInst *head) {
    int changed = 0;
    IRInst *current = head;
    
    if (!current) return 0;

    while (current && current->next) {
        IRInst *target = current->next;
        int should_delete = 0;

        if ((current->op == IR_GOTO || current->op == IR_RET) && target->op != IR_LABEL) {
            should_delete = 1;
        }

        else if (target->result && (target->op == IR_ADD || target->op == IR_SUB || 
                                    target->op == IR_MUL || target->op == IR_DIV || 
                                    target->op == IR_ASSIGN)) {
            
            char *var_name = target->result;
            int is_used = 0;
            
            IRInst *runner = target->next;
            while (runner) {
                if ((runner->arg1 && strcmp(runner->arg1, var_name) == 0) ||
                    (runner->arg2 && strcmp(runner->arg2, var_name) == 0) ||
                    (runner->op == IR_ARRAY_STORE && (
                        (runner->arg1 && strcmp(runner->arg1, var_name) == 0) || 
                        (runner->arg2 && strcmp(runner->arg2, var_name) == 0) ||
                        (runner->result && strcmp(runner->result, var_name) == 0) 
                    )) ||
                    (runner->op == IR_ARRAY_LOAD && (
                        (runner->arg1 && strcmp(runner->arg1, var_name) == 0) || 
                        (runner->arg2 && strcmp(runner->arg2, var_name) == 0)    
                    )) ||
                    (runner->op == IR_PRINT && strcmp(runner->arg1, var_name) == 0) ||
                    (runner->op == IR_RET && runner->result && strcmp(runner->result, var_name) == 0) ||
                    (runner->op == IR_PARAM && strcmp(runner->arg1, var_name) == 0)) {
                    
                    is_used = 1;
                    break; 
                }

                if (runner->op == IR_LABEL || runner->op == IR_IFZ || 
                    runner->op == IR_GOTO || runner->op == IR_FUNC_START) {
                    is_used = 1; 
                    break;
                }

                if (runner->result && strcmp(runner->result, var_name) == 0) {
                    break; 
                }
                
                runner = runner->next;
            }

            if (!is_used) {
                if (target->op != IR_CALL && target->op != IR_READ && target->op != IR_PRINT) {
                    should_delete = 1;
                }
            }
        }

        if (should_delete) {
            delete_instruction(head, target);
            changed++;
        } else {
            current = current->next;
        }
    }

    return changed;
}
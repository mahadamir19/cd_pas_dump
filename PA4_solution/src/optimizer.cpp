#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>

#include "optimizer.h"
#include "ir.h"

using namespace std;

static bool is_constant_int(const char *s) {
    if (!s || *s == '\0') return false;
    
    if (*s == '-') s++; 
    
    while (*s) {
        if (!isdigit(*s)) return false;
        s++;
    }
    return true;
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

    return prev ? prev->next : nullptr;
}

void optimize_ir(IRInst *head) {
    bool changed = false;

    do {
        changed = false;
        
        int folded = constant_folding(head);
        int eliminated = dead_code_elimination(head);
        
        if (folded > 0 || eliminated > 0) {
            changed = true;
        }
        
    } while (changed);
}

int constant_folding(IRInst *head) {
    int changed_count = 0;
    IRInst *current = head;

    while (current) {
        bool is_arithmetic = (current->op == IR_ADD || current->op == IR_SUB || 
                              current->op == IR_MUL || current->op == IR_DIV);

        if (is_arithmetic && is_constant_int(current->arg1) && is_constant_int(current->arg2)) {
            
            int v1 = stoi(current->arg1);
            int v2 = stoi(current->arg2);
            int res = 0;
            bool valid = true;

            switch (current->op) {
                case IR_ADD: res = v1 + v2; break;
                case IR_SUB: res = v1 - v2; break;
                case IR_MUL: res = v1 * v2; break;
                case IR_DIV: 
                    if (v2 != 0) res = v1 / v2; 
                    else valid = false; 
                    break;
                default: valid = false; break;
            }

            if (valid) {
                current->op = IR_ASSIGN;
                
                if (current->arg1) free(current->arg1);
                if (current->arg2) free(current->arg2);

                string res_str = to_string(res);
                
                current->arg1 = strdup(res_str.c_str()); 
                current->arg2 = nullptr;

                changed_count++;
            }
        }
        current = current->next;
    }
    return changed_count;
}

int dead_code_elimination(IRInst *head) {
    int changed_count = 0;
    IRInst *current = head;
    
    if (!current) return 0;

    while (current && current->next) {
        IRInst *target = current->next;
        bool should_delete = false;

        if ((current->op == IR_GOTO || current->op == IR_RET) && target->op != IR_LABEL) {
            should_delete = true;
        }

        else if (target->result && (target->op == IR_ADD || target->op == IR_SUB || 
                                    target->op == IR_MUL || target->op == IR_DIV || 
                                    target->op == IR_ASSIGN)) {
            
            string var_name = target->result;
            bool is_used = false;
            
            IRInst *runner = target->next;
            while (runner) {
                if ((runner->arg1 && var_name == runner->arg1) ||
                    (runner->arg2 && var_name == runner->arg2)) {
                    is_used = true;
                    break;
                }

                if (runner->op == IR_ARRAY_STORE) {
                    if ((runner->arg1 && var_name == runner->arg1) || 
                        (runner->arg2 && var_name == runner->arg2) ||
                        (runner->result && var_name == runner->result)) {
                        is_used = true;
                        break;
                    }
                }
                if (runner->op == IR_ARRAY_LOAD) {
                    if ((runner->arg1 && var_name == runner->arg1) || 
                        (runner->arg2 && var_name == runner->arg2)) {
                        is_used = true;
                        break; 
                    }
                }

                if (runner->op == IR_PRINT && runner->arg1 && var_name == runner->arg1) {
                    is_used = true; 
                    break;
                }
                if (runner->op == IR_RET && runner->result && var_name == runner->result) {
                    is_used = true; 
                    break;
                }
                if (runner->op == IR_PARAM && runner->arg1 && var_name == runner->arg1) {
                    is_used = true; 
                    break;
                }

                if (runner->op == IR_LABEL || runner->op == IR_IFZ || 
                    runner->op == IR_GOTO || runner->op == IR_FUNC_START) {
                    is_used = true;
                    break;
                }

                if (runner->result && var_name == runner->result) {
                    break; 
                }
                
                runner = runner->next;
            }

            if (!is_used) {
                if (target->op != IR_CALL && target->op != IR_READ && target->op != IR_PRINT) {
                    should_delete = true;
                }
            }
        }

        if (should_delete) {
            delete_instruction(head, target);
            changed_count++;
        } else {
            current = current->next;
        }
    }

    return changed_count;
}
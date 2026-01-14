#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"

char *new_temp()
{
}

char *new_label()
{
}

IRInst *create_instruction(IROp op, char *arg1, char *arg2, char *result)
{
}

void append_instruction(IRInst *head, IRInst *new_inst)
{
}

void print_ir_list(IRInst *head)
{
}

void free_ir_list(IRInst *head)
{
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "ir.h"
#include "ir_generator.h"

static IRInst *ir_head = NULL;

IRInst *generate_ir_from_ast(ASTNode *root)
{
    return ir_head;
}


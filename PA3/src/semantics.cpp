#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "symbol_table.h"
#include "errors.h"

static int semantic_errors = 0;

int semantic_analysis(ASTNode *root)
{
    return semantic_errors;
}


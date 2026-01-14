#include <stdio.h>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

#include "ast.h"         
#include "parser.tab.h"  

extern int yyparse();    
extern ASTNode* root;  
extern FILE* yyin;  

#ifdef __cplusplus
}
#endif

int main(int argc, char **argv) {

    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        yyin = file;
    }

    int result = yyparse();

    if (result == 0) {
        printf("--- AST Structure ---\n");
        print_ast(root, 0);
        free_ast(root);
    } else {
        printf("--- Parse Failed! ---\n");
    }

    return result;
}
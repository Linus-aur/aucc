#include "aucc.h"

extern Token *token;
extern char* user_input;

void gen (Node* node) {
    if (node->kind == ND_NUM) {
        printf("    PUSH %d\n", node->val);
        return;
    }
    
    gen(node->lhs);
    gen(node->rhs);

    printf("    POP RDI\n");
    printf("    POP RAX\n");

    switch(node->kind) {
        case ND_ADD:
            printf("    ADD RAX, RDI\n");
            break;
        case ND_SUB:
            printf("    SUB RAX, RDI\n");
            break;
        case ND_MUL:
            printf("    IMUL RAX, RDI\n");
            break;
        case ND_DIV:
            printf("    CQO\n");
            printf("    IDIV RDI\n");
            break;
        
        case ND_EQUAL:
            puts("    CMP RAX, RDI");
            puts("    SETE AL");
            puts("    MOVZB RAX, AL");
            break;
        case ND_NOT_EQUAL:
            puts("    CMP RAX, RDI");
            puts("    SETNE AL");
            puts("    MOVZB RAX, AL");
            break;
        // ---
        case ND_BIGGER:
            puts("    PUSH RDI");
            puts("    MOV RDI, RAX");
            puts("    POP RAX");
        case ND_LESS:
            puts("    CMP RAX, RDI");
            puts("    SETL AL");
            puts("    MOVZB RAX, AL");
            break;
        // ---
        case ND_BAE:
            puts("    PUSH RDI");
            puts("    MOV RDI, RAX");
            puts("    POP RAX");
        case ND_LAE:
            puts("    CMP RAX, RDI");
            puts("    SETLE AL");
            puts("    MOVZB RAX, AL");
            break;
        
    }

    puts("    PUSH RAX");

}
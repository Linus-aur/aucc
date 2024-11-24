#include "aucc.h"
Token *token;
char* user_input;


int main (int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number\n");
        return 1;
    }
    user_input = argv[1];
    token = tokenize(user_input);
    Node *node = expr();
    
    puts(".intel_syntax noprefix");
    puts(".globl main");
    puts("main:");

    gen(node);

    printf("    POP RAX\n");
    printf("    RET\n");


    puts(".section .note.GNU-stack,\"\",@progbits");
    
    return 0;
}
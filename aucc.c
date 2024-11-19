#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number");
    }
    
    puts(".intel_syntax noprefix");
    
    puts(".globl main");
    puts("main:");
    printf("    MOV RAX, %d\n", atoi(argv[1]));
    printf("    RET\n");

    
    puts(".section .note.GNU-stack,\"\",@progbits");
    
    return 0;
}
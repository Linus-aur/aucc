#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number");
        return 1;
    }

    char* p = argv[1];
    
    puts(".intel_syntax noprefix");
    puts(".globl main");
    puts("main:");


    printf("    MOV RAX, %ld\n", strtol(p, &p, 10));

    while(*p) {
        if (*p == '+') {
            p++;
            printf("    ADD RAX, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if (*p == '-') {
            p++;
            printf("    SUB RAX, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "Unexpected character: '%c'", *p);
    }
    printf("    RET\n");


    puts(".section .note.GNU-stack,\"\",@progbits");
    
    return 0;
}
#ifndef __AUCC_H
#define __AUCC_H
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct Token Token;
Token *tokenize(char* p);
Node* expr();
void gen (Node* node);

typedef enum {
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef enum {
    ND_EQUAL, ND_NOT_EQUAL,
    ND_LESS, ND_LAE, ND_BIGGER, ND_BAE,
    ND_ADD, ND_SUB,
    ND_MUL, ND_DIV,
    ND_NUM,
} NodeKind;



struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

struct Token {
    TokenKind kind;
    Token* next;
    int val;
    char *str;
    int len;
};


#endif
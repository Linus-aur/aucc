#include "aucc.h"

extern Token *token;
extern char* user_input;

char* err_p;

void error(char* loc, char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input - 2;
    fprintf(stderr, "%s\n", user_input);
    if (pos != 0 && loc > user_input)
        fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char* op) {
    if (token->kind != TK_RESERVED ||
            strlen(op) != token->len ||
            memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

void expect(char* op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len ||
            memcmp(token->str, op, token->len))
        error(err_p,
        "'%s' expected, but gets '%s'.", op, token->str);
    token = token->next;
}

int expect_number () {
    if (token->kind != TK_NUM) error(err_p,
        "Integer expected, but it isn't.");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

bool is_endof(char* p) {
    for(p += 1; *p != 0; ++p) {if (*p != ' ') return 0;}
    return 1;

}

Token* new_token(TokenKind kind, Token* cur, char* str, int len) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

Token *tokenize(char* p) {

    Token head;
    head.next = NULL;
    Token* cur = &head;

    while(*p) {
        if (isspace(*p)) {
            p++;
            err_p = p;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' ||
        *p == '(' || *p == ')') {
            char* ns = calloc(2, sizeof(char));
            ns[0] = *p;
            cur = new_token(TK_RESERVED, cur, ns, 1);
            p += 1;
            err_p = p;
            continue;
        }
        
        if (*p == '<' || *p == '>') {
            switch(*(p + 1)) {
                case '=':
                    char* ns = calloc(3, sizeof(char));
                    ns[0] = *p, ns[1] = *(p + 1);
                    cur = new_token(TK_RESERVED, cur, ns, 2);
                    p += 2; break;
                    err_p = p;
                default:
                    if(is_endof(p)) error(err_p,
                        "Integer expected, but it isn't.");
                    char* nv = calloc(2, sizeof(char));
                    nv[0] = *p;
                    cur = new_token(TK_RESERVED, cur, nv, 1);
                    p += 1; break;
                    err_p = p;
            }
            continue;
        }

        if (*p == '!' || *p == '=') {
            switch(*(p + 1)) {
                case '=':
                    char* ns = calloc(3, sizeof(char));
                    ns[0] = *p, ns[1] = *(p + 1);
                    cur = new_token(TK_RESERVED, cur, ns, 2);
                    p += 2; break;
                    err_p = p;
                default:
                    error(err_p,
                        "'%c' is not a complete operator. Do you mean '%c=' ?",
                        *p, *p);
            }
            continue;
        }


        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, -1);
            cur->val = strtol(p, &p, 10);
            err_p = p;
            continue;
        }

        error(err_p, "Unable to tokenize this");

    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    
    return node;
}


Node* expr();
Node* relational();
Node* equality();
Node* add();
Node* mul();
Node* unary();
Node* primary();


Node* expr() {
    return equality();
}
Node* equality() {
    Node* node = relational();
    for(;;) {
        if (consume("=="))
            node = new_node(ND_EQUAL, node, relational());
        else if (consume("!="))
            node = new_node(ND_NOT_EQUAL, node, relational());
        else
            return node;
    }
}
Node* relational() {
    Node* node = add();
    
    for(;;) {
        if (consume("<"))
            node = new_node(ND_LESS, node, add());
        else if (consume("<="))
            node = new_node(ND_LAE, node, add());
        else if (consume(">"))
            node = new_node(ND_BIGGER, node, add());
        else if (consume(">="))
            node = new_node(ND_BAE, node, add());
        else
            return node;
    }
}
Node* add() {
    Node* node = mul();
    
    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}
Node* mul() {
    Node *node = unary();
    
    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, primary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, primary());
        else
            return node;
    }
}
Node* unary() {
    if (consume("+")) return primary();
    if (consume("-")) return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}
Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    return new_node_num(expect_number());
}
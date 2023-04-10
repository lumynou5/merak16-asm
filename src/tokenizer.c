#include "tokenizer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define IS_INST(inst) { \
    const size_t size = sizeof(#inst) - 1; \
    char lower[size]; \
    for (size_t i = 0; i < size; ++i) { \
        lower[i] = tolower((#inst)[i]); \
    } \
    if ((!strncmp(ptr, #inst, size) || !strncmp(ptr, lower, size)) && !isalnum(*(ptr + size))) { \
        appendToken(&curr, TK_INST_##inst, ptr, 0); \
        ptr += size; \
        continue; \
    } \
}

#define IS_REG(reg) { \
    const size_t size = sizeof(#reg) - 1; \
    if ((*ptr == 'A' || *ptr == 'a') && !strncmp(ptr + 1, #reg, size) && !isalnum(*(ptr + 1 + size))) { \
        appendToken(&curr, TK_REG, ptr, reg); \
        ptr += 1 + size; \
        continue; \
    } \
}

static void appendToken(Token** curr, TokenKind kind, const char* str, unsigned int num);

const char* tokenize(const char* src, Token** res) {
    Token head;
    head.next = NULL;
    Token* curr = &head;

    for (const char* ptr = src; *ptr;) {
        if (isspace(*ptr)) {
            ++ptr;
            continue;
        }

        if (isdigit(*ptr)) {
            char* end;
            appendToken(&curr, TK_NUM, ptr, strtoul(ptr, &end, 10));
            ptr = end;
            continue;
        }

        if (*ptr == ',') {
            appendToken(&curr, TK_COM, ptr, 0);
            ++ptr;
            continue;
        }

        if (*ptr == '(') {
            appendToken(&curr, TK_LP, ptr, 0);
            ++ptr;
            continue;
        }

        if (*ptr == ')') {
            appendToken(&curr, TK_RP, ptr, 0);
            ++ptr;
            continue;
        }

        IS_INST(AND);
        IS_INST(OR);
        IS_INST(XOR);
        IS_INST(ADD);
        IS_INST(SUB);
        IS_INST(SLL);
        IS_INST(SRA);
        IS_INST(SRL);
        IS_INST(NOT);
        IS_INST(COM);
        IS_INST(MVHL);
        IS_INST(MVLH);
        IS_INST(MVH);
        IS_INST(LH);
        IS_INST(LI);
        IS_INST(SH);
        IS_INST(SLT);
        IS_INST(SOE);
        IS_INST(BOZ);
        IS_INST(BONZ);
        IS_INST(JAL);
        IS_INST(JALR);
        IS_INST(HALT);

        IS_REG(0);
        IS_REG(1);
        IS_REG(2);
        IS_REG(3);
        IS_REG(4);
        IS_REG(5);
        IS_REG(6);
        IS_REG(7);
        IS_REG(8);
        IS_REG(9);
        IS_REG(10);
        IS_REG(11);
        IS_REG(12);
        IS_REG(13);
        IS_REG(14);
        IS_REG(15);

        // Cleanup.
        freeTokens(head.next);

        return ptr;
    }

    appendToken(&curr, TK_EOF, NULL, 0);
    *res = head.next;
    return NULL;
}

void freeTokens(Token* tk) {
    while (tk != NULL) {
        Token* next = tk->next;
        free(tk);
        tk = next;
    }
}

void appendToken(Token** curr, TokenKind kind, const char* str, unsigned int num) {
    Token* new = malloc(sizeof(Token));
    new->kind = kind;
    new->str = str;
    new->num = num;
    new->next = NULL;
    (*curr)->next = new;
    *curr = new;
}

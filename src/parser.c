#include "parser.h"

size_t parse(Token* tk, unsigned char* buf, size_t cap) {
    size_t len = 0;
    for (; tk->kind != TK_EOF && len < cap; tk = tk->next) {
        switch (tk->kind) {
            case TK_INST_AND:
            case TK_INST_OR:
            case TK_INST_XOR:
            case TK_INST_ADD:
            case TK_INST_SUB:
            case TK_INST_SLL:
            case TK_INST_SRA:
            case TK_INST_SRL:
                if (!parseBin(&tk, buf + len)) return 0;
                break;

            case TK_INST_NOT:
            case TK_INST_COM:
                if (!parseMono(&tk, buf + len)) return 0;
                break;

            case TK_INST_MVHL:
                if (!parseMVHL(&tk, buf + len)) return 0;
                break;

            case TK_INST_MVLH:
                if (!parseMVLH(&tk, buf + len)) return 0;
                break;

            case TK_INST_MVH:
                if (!parseMVH(&tk, buf + len)) return 0;
                break;

            case TK_INST_LH:
                if (!parseLH(&tk, buf + len)) return 0;
                break;

            case TK_INST_LI:
                if (!parseLI(&tk, buf + len)) return 0;
                break;

            case TK_INST_SH:
                if (!parseSH(&tk, buf + len)) return 0;
                break;

            case TK_INST_SLT:
            case TK_INST_SOE:
                if (!parseSet(&tk, buf + len)) return 0;
                break;

            case TK_INST_BOZ:
            case TK_INST_BONZ:
                if (!parseBoz(&tk, buf + len)) return 0;
                break;

            case TK_INST_JAL:
                if (!parseJAL(&tk, buf + len)) return 0;
                break;

            case TK_INST_JALR:
                if (!parseJALR(&tk, buf + len)) return 0;
                break;

            default:
                return 0;
        }
        len += 2;
    }
    return len;
}

bool parseBin(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    TokenKind kind = tk->kind;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs2 = tk->num;

    if ((rd ^ rs1 ^ rs2) >= 8) return false;

    setBuf(buf, 0x0000 | ((kind - TK_INST_AND) << 10) | ((rd >= 8) << 9) |
                ((rs2 % 8) << 6) | ((rs1 % 8) << 3) | (rd % 8));
    *curr = tk;
    return true;
}

bool parseMono(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    TokenKind kind = tk->kind;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    if ((rd ^ rs1) >= 8) return false;

    setBuf(buf, 0x2000 | ((kind - TK_INST_NOT) << 6) |
                ((rd >= 8) << 9) | ((rs1 % 8) << 3) | (rd % 8));
    *curr = tk;
    return true;
}

bool parseMVHL(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    if (!(rd < 8 && rs1 >= 8)) return false;

    setBuf(buf, 0x2080 | ((rs1 - 8) << 3) | rd);
    *curr = tk;
    return true;
}

bool parseMVLH(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    if (!(rd >= 8 && rs1 < 8)) return false;

    setBuf(buf, 0x2280 | (rs1 << 3) | (rd - 8));
    *curr = tk;
    return true;
}

bool parseMVH(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    if (!(rd >= 8 && rs1 >= 8)) return false;

    setBuf(buf, 0x22C0 | ((rs1 - 8) << 3) | (rd - 8));
    *curr = tk;
    return true;
}

bool parseLH(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_LP) return false;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int offset = tk->num;

    tk = tk->next;
    if (tk->kind != TK_RP) return false;

    if ((rd ^ rs1) >= 8) return false;

    setBuf(buf, 0x2400 | ((rd >= 8) << 9) | (offset << 6) | ((rs1 % 8) << 3) | (rd % 8));
    *curr = tk;
    return true;
}

bool parseLI(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rd = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int imm = tk->num;

    // 7 is the mask to get the lowest 3 bits.
    // As a result, higher bits will be discarded.
    setBuf(buf, 0x4000 | ((imm >> 6 & 7) << 10) | ((rd >= 8) << 9) |
                ((imm >> 3 & 7) << 6) | ((imm & 7) << 3) | (rd % 8));
    *curr = tk;
    return true;
}

bool parseSH(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs2 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_LP) return false;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int offset = tk->num;

    tk = tk->next;
    if (tk->kind != TK_RP) return false;

    if ((rs1 ^ rs2) >= 8) return false;

    setBuf(buf, 0x2800 | ((rs1 >= 8) << 9) | ((rs2 % 8) << 6) | ((rs1 % 8) << 3) | offset);
    *curr = tk;
    return true;
}

bool parseSet(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    TokenKind kind = tk->kind;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_COM) return false;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs2 = tk->num;

    if ((rs1 ^ rs2) >= 8) return false;

    setBuf(buf, 0x8000 | ((rs1 >= 8) << 9) | ((rs2 % 8) << 6) | ((rs1 % 8) << 3) | (kind == TK_INST_SOE));
    *curr = tk;
    return true;
}

bool parseBoz(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    TokenKind kind = tk->kind;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int offset = tk->num;

    setBuf(buf, 0x8400 + ((kind == TK_INST_BONZ) << 10) | offset);
    *curr = tk;
    return true;
}

bool parseJAL(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int offset = tk->num;

    setBuf(buf, 0x9000 | offset);
    *curr = tk;
    return true;
}

bool parseJALR(Token** curr, unsigned char* buf) {
    Token* tk = *curr;

    tk = tk->next;
    if (tk->kind != TK_REG) return false;
    unsigned int rs1 = tk->num;

    tk = tk->next;
    if (tk->kind != TK_NUM) return false;
    unsigned int offset = tk->num;

    setBuf(buf,
           0x9400 | ((rs1 >= 8) << 9) | ((offset >> 3 & 7) << 6) | ((rs1 % 8) << 3) | (offset & 7));
    *curr = tk;
    return true;
}

void setBuf(unsigned char* buf, unsigned int val) {
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
}

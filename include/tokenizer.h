typedef enum {
    TK_EOF,
    TK_NUM,
    TK_COM,
    TK_LP,
    TK_RP,
    TK_INST_AND,
    TK_INST_OR,
    TK_INST_XOR,
    TK_INST_ADD,
    TK_INST_SUB,
    TK_INST_SLL,
    TK_INST_SRA,
    TK_INST_SRL,
    TK_INST_NOT,
    TK_INST_COM,
    TK_INST_MVHL,
    TK_INST_MVLH,
    TK_INST_MVH,
    TK_INST_LH,
    TK_INST_LI,
    TK_INST_SH,
    TK_INST_SLT,
    TK_INST_SOE,
    TK_INST_BOZ,
    TK_INST_BONZ,
    TK_INST_JAL,
    TK_INST_JALR,
    TK_INST_HALT,
    TK_REG,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    const char* str;
    unsigned int num;
    Token* next;
};

/// Tokenize the given source.
///
/// \param src The source that has to live as long as the result does.
/// \param res The result that has to be freed manually.
/// \return \c NULL if no error occurred, or the pointer to the illegal character in \c src.
/// \see freeTokens
const char* tokenize(const char* src, Token** res);

/// Free tokens recursively.
///
/// \param tk The most parent token to free.
void freeTokens(Token* tk);

void appendToken(Token** curr, TokenKind kind, const char* str, unsigned int num);

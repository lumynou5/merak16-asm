#include <stdbool.h>
#include <stddef.h>
#include "tokenizer.h"

/// Parse a token sequence into machine code.
///
/// \param tk The token sequence.
/// \param buf The buffer to contain machine code.
/// \param cap The capacity of the buffer, which must be multiple of 2.
/// \param len The length of the machine code.
/// \return If a null pointer returned, an error occured;
///         if it points to an EOF token, parsing is done;
///         else, the buffer is full and the next token is returned.
Token* parse(Token* tk, unsigned char* buf, size_t cap, size_t* len);

bool parseBin(Token** curr, unsigned char* buf);

bool parseMono(Token** curr, unsigned char* buf);

bool parseMVHL(Token** curr, unsigned char* buf);

bool parseMVLH(Token** curr, unsigned char* buf);

bool parseMVH(Token** curr, unsigned char* buf);

bool parseLH(Token** curr, unsigned char* buf);

bool parseLI(Token** curr, unsigned char* buf);

bool parseSH(Token** curr, unsigned char* buf);

bool parseSet(Token** curr, unsigned char* buf);

bool parseBoz(Token** curr, unsigned char* buf);

bool parseJAL(Token** curr, unsigned char* buf);

bool parseJALR(Token** curr, unsigned char* buf);

void setBuf(unsigned char* buf, unsigned int val);

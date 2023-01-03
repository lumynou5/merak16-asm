#include <stdbool.h>
#include <stddef.h>
#include "tokenizer.h"

size_t parse(Token* tk, unsigned char* buf, size_t cap);

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

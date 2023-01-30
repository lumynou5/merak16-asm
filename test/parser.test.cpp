extern "C" {
#include "parser.h"
}

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parse with illegal token sequence", "[parse]") {
    const char* src = "LH a1, a0(0)\n"
                      "LI a2\n";
    Token* tk;
    tokenize(src, &tk);

    unsigned char buf[1024];
    size_t len;
    Token* last = parse(tk, buf, 1024, &len);

    REQUIRE(last == NULL);

    freeTokens(tk);
}

TEST_CASE("Parse with legal token sequence", "[parse]") {
    const char* src = "LH  a1, a0(0)\n"
                      "LI  a2, 1\n"
                      "LI  a3, 1\n"
                      "ADD a1, a1, a2\n"
                      "ADD a2, a2, a3\n";
    const unsigned char expect[] = { 0b00'000'001, 0b001'001'0'0,
                                     0b00'001'010, 0b010'000'0'0,
                                     0b00'001'011, 0b010'000'0'0,
                                     0b10'001'001, 0b000'011'0'0,
                                     0b11'010'010, 0b000'011'0'0 };
    Token* tk;
    tokenize(src, &tk);

    unsigned char buf[1024];
    size_t len;
    Token* end = parse(tk, buf, 1024, &len);

    REQUIRE(end != NULL);
    REQUIRE(end->kind == TK_EOF);
    REQUIRE(len == 10);

    for (size_t i = 0; i < len; ++i) {
        INFO("i = " << i);
        REQUIRE(buf[i] == expect[i]);
    }

    freeTokens(tk);
}

TEST_CASE("Parse with not enough buffer", "[parse]") {
    const char* src = "LH  a1, a0(0)\n"
                      "LI  a2, 1\n"
                      "LI  a3, 1\n"
                      "ADD a1, a1, a2\n"
                      "ADD a2, a2, a3\n";
    const unsigned char expect[] = { 0b00'000'001, 0b001'001'0'0,
                                     0b00'001'010, 0b010'000'0'0,
                                     0b00'001'011, 0b010'000'0'0,
                                     0b10'001'001, 0b000'011'0'0,
                                     0b11'010'010, 0b000'011'0'0 };
    Token* tk;
    tokenize(src, &tk);

    unsigned char buf[8];
    size_t len;
    Token* end = parse(tk, buf, 8, &len);

    REQUIRE(end != NULL);
    REQUIRE(end->kind != TK_EOF);
    REQUIRE(len == 8);

    for (size_t i = 0; i < len; ++i) {
        INFO("i = " << i);
        REQUIRE(buf[i] == expect[i]);
    }

    Token* new_end = parse(end, buf, 8, &len);

    REQUIRE(new_end != NULL);
    REQUIRE(new_end->kind == TK_EOF);
    REQUIRE(len == 2);

    for (size_t i = 0; i < len; ++i) {
        INFO("i = " << i);
        REQUIRE(buf[i] == expect[i + 8]);
    }
}

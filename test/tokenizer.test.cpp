extern "C" {
#include "tokenizer.h"
}

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Tokenize with illegal token.", "[tokenize]") {
    SECTION("Unused character.") {
        const char* src = "~";

        Token* res = NULL;
        const char* err = tokenize(src, &res);

        REQUIRE(err == src);
        REQUIRE(res == NULL);
    }

    SECTION("Illegal token.") {
        const char* src = "FOO";

        Token* res = NULL;
        const char* err = tokenize(src, &res);

        REQUIRE(err == src);
        REQUIRE(res == NULL);
    }

    SECTION("Missing space.") {
        const char* src = "AND0";

        Token* res = NULL;
        const char* err = tokenize(src, &res);

        REQUIRE(err == src);
        REQUIRE(res == NULL);
    }
}

TEST_CASE("Tokenize with legal source.", "[tokenize]") {
    SECTION("Uppercase.") {
        const char* src = "LH A1, A0(0)";

        Token* res = NULL;
        const char* err = tokenize(src, &res);

        REQUIRE(err == NULL);

        Token* curr = res;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_INST_LH);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_REG);
        REQUIRE(curr->num == 1);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_COM);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_REG);
        REQUIRE(curr->num == 0);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_LP);
        
        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_NUM);
        REQUIRE(curr->num == 0);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_RP);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_EOF);
        REQUIRE(curr->next == NULL);

        freeTokens(res);
    }

    SECTION("Lowercase.") {
        const char* src = "lh a1, a0(0)";

        Token* res = NULL;
        const char* err = tokenize(src, &res);

        REQUIRE(err == NULL);

        Token* curr = res;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_INST_LH);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_REG);
        REQUIRE(curr->num == 1);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_COM);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_REG);
        REQUIRE(curr->num == 0);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_LP);
        
        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_NUM);
        REQUIRE(curr->num == 0);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_RP);

        curr = curr->next;
        REQUIRE(curr != NULL);
        REQUIRE(curr->kind == TK_EOF);
        REQUIRE(curr->next == NULL);

        freeTokens(res);
    }
}

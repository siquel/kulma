#include "catch/catch.hpp"
#include "kulma/macros.h"
#include <string.h> // strcmp
TEST_CASE("VA args", "[macro]") {
    SECTION("Count") {
        REQUIRE(KULMA_VA_ARGS_COUNT(1) == 1);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2) == 2);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3) == 3);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3, 4) == 4);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3, 4, 5) == 5);
    }

    SECTION("To string") {
        REQUIRE(strcmp(KULMA_TO_STRING(asd),"asd")==0);
        REQUIRE(strcmp(KULMA_TO_STRING(let there be spaces),"let there be spaces")==0);
    }

    SECTION("Concat") {
        REQUIRE(KULMA_CONCAT(KULMA_VA_ARGS_, COUNT)(1, 2) == 2);
    }

    SECTION("Count of") {
        static const int s_ints[] = {
            1, 2, 3, 4, 5, 6, 7
        };
        static const char* s_strings[] = {
            "top", "kek", "kulma"
        };
        REQUIRE(KULMA_COUNTOF(s_ints) == 7);
        REQUIRE(KULMA_COUNTOF(s_strings) == 3);
    }
}

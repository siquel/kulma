#include "catch/catch.hpp"
#include "kulma/macros.h"

TEST_CASE("VA args", "[macro]") {
    SECTION("Count") {
        REQUIRE(KULMA_VA_ARGS_COUNT(1) == 1);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2) == 2);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3) == 3);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3, 4) == 4);
        REQUIRE(KULMA_VA_ARGS_COUNT(1, 2, 3, 4, 5) == 5);
    }

    SECTION("To string") {
        REQUIRE(KULMA_TO_STRING(asd) == "asd");
        REQUIRE(KULMA_TO_STRING(let there be spaces) == "let there be spaces");
    }

    SECTION("Concat") {
        REQUIRE(KULMA_CONCAT(KULMA_VA_ARGS_, COUNT)(1, 2) == 2);
        REQUIRE(KULMA_CONCAT(KULMA_TO, _STRING)(ff) == "ff");
    }
}
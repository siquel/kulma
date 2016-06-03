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

    SECTION("FOURCC") {
        uint32_t cc = KULMA_MAKEFOURCC('E', 'N', 'K', 'O');
        
        REQUIRE(((uint8_t*)&cc)[0] == 'E');
        REQUIRE(((uint8_t*)&cc)[1] == 'N');
        REQUIRE(((uint8_t*)&cc)[2] == 'K');
        REQUIRE(((uint8_t*)&cc)[3] == 'O');
        // little endian
        //REQUIRE(cc == uint32_t('OKNE')); GCC doesn't like this
        // E = 0x45, N=0x4E, K = 0x4B, O = 0x4F
        REQUIRE(cc == 0x4F4B4E45);
    }
}

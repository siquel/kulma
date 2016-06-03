#include <catch/catch.hpp>
#include <kulma/error.h>
#include <kulma/macros.h>

KULMA_ERROR_RESULT(LOLOBSTER, KULMA_MAKEFOURCC('L', 'O', 'B', 'S'));
KULMA_ERROR_RESULT(ENKO, KULMA_MAKEFOURCC('E', 'N', 'K', 'O'));

TEST_CASE("Errors", "[error]") {
    SECTION("Set error") {
        kulma::Error err;
        KULMA_ERROR_SET((&err), LOLOBSTER, "Lolobster prkl");
        
        REQUIRE(err == LOLOBSTER);
    }

    SECTION("isOk") {
        kulma::Error err;
        REQUIRE(err.isOk());
        KULMA_ERROR_SET((&err), ENKO, "Enko prkl");
        REQUIRE(!err.isOk());
    }

    SECTION("scope") {
        kulma::Error err;
        kulma::ScopedError scoped(&err);
        //err.setError(ENKO, "error"); should crash
    }
}
#include <catch/catch.hpp>
#include <kulma/io/file.h>
#include <kulma/macros.h>

const static char* s_filePath = "unit_test_file_cpp_test_file";

TEST_CASE("File", "[file]")
{
    SECTION("Opening file for read")
    {
        kulma::Error err;
        kulma::ScopedError scoped(&err);

        kulma::FileReader reader;
        REQUIRE(reader.open(s_filePath, &err));
        reader.close();
    }

    SECTION("Opening file for write")
    {
        kulma::Error err;
        kulma::ScopedError scoped(&err);

        kulma::FileWriter writer;
        REQUIRE(writer.open(s_filePath, false, &err));
        char data[] = "this is test string";
        REQUIRE(writer.write(data, sizeof(data), &err) == sizeof(data));
        
        writer.close();
    }
}
#include <catch/catch.hpp>
#include <kulma/io/file.h>
#include <kulma/macros.h>
#include <kulma/os.h>
#include <string.h>

const static char* s_filePath = "unit_test_file_cpp_test_file";
const static char s_data[] = "this is test string";
const static char s_append[] = " hello world";
const static char s_appended[] = "this is test string hello world";
TEST_CASE("File", "[file]")
{
    

    SECTION("Opening file for write")
    {
        kulma::touch(s_filePath);

        kulma::Error err;

        kulma::FileWriter writer;
        REQUIRE(writer.open(s_filePath, false, &err));

        REQUIRE(writer.write(s_data, sizeof(s_data), &err) == sizeof(s_data));
        writer.close();
    }

    SECTION("Opening file for appended write")
    {
        kulma::Error err;

        kulma::FileWriter writer;
        REQUIRE(writer.open(s_filePath, true, &err));

        REQUIRE(writer.write(s_append, sizeof(s_append), &err) == sizeof(s_append));
        writer.close();
    }

    SECTION("Opening file for read")
    {
        kulma::Error err;

        kulma::FileReader reader;
        REQUIRE(reader.open(s_filePath, &err));
        char buf[sizeof(s_appended)] = { 0 };
        REQUIRE(reader.read(buf, sizeof(s_data), &err) == sizeof(s_data));
        REQUIRE(strcmp(s_data, buf) == 0);
        // get rid of null terminator
        REQUIRE(reader.read(buf + sizeof(s_data) - 1, sizeof(s_append), &err) == sizeof(s_append));
        REQUIRE(strcmp(s_appended, buf) == 0);
        
        reader.close();

        kulma::remove_file(s_filePath);
    }

    
}

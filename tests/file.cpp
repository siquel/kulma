#include <catch/catch.hpp>
#include <kulma/io/file.h>
#include <kulma/macros.h>
#include <kulma/os.h>

const static char* s_filePath = "unit_test_file_cpp_test_file";

TEST_CASE("File", "[file]")
{

    SECTION("Opening file for read/write")
    {
        char data[] = "this is test string";
        kulma::touch(s_filePath);

        {
            kulma::Error err;

            kulma::FileWriter writer;
            REQUIRE(writer.open(s_filePath, false, &err));

            REQUIRE(writer.write(data, sizeof(data), &err) == sizeof(data));
            writer.close();
        }

        {
            kulma::Error err;

            kulma::FileReader reader;
            REQUIRE(reader.open(s_filePath, &err));
            char buf[sizeof(data)] = { 0 };
            REQUIRE(reader.read(buf, sizeof(data), &err) == sizeof(buf));
            reader.close();

            REQUIRE(strcmp(data, buf) == 0);
        }
        kulma::remove_file(s_filePath);
    }
}
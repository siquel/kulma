#include <catch/catch.hpp>
#include <kulma/os.h>
#include <string.h>

static const char* s_directoryPath = "test";
static const char* s_filePath = "kulma_test_file";

TEST_CASE("OS", "[os]") {
    SECTION("directory") {
        if (!kulma::exists(s_directoryPath))
        {
            kulma::mkdir(s_directoryPath);
        }
        REQUIRE(kulma::exists(s_directoryPath) == true);
        kulma::rmdir(s_directoryPath);
        REQUIRE(kulma::exists(s_directoryPath) == false);
        kulma::mkdir(s_directoryPath);
        REQUIRE(kulma::exists(s_directoryPath) == true);
    }

    SECTION("file") {
        // touch doesn't fail if the file exists, oh yes on linux it will..
        if (!kulma::exists(s_filePath)) 
        {
            kulma::touch(s_filePath);
        }
        REQUIRE(kulma::exists(s_filePath) == true);
        // remove it 
        kulma::remove_file(s_filePath);
        REQUIRE(kulma::exists(s_filePath) == false);
        // create it again
        kulma::touch(s_filePath);
        REQUIRE(kulma::exists(s_filePath) == true);
    }

    SECTION("stat") {
        kulma::FileInfo fi;
        kulma::stat(s_filePath, fi);
        REQUIRE(fi.m_type == kulma::FileInfo::File);
        REQUIRE(fi.m_size == 0);
    }

    SECTION("cwd") {
        const uint32_t MaxPath = 4096;
        char buffer[MaxPath] = { 0 };
        char buffer2[MaxPath] = { 0 };

        char* cwd = NULL;
        cwd = kulma::current_working_directory(buffer, MaxPath);
        REQUIRE(cwd != NULL);

        printf("Current working directory = %s\n", cwd);
        printf("Changing working dir to ..\n");
        int32_t err = kulma::set_working_directory("..");
        REQUIRE(err == 0);
        err =  kulma::set_working_directory("this_doesnt_not_probably_exist");
        REQUIRE(err == -1);
        char* cwd2 = kulma::current_working_directory(buffer2, MaxPath);
        printf("Current working directory = %s\n", cwd2);
        REQUIRE(strlen(cwd2) < strlen(cwd));
    }
}

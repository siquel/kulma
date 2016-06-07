#include "kulma/io/file.h"

namespace kulma
{
    FileReader::FileReader()
    {

    }

    FileReader::~FileReader()
    {

    }

    int32_t FileReader::read(void * p_data, int32_t p_size, Error * p_err)
    {
        KULMA_UNUSED(p_data, p_size, p_err);
        return int32_t();
    }

    int64_t FileReader::seek(int64_t p_offset, Whence::Enum p_whence)
    {
        KULMA_UNUSED(p_offset, p_whence);
        return int64_t();
    }

    bool FileReader::open(const char * p_filePath, Error * p_err)
    {
        KULMA_UNUSED(p_filePath, p_err);
        return false;
    }

    void FileReader::close()
    {
    }
}
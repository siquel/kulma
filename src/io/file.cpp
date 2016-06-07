#include "kulma/io/file.h"

#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#elif KULMA_PLATFORM_LINUX

#endif

namespace kulma
{
    class DiskFile : public IFile
    {
    public:
        DiskFile()
#if KULMA_PLATFORM_WINDOWS
            : m_file(INVALID_HANDLE_VALUE)
#elif KULMA_PLATFORM_LINUX
            : m_file(NULL)
#endif
        {

        }

        bool open(const char* p_path, FileOpenMode::Enum p_mode) override
        {
#if KULMA_PLATFORM_WINDOWS
            m_file = CreateFile(
                p_path,
                (p_mode == FileOpenMode::Read) ? GENERIC_READ : GENERIC_WRITE,
                0,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
                );
            KULMA_ASSERT(m_file != INVALID_HANDLE_VALUE,
                "CreateFile failed, GetLastError = %d, path = %s",
                GetLastError(),
                p_path);
            return m_file != INVALID_HANDLE_VALUE;
#elif KULMA_PLATFORM_LINUX
            m_file = fopen(p_path, (p_mode == FileOpenMode::Read) ? "rb" : "wb");
            KULMA_ASSERT(m_file != NULL,
                "fopen: errno = %d, path = %s",
                errno,
                p_path);
            return m_file != NULL;
#endif
        }

    private:
#if KULMA_PLATFORM_WINDOWS
        HANDLE m_file;
#elif KULMA_PLATFORM_LINUX
        FILE* m_file;
#endif
    };

    FileReader::FileReader()
        : m_file(NULL)
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
        KULMA_ASSERT(p_err != NULL, "Reading interface error handler can't be null");
        KULMA_ASSERT(m_file != NULL, "File can't be null");

        if (!m_file->open(p_filePath, FileOpenMode::Read))
        {
            KULMA_ERROR_SET(p_err, KULMA_ERROR_IO_OPEN, "FileReader: Failed to open file");
            return false;
        }
        
        return true;
    }

    void FileReader::close()
    {
    }
}
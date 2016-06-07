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
    FileReader::FileReader()
#if KULMA_PLATFORM_WINDOWS
        : m_file(INVALID_HANDLE_VALUE)
#elif KULMA_PLATFORM_LINUX
        : m_file(NULL)
#endif
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
#if KULMA_PLATFORM_WINDOWS
        LARGE_INTEGER li;

        li.QuadPart = p_offset;

        li.LowPart = ::SetFilePointer(m_file,
            li.LowPart,
            &li.HighPart,
            p_whence);

        KULMA_ASSERT(li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR,
            "SetFilePointer failed, GetLastError = %d",
            GetLastError()
            );

        return li.QuadPart;

#elif KULMA_PLATFORM_LINUX
        fseeko64(m_file, p_offset, p_whence);
        return ftello64(m_file);
#endif
    }

    bool FileReader::open(const char * p_filePath, Error * p_err)
    {
        KULMA_ASSERT(p_err != NULL, "Reading interface error handler can't be null");

#if KULMA_PLATFORM_WINDOWS
        m_file = ::CreateFile(
            p_filePath,
            GENERIC_READ,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

        if (m_file == INVALID_HANDLE_VALUE)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_OPEN,
                "FileReader: failed to open file");
            return false;
        }
#elif KULMA_PLATFORM_LINUX
        m_file = ::fopen(p_filePath, "rb");

        if (m_file == NULL)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_OPEN,
                "FileReader: failed to open file");
            return false;
        }
#endif
        
        return true;
    }

    void FileReader::close()
    {
#if KULMA_PLATFORM_WINDOWS
        if (m_file != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(m_file);
            m_file = INVALID_HANDLE_VALUE;
        }
#elif KULMA_PLATFORM_LINUX
        if (m_file != NULL)
        {
            ::fclose(m_file);
            m_file = NULL;
        }
#endif
    }
}
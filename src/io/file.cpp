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
        KULMA_ASSERT(p_err != NULL, "Reading interface error handler can't be null");

#if KULMA_PLATFORM_WINDOWS
        DWORD read = 0;
        if (::ReadFile(m_file, p_data, p_size, &read, NULL) == FALSE)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_READ,
                "FileReader: read failed");
        }
        else if ((int32_t)read != p_size)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_READ,
                "FileReader: read failed");
        }
        return (int32_t)read;
#elif KULMA_PLATFORM_LINUX
        int32_t size = (int32_t)fread(p_data, 1, p_size, m_file);
        if (size != p_size)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_READ,
                "FileReader: read failed");
            return size >= 0 ? size : 0;
        }
        return size;
#endif
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
        m_file = ::CreateFileA(
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

    FileWriter::FileWriter()
#if KULMA_PLATFORM_WINDOWS
        : m_file(INVALID_HANDLE_VALUE)
#elif KULMA_PLATFORM_LINUX
        : m_file(NULL)
#endif
    {

    }

    FileWriter::~FileWriter()
    {

    }

    int32_t FileWriter::write(const void* p_data, int32_t p_size, Error* p_err)
    {
        KULMA_UNUSED(p_data, p_size, p_err);
        return 0;
    }

    int64_t FileWriter::seek(int64_t p_offset, Whence::Enum p_whence)
    {
        KULMA_UNUSED(p_offset, p_whence);
        return 0;
    }

    bool FileWriter::open(const char* p_filePath, bool p_append, Error* p_err)
    {
#if KULMA_PLATFORM_WINDOWS
        m_file = ::CreateFileA(p_filePath,
            GENERIC_WRITE,
            0,
            NULL,
            p_append ? OPEN_ALWAYS : CREATE_ALWAYS, // truncate if necessary
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

        if (m_file == INVALID_HANDLE_VALUE)
        {
            KULMA_ERROR_SET(p_err,
                KULMA_ERROR_IO_OPEN,
                "FileWriter: failed to open file");
            return false;
        }

        if (p_append) 
        {
            // move the cursor to the eof
            DWORD err = ::SetFilePointer(m_file, 0, NULL, FILE_END);
            if (err == INVALID_SET_FILE_POINTER)
            {
                KULMA_ERROR_SET(p_err,
                    KULMA_ERROR_IO_OPEN,
                    "FileWriter: faled to open file, SetFilePointer failed");
                return false;
            }
        }
#elif KULMA_PLATFORM_LINUX
#endif

        return true;
    }

    void FileWriter::close()
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
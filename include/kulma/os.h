#pragma once

#include <stdint.h> // uint32_t
#include <sys/stat.h> // stat
#include <kulma/string.h>
#include <kulma/platform.h>
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#elif KULMA_PLATFORM_LINUX
#   include <unistd.h> // usleep
#   include <sys/time.h> // gettimeofday
#   include <dlfcn.h> //dlopen, dlclose, dlsym
#endif

#if KULMA_COMPILER_MSVC
#   include <direct.h> // _getcwd
#endif

namespace kulma
{
    inline void sleep(uint32_t p_millisecs)
    {
#if KULMA_PLATFORM_WINDOWS
        Sleep(p_millisecs);
#elif KULMA_PLATFORM_LINUX
        usleep(p_millisecs * 1000);
#endif
    }

    inline int64_t clockFrequency()
    {
#if KULMA_PLATFORM_WINDOWS
        LARGE_INTEGER li;
        QueryPerformanceFrequency(&li);
        return li.QuadPart;
#elif KULMA_PLATFORM_LINUX
        return INT64_C(1000000);
#endif
    }

    inline int64_t clocktime()
    {
#if KULMA_PLATFORM_WINDOWS
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return li.QuadPart;
#elif KULMA_PLATFORM_LINUX
        struct timeval now;
        gettimeofday(&now, 0);
        return now.tv_sec*INT64_C(1000000) + now.tv_usec;
#endif
    }

    inline void* dlopen(const char* p_path)
    {
#if KULMA_PLATFORM_WINDOWS
        return (void*)::LoadLibraryA(p_path);
#elif KULMA_PLATFORM_LINUX
        return ::dlopen(p_path, RTLD_LAZY);
#endif
    }

    inline void dlclose(void* p_handle)
    {
#if KULMA_PLATFORM_WINDOWS
        ::FreeLibrary((HMODULE)p_handle);
#elif KULMA_PLATFORM_LINUX
        ::dlclose(p_handle);
#endif
    }

    inline void* dlsym(void* p_handle, const char* p_symbol)
    {
#if KULMA_PLATFORM_WINDOWS
        return (void*)::GetProcAddress((HMODULE)p_handle, p_symbol);
#elif KULMA_PLATFORM_LINUX
        return ::dlsym(p_handle, p_symbol);
#endif
    }

    inline char* current_working_directory(char* p_buffer, uint32_t p_size)
    {
#if KULMA_PLATFORM_WINDOWS
        // getcwd is deprecated
        return ::_getcwd(p_buffer, p_size);
#elif KULMA_PLATFORM_LINUX
        return ::getcwd(p_buffer, p_size);
#endif
    }

    inline int32_t set_working_directory(const char* p_path)
    {
#if KULMA_PLATFORM_WINDOWS
        return ::_chdir(p_path);
#elif KULMA_PLATFORM_LINUX
        return ::chdir(p_path);
#endif
    }

    inline void setenv(const char* p_name, const char* p_value)
    {
#if KULMA_PLATFORM_WINDOWS
        // we could use putenv so getenv should work?
        //kulma::snprintf("%s=%s", )
        ::SetEnvironmentVariableA(p_name, p_value);
#elif KULMA_PLATFORM_LINUX
        ::setenv(p_name, p_value, 1);
#endif
    }

    inline void unsetenv(const char* p_name)
    {
#if KULMA_PLATFORM_WINDOWS
        ::SetEnvironmentVariableA(p_name, NULL);
#elif KULMA_PLATFORM_LINUX
        ::unsetenv(p_name);
#endif
    }

    inline const char* getenv(const char* p_name)
    {
#if KULMA_PLATFORM_WINDOWS
        // getenv can not retrieve values set by ::SetEnvironmentVariableA
        return ::getenv(p_name);
#   if 0
        KULMA_ASSERT(p_buffer != NULL, "getenv: p_buffer can't be null!");
        KULMA_ASSERT(p_size != 0, "getenv: p_size can't be 0!");
        uint32_t len = GetEnvironmentVariableA(p_name, p_buffer, p_size);
        if (len == 0)
        {
            DWORD err = GetLastError();
            if (err == ERROR_ENVVAR_NOT_FOUND)
            {
                return NULL;
            }
            else
            {
                KULMA_ASSERT(0, "Unkown error: GetLastError() %d", err);
            }
        }
        return p_buffer;
#   endif
#elif KULMA_PLATFORM_LINUX
        return ::getenv(p_name);
#endif

    }

    struct FileInfo
    {
        enum Enum 
        {
            File,
            Directory,
            Count
        };

        uint64_t m_size;
        Enum m_type;
    };

    inline bool stat(const char* p_path, FileInfo& p_fileInfo)
    {
        p_fileInfo.m_size = 0;
        p_fileInfo.m_type = FileInfo::Count;
#if KULMA_COMPILER_MSVC
        struct ::_stat64 st;
        int32_t result = ::_stat64(p_path, &st);
        if (result != 0) return false;
        if ((st.st_mode & _S_IFREG) != 0)
        {
            p_fileInfo.m_type = FileInfo::File;
        }
        else if ((st.st_mode & _S_IFDIR) != 0)
        {
            p_fileInfo.m_type = FileInfo::Directory;
        }
#else
        struct ::stat st;
        int32_t result = ::stat(p_path, &st);
        if (result != 0) return false;
        if ((st.st_mode & S_IFREG) != 0) 
        {
            p_fileInfo.m_type = FileInfo::File;
        }
        else if ((st.st_mode & S_IFDIR) != 0)
        {
            p_fileInfo.m_type = FileInfo::Directory;
        }
#endif
        p_fileInfo.m_size = st.st_size;
        return true;
    }

}

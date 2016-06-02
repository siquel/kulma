#pragma once

#include <stdint.h> // uint32_t

#include <kulma/platform.h>
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#elif KULMA_PLATFORM_LINUX
#   include <unistd.h> // usleep
#endif

#if KULMA_COMPILER_MSVC
#   include <direct.h> // _getcwd
#endif

namespace kulma
{
    namespace os
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
            retrun now.tv_sec*INT64_C(1000000) + now.tv_usec;
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
    }

}

#pragma once

#include <stdint.h> // uint32_t

#include <kulma/platform.h>
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#endif

namespace kulma
{
    namespace os
    {
        void sleep(uint32_t p_millisecs)
        {
#if KULMA_PLATFORM_WINDOWS
            Sleep(p_millisecs);
#elif KULMA_PLATFORM_LINUX
            usleep(p_millisecs * 1000);
#endif
        }
    }

}
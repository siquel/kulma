#include "kulma/debug.h"
#include "kulma/macros.h"
#include <stdarg.h> // va_start
#include <stdio.h>
#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS
#   include <malloc.h> // alloca
#elif KULMA_PLATFORM_LINUX
#   include <alloca.h> // alloca
#endif
#include <string.h> // memcpy
#include "kulma/string.h"

namespace kulma
{

    void printVargs(const char* p_path, uint16_t p_line, const char* p_format, va_list p_args)
    {
        char temp[2048];
        char* out = temp;
        int32_t len = snprintf(out, sizeof(temp), "%s (%d): ", p_path, p_line);
        int32_t total = len + kulma::vsnprintf(out + len, sizeof(temp) - len, p_format, p_args);
        if ((int32_t)sizeof(temp) < total)
        {
            out = (char*)alloca(total + 1u);
            memcpy(out, temp, len);
            kulma::vsnprintf(out + len, total - len, p_format, p_args);
        }
        out[total] = '\0';
        fprintf(stderr, "%s", out);
    }

    void trace(const char* p_path, uint16_t p_line, const char* p_format, ...)
    {
        va_list args;
        va_start(args, p_format);
        printVargs(p_path, p_line, p_format, args);
        va_end(args);
    }

    void fatal(const char* p_path, uint16_t p_line, const char* p_format, ...)
    {
        KULMA_UNUSED(p_path, p_line, p_format);
        debugBreak();
    }

}

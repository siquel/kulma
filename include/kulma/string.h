#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

namespace kulma
{
    inline int32_t vsnprintf(char* p_str, size_t p_count, const char* p_format, va_list p_args)
    {
#if KULMA_COMPILER_MSVC
        int32_t len = ::vsnprintf_s(p_str, p_count, size_t(-1), p_format, p_args);
        return -1 == len ? ::_vscprintf(p_format, p_args) : len;
#else
        return ::vsnprintf(p_str, p_count, p_format, p_args);
#endif
    }

    inline int32_t snprintf(char* _str, size_t _count, const char* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        int32_t len = kulma::vsnprintf(_str, _count, _format, argList);
        va_end(argList);
        return len;
    }
}

#pragma once


#include <stdint.h>
#include "platform.h"

namespace kulma 
{
    inline void debugBreak() 
    {
#if KULMA_COMPILER_MSVC
            __debugbreak();
#else
            // http://stackoverflow.com/questions/4326414/set-breakpoint-in-c-or-c-code-programmatically-for-gdb-on-linux
            __asm__("int $3");
#endif
    }
    void trace(const char* p_path, uint16_t p_line, const char* p_format, ...);
    void fatal(const char* p_path, uint16_t p_line, const char* p_format, ...);
}

#ifdef NDEBUG
#   define _KULMA_TRACE(_format, ...) do { } while (0)
#   define _KULMA_WARN(_condition, _format, ...) do { } while (0)
#   define _KULMA_ASSERT(_condition, _format, ...) do { KULMA_UNUSED(_condition); } while (0)
#else
#   define _KULMA_TRACE(_format, ...) do {                                                                          \
                                        kulma::trace(__FILE__, uint16_t(__LINE__), _format "\n", ##__VA_ARGS__);    \
                                      } while(0)

#   define _KULMA_WARN(_condition, _format, ...) do {                                                               \
                                                    if (!(_condition)) {                                            \
                                                       KULMA_TRACE("WARN " _format, ##__VA_ARGS__);                 \
                                                       kulma::debugBreak();                                         \
                                                    }                                                               \
                                                 } while (0)

#   define _KULMA_ASSERT(_condition, _msg, ...) do {                                                                \
                                                if (!(_condition)) {                                                \
                                 KULMA_TRACE("\nAssertion failed: %s with message " _msg "\n", #_condition, ##__VA_ARGS__);  \
                                                    kulma::fatal(__FILE__, uint16_t(__LINE__),                      \
                                            "\nAssertion failed: %s\n\t " _msg "\n", #_condition, ##__VA_ARGS__);   \
                                                    }                                                               \
                                                } while(0)
#endif



#define KULMA_TRACE _KULMA_TRACE
#define KULMA_WARN _KULMA_WARN
#define KULMA_ASSERT _KULMA_ASSERT
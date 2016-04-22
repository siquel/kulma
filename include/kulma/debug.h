#pragma once

#include <stdint.h>

namespace kulma 
{
    void debugBreak();
    void trace(const char* p_path, uint16_t p_line, const char* p_format, ...);
}

#if defined(NDEBUG)
#   define _KULMA_TRACE(_format, ...) do { } while (0)
#   define _KULMA_WARN(_condition, _format, ...) do { } while (0)
#   define _KULMA_ASSERT(_condition, _format, ...) do { } while (0)
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

#   define _KULMA_ASSERT(_condition, _format, ...) do {                                                             \
                                                   } while(0)
#endif



#define KULMA_TRACE _KULMA_TRACE
#define KULMA_WARN _KULMA_WARN
#define KULMA_ASSERT _KULMA_ASSERT
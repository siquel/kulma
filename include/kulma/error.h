#pragma once

#include <stdint.h>
#include "debug.h"

#define KULMA_ERROR_SET(ptr, result, msg)                                   \
            do {                                                            \
                KULMA_TRACE("Error %d: %s", result.m_code, "" msg);         \
                ptr->setError(result, "" msg);                              \
            } while (0)

#define KULMA_ERROR_RESULT(err, code)                                       \
            static_assert(code != 0, "Error code 0 is reserved");           \
            static const kulma::ErrorResult err = { code }

namespace kulma
{
    // just a wrapper
    struct ErrorResult
    {
        uint32_t m_code;
    };

    class Error
    {
    public:
        KULMA_NO(Error, COPY, MOVE);

        Error()
            : m_code(0)
        {

        }

        void setError(ErrorResult p_error, const char* p_msg)
        {
            // 0 means success
            KULMA_ASSERT(p_error.m_code != 0, "Invalid error code passed to setError, 0 is reserved error code!");

            if (!isOk()) return;

            m_code = p_error.m_code;
            m_msg = p_msg;
        }

        ErrorResult get() const
        {
            return ErrorResult{ m_code };
        }

        bool isOk() const
        {
            return m_code == 0;
        }

        bool operator==(ErrorResult p_rhs) const 
        {
            return m_code == p_rhs.m_code;
        }
    private:
        const char* m_msg; ///< Error message
        uint32_t m_code; ///< FOURCC code
    };

    class ScopedError
    {
    public:
        ScopedError(Error* p_err)
            : m_error(p_err)
        {
            KULMA_ASSERT(p_err != nullptr, "p_err can not be null");
        }

        ~ScopedError()
        {
            KULMA_ASSERT(m_error->isOk(), "Error: %d", m_error->get().m_code);
        }
    private:
        Error* m_error;
    };
}
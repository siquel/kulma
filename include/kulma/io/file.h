#pragma once
#include "../macros.h"
#include <stdint.h>

namespace kulma
{
    struct FileOpenMode
    {
        enum Enum
        {
            Open,
            Write,
            Truncate
        };
    };

    struct KULMA_NO_VTABLE IFile 
    {
        virtual ~IFile() = 0;

        virtual void close() = 0;

        /// \brief 
        virtual int32_t read(void* p_data, int32_t p_size) = 0;
    };

}


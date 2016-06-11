#pragma once

#include <stdint.h>

namespace kulma
{
    struct IAllocator;
    namespace memory_globals
    {
        void init(uint32_t p_buffer_size = 4 * 1024 * 1024);
        void shutdown();
    }

    IAllocator& default_allocator();

    IAllocator& default_scratch_allocator();
}
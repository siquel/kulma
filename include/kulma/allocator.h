#pragma once

#include <stdint.h> // uintptr_t
#include <stddef.h> // ptrdiff_t

#include "kulma/macros.h"

namespace kulma
{
    struct KULMA_NO_VTABLE IAllocator
    {
        virtual ~IAllocator() = 0;
        virtual void* allocate(size_t p_size, size_t p_alignment) = 0;
        virtual void deallocate(size_t p_size) = 0;
    };

    inline IAllocator::~IAllocator() {}

    inline ptrdiff_t get_align_adjustment(const void* p_address, size_t p_alignment)
    {
        uintptr_t misalignment = ((uintptr_t)p_address & (p_alignment - 1));
        ptrdiff_t adjustment = p_alignment - misalignment;

        if (adjustment == (ptrdiff_t)p_alignment)
            return 0;

        return adjustment;
    }

    struct LinearAllocator : public IAllocator
    {
        LinearAllocator(size_t p_size, void* p_start_pos);
        ~LinearAllocator();

        void* allocate(size_t p_size, size_t p_alignment = 4);
        void deallocate(size_t p_size);
        void clear();

    private:
        KULMA_NO(LinearAllocator, COPY, MOVE);

        void* m_current_pos;
        void* m_start_pos;
        size_t m_size;
        size_t m_memory_used;
        size_t m_allocs;
    };
}

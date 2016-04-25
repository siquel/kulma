#include "kulma/allocator.h"
#include "kulma/debug.h"

#include <assert.h> // assert

namespace kulma
{
    LinearAllocator::LinearAllocator(size_t p_size, void* p_start_pos) :
        m_current_pos(p_start_pos),
        m_start_pos(p_start_pos),
        m_size(p_size),
        m_memory_used(0),
        m_allocs(0)	
    {
    }

    LinearAllocator::~LinearAllocator()
    {
        KULMA_ASSERT(m_allocs == 0, "Allocator not cleaned properly.");
    }

    void* LinearAllocator::allocate(size_t p_size, size_t p_alignment)
    {
        KULMA_ASSERT((p_alignment & (p_alignment - 1)) == 0, "Allocator alignment not power of two.");

        ptrdiff_t adjustment = get_align_adjustment(m_current_pos, p_alignment);

        if (m_memory_used + p_size + adjustment > m_size)
            return nullptr;

        uintptr_t aligned_address = (uintptr_t)m_current_pos + adjustment;

        m_current_pos = (void*)(aligned_address + p_size);
        m_memory_used += p_size + adjustment;
        ++m_allocs;

        return (void*)aligned_address;
    }

    void LinearAllocator::deallocate(size_t p_size)
    {
        KULMA_UNUSED(p_size);
    }

    void LinearAllocator::clear()
    {
        m_allocs = 0;
        m_memory_used = 0;
        m_current_pos = m_start_pos;
    }
}
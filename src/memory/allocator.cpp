#include "allocator.h"
#include <assert.h> // assert
#include <stdint.h> // uintptr_t

namespace kulma
{
    LinearAllocator::LinearAllocator(size_t size_, void* start_pos_) :
        m_buffer(nullptr),
        m_current_pos(start_pos_),
        m_start_pos(start_pos_),
        m_size(size_),
        m_allocs(0)	
    {
    }

    LinearAllocator::~LinearAllocator()
    {
        assert(m_allocs == 0);
    }

    void* LinearAllocator::allocate(size_t size_)
    {
        if ((m_memory_used + size_) > m_size) 
            return nullptr;

        void* position = m_current_pos;

        m_current_pos = (void*)((uintptr_t)m_current_pos + size_);
        m_memory_used += size_;
        ++m_allocs;

        return position;
    }

    void LinearAllocator::deallocate(size_t size_)
    {
        (void)size_;
    }

    void LinearAllocator::clear()
    {
        m_allocs = 0;
        m_memory_used = 0;
        m_current_pos = m_start_pos;
    }
}
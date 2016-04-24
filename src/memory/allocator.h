#pragma once

namespace kulma
{
    struct Allocator
    {
        virtual ~Allocator() {};
        virtual void* allocate(size_t size_) = 0;
        virtual void deallocate(size_t size_) = 0;
    };

    class LinearAllocator : public Allocator
    {
        LinearAllocator(size_t size_, void* start_pos_);
        ~LinearAllocator();

        void* allocate(size_t size_);
        void deallocate(size_t size_);
        void clear();

    private:
        void* m_buffer;
        void* m_current_pos;
        void* m_start_pos;
        size_t m_size;
        size_t m_memory_used;
        size_t m_allocs;
    };
}
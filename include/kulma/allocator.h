#pragma once

#include <stdint.h> // uintptr_t
#include <stddef.h> // ptrdiff_t

#include "kulma/macros.h"

namespace kulma
{
    /// \brief Allocator interface
    struct KULMA_NO_VTABLE IAllocator
    {
        /// \brief Destructor
        virtual ~IAllocator() = 0;
        /// \brief Allocates memory
        /// 
        /// \param p_size How much memory needed
        /// \param p_alignment How should it be aligned
        /// 
        /// \return Pointer to allocated memory or NULL if the allocation failed
        virtual void* allocate(size_t p_size, size_t p_alignment) = 0;

        /// \brief Frees an allocation previously made with allocate().
        virtual void deallocate(void* p_ptr) = 0;
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
        /// \brief Create linear allocator with specified size and pre-allocated memory
        ///
        /// \param p_size Size in bytes
        /// \param p_start_pos Pointer to pre-allocated buffer
        /// \note You must free the buffer yourself
        LinearAllocator(size_t p_size, void* p_start_pos);

        /// \brief Default destructor
        ~LinearAllocator();

        /// \copydoc IAllocator::allocate()
        void* allocate(size_t p_size, size_t p_alignment = 4) override;

        /// \brief Deallocate is NOP for linear allocator
        void deallocate(void* p_ptr) override;
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

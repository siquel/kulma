#pragma once

#include <stdint.h> // uintptr_t
#include <stddef.h> // ptrdiff_t

#include "kulma/macros.h"
#include "kulma/debug.h"
#include "kulma/memory/memory.h"

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
        virtual void* allocate(size_t p_size, size_t p_alignment = 4) = 0;

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

    inline void* align_top(void* p_ptr, uint32_t p_align)
    {
        KULMA_ASSERT(p_align >= 1, "Alignment must be > 1");
        KULMA_ASSERT(p_align % 2 == 0 || p_align == 1, "Alignment must be a power of two");

        uintptr_t ptr = (uintptr_t)p_ptr;
        const uint32_t mod = ptr % p_align;

        if (mod)
            ptr += p_align - mod;

        return (void*)ptr;
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
        virtual ~LinearAllocator() override;

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

    /*
    * Copyright (C) 2012 Bitsquid AB
    * License: https://bitbucket.org/bitsquid/foundation/src/default/LICENCSE
    */

    template <int BUFFER_SIZE>
    class TempAllocator : public IAllocator
    {
    public:
        ///Creates a new temporary allocator using the specified backing allocator.
        TempAllocator(IAllocator &backing = default_scratch_allocator());

        virtual ~TempAllocator() override;

        virtual void* allocate(uint32_t size, uint32_t align = 4) override;

        /// Deallocation is a NOP for the TempAllocator. The memory is automatically
        /// deallocated when the TempAllocator is destroyed.
        virtual void deallocate(void *) override {}
    private:
        char _buffer[BUFFER_SIZE];	//< Local stack buffer for allocations.
        IAllocator &_backing;		//< Backing allocator if local memory is exhausted.
        char *_start;				//< Start of current allocation region
        char *_p;					//< Current allocation pointer.
        char *_end;					//< End of current allocation region
        unsigned _chunk_size;		//< Chunks to allocate from backing allocator

    };

    typedef TempAllocator<64> TempAllocator64;
    typedef TempAllocator<128> TempAllocator128;
    typedef TempAllocator<256> TempAllocator256;
    typedef TempAllocator<512> TempAllocator512;
    typedef TempAllocator<1024> TempAllocator1024;
    typedef TempAllocator<2048> TempAllocator2048;
    typedef TempAllocator<4096> TempAllocator4096;

    template <int BUFFER_SIZE>
	TempAllocator<BUFFER_SIZE>::TempAllocator(IAllocator &backing) : _backing(backing), _chunk_size(4*1024)
	{
		_p = _start = _buffer;
		_end = _start + BUFFER_SIZE;
		*(void **)_start = 0;
		_p += sizeof(void *);
	}

	template <int BUFFER_SIZE>
	TempAllocator<BUFFER_SIZE>::~TempAllocator()
	{
		char* start = _buffer;
		void *p = *(void **)start;
		while (p) {
			void *next = *(void **)p;
			_backing.deallocate(p);
			p = next;
		}
	}

	template <int BUFFER_SIZE>
    void *TempAllocator<BUFFER_SIZE>::allocate(uint32_t size, uint32_t align)
    {
        _p = (char *)align_top(_p, align);
        if ((int)size > _end - _p) {
            uint32_t to_allocate = sizeof(void *) + size + align;
            if (to_allocate < _chunk_size)
                to_allocate = _chunk_size;
            _chunk_size *= 2;
            void *p = _backing.allocate(to_allocate);
            *(void **)_start = p;
            _p = _start = (char *)p;
            _end = _start + to_allocate;
            *(void **)_start = 0;
            _p += sizeof(void *);
            _p = (char *)align_top(_p, align);
        }
        void *result = _p;
        _p += size;
        return result;
    }
}

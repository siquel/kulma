#include "kulma/allocator.h"
#include "kulma/thread/mutex.h"
#include <new>

namespace kulma
{
    // Copyright (C) 2012 Bitsquid AB
    // License: https://bitbucket.org/bitsquid/foundation/src/default/LICENCSE
    //
    // An allocator used to allocate temporary "scratch" memory. The allocator
    // uses a fixed size ring buffer to services the requests.
    //
    // Memory is always always allocated linearly. An allocation pointer is
    // advanced through the buffer as memory is allocated and wraps around at
    // the end of the buffer. Similarly, a free pointer is advanced as memory
    // is freed.
    //
    // It is important that the scratch allocator is only used for short-lived
    // memory allocations. A long lived allocator will lock the "free" pointer
    // and prevent the "allocate" pointer from proceeding past it, which means
    // the ring buffer can't be used.
    //
    // If the ring buffer is exhausted, the scratch allocator will use its backing
    // allocator to allocate memory instead.
    // Header stored at the beginning of a memory allocation to indicate the
    // size of the allocated data.
    struct Header
    {
        uint32_t size;
    };

    // If we need to align the memory allocation we pad the header with this
    // value after storing the size. That way we can
    const uint32_t HEADER_PAD_VALUE = 0xffffffffu;

    // Given a pointer to the header, returns a pointer to the data that follows it.
    inline void *data_pointer(Header* header, uint32_t align) {
        void *p = header + 1;
        return align_top(p, align);
    }

    // Given a pointer to the data, returns a pointer to the header before it.
    inline Header*header(const void* data)
    {
        uint32_t *p = (uint32_t*)data;
        while (p[-1] == HEADER_PAD_VALUE)
            --p;
        return (Header*)p - 1;
    }

    // Stores the size in the header and pads with HEADER_PAD_VALUE up to the
    // data pointer.
    inline void fill(Header*header, void *data, uint32_t size)
    {
        header->size = size;
        uint32_t *p = (uint32_t*)(header + 1);
        while (p < data)
            *p++ = HEADER_PAD_VALUE;
    }

    inline uint32_t actual_allocation_size(uint32_t size, uint32_t align)
    {
        return size + align + sizeof(Header);
    }

    inline void pad(Header* header, void* data)
    {
        uint32_t* p = (uint32_t*)(header + 1);

        while (p != data)
        {
            *p = HEADER_PAD_VALUE;
            p++;
        }
    }

    class ScratchAllocator : public IAllocator
    {
        Mutex _mutex;
        IAllocator &_backing;

        // Start and end of the ring buffer.
        char*_begin, *_end;

        // Pointers to where to allocate memory and where to free memory.
        char*_allocate, *_free;

    public:
        /// Creates a ScratchAllocator. The allocator will use the backing
        /// allocator to create the ring buffer and to service any requests
        /// that don't fit in the ring buffer.
        ///
        /// size specifies the size of the ring buffer.
        ScratchAllocator(IAllocator &backing, uint32_t size) : _backing(backing)
        {
            _begin = (char*)_backing.allocate(size);
            _end = _begin + size;
            _allocate = _begin;
            _free = _begin;
        }

        ~ScratchAllocator()
        {
            KULMA_ASSERT(_free == _allocate, "Memory leak");
            _backing.deallocate(_begin);
        }

        bool in_use(void *p)
        {
            if (_free == _allocate)
                return false;
            if (_allocate > _free)
                return p >= _free && p < _allocate;
            return p >= _free || p < _allocate;
        }

        void *allocate(uint32_t size, uint32_t align)
        {
            ScopedMutex sm(_mutex);

            KULMA_ASSERT(align % 4 == 0, "Must be 4-byte aligned");
            size = ((size + 3) / 4) * 4;

            char* p = _allocate;
            Header* h = (Header*)p;
            char* data = (char*)data_pointer(h, align);
            p = data + size;

            // Reached the end of the buffer, wrap around to the beginning.
            if (p > _end) {
                h->size = uint32_t(_end - (char*)h) | 0x80000000u;

                p = _begin;
                h = (Header*)p;
                data = (char*)data_pointer(h, align);
                p = data + size;
            }

            // If the buffer is exhausted use the backing allocator instead.
            if (in_use(p))
                return _backing.allocate(size, align);

            fill(h, data, uint32_t(p - (char*)h));
            _allocate = p;
            return data;
        }

        void deallocate(void *p)
        {
            ScopedMutex sm(_mutex);

            if (!p)
                return;

            if (p < _begin || p >= _end) {
                _backing.deallocate(p);
                return;
            }

            // Mark this slot as free
            Header*h = header(p);
            KULMA_ASSERT((h->size & 0x80000000u) == 0, "Not free");
            h->size = h->size | 0x80000000u;

            // Advance the free pointer past all free slots.
            while (_free != _allocate) {
                Header* hh = (Header*)_free;
                if ((hh->size & 0x80000000u) == 0)
                    break;

                _free += hh->size & 0x7fffffffu;
                if (_free == _end)
                    _free = _begin;
            }
        }

        uint32_t allocated_size(const void *p)
        {
            ScopedMutex sm(_mutex);
            Header* h = header(p);
            return h->size - uint32_t((char*)p - (char*)h);
        }

        uint32_t total_allocated()
        {
            ScopedMutex sm(_mutex);
            return uint32_t(_end - _begin);
        }
    };

    /// An allocator that uses the default system malloc(). Allocations are
    /// padded so that we can store the size of each allocation and align them
    /// to the desired alignment.
    ///
    /// (Note: An OS-specific allocator that can do alignment and tracks size
    /// does need this padding and can thus be more efficient than the
    /// MallocAllocator.)
    class MallocAllocator : public IAllocator
    {
        uint32_t _total_allocated;

        // Returns the size to allocate from malloc() for a given size and align.		
        static inline uint32_t size_with_padding(uint32_t size, uint32_t align) {
            return size + align + sizeof(Header);
        }

    public:
        MallocAllocator() : _total_allocated(0) {}

        ~MallocAllocator() {
            // Check that we don't have any memory leaks when allocator is
            // destroyed.
            KULMA_ASSERT(_total_allocated == 0, "Memory leak");
        }

        virtual void *allocate(uint32_t size, uint32_t align) override {
            const uint32_t ts = size_with_padding(size, align);
            Header *h = (Header *)malloc(ts);
            void *p = data_pointer(h, align);
            fill(h, p, ts);
            _total_allocated += ts;
            return p;
        }

        virtual void deallocate(void *p) override {
            if (!p)
                return;

            Header *h = header(p);
            _total_allocated -= h->size;
            free(h);
        }

        uint32_t allocated_size(void *p) {
            return header(p)->size;
        }

        uint32_t total_allocated() {
            return _total_allocated;
        }
    };

    using DefaultMemoryAllocator = MallocAllocator;
    using DefaultScratchAllocator = ScratchAllocator;

    struct MemoryGlobals 
    {
        static const int AllocatorMemory = sizeof(DefaultMemoryAllocator) + sizeof(DefaultScratchAllocator);
        char m_buffer[AllocatorMemory];
        DefaultMemoryAllocator* m_defaultAllocator;
        DefaultScratchAllocator* m_defaultScratchAllocator;
    };


    static MemoryGlobals s_memoryGlobals;


    namespace memory_globals
    {
        

        void init(uint32_t _bufferSize /* = 4 * 1024 * 1024 */) {
            char* mem = s_memoryGlobals.m_buffer;
            s_memoryGlobals.m_defaultAllocator = new (mem) DefaultMemoryAllocator;
            mem += sizeof(DefaultMemoryAllocator);
            s_memoryGlobals.m_defaultScratchAllocator = new (mem) DefaultScratchAllocator(*s_memoryGlobals.m_defaultAllocator, _bufferSize);
        }

        void shutdown() {
            s_memoryGlobals.m_defaultScratchAllocator->~DefaultScratchAllocator();
            s_memoryGlobals.m_defaultAllocator->~DefaultMemoryAllocator();
        }
    }

    IAllocator &default_allocator() {
        return *s_memoryGlobals.m_defaultAllocator;
    }

    IAllocator &default_scratch_allocator() {
        return *s_memoryGlobals.m_defaultScratchAllocator;
    }
}
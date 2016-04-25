#pragma once

#include "kulma/platform.h"

#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#   include <limits.h> // LONG_MAX
#endif

#include "kulma/debug.h"

namespace kulma
{
#if KULMA_PLATFORM_WINDOWS
    struct Semaphore
    {
        Semaphore(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;

        Semaphore() 
            : m_handle(INVALID_HANDLE_VALUE)
        {
            m_handle = CreateSemaphoreA(
                NULL, 
                0,        // initial count of semaphores
                LONG_MAX, // maximum count of semaphores
                NULL);    // unnamed semaphore
            KULMA_ASSERT(m_handle != INVALID_HANDLE_VALUE, "Failed to create semaphore: GetLastError = %d", GetLastError());
        }

        ~Semaphore()
        {
            BOOL err = CloseHandle(m_handle);
            KULMA_ASSERT(err != 0, "CloseHandle failed: GetLastError = %d", GetLastError());
        }
        // this shall be called when task is finished
        void post(uint32_t p_count = 1) const
        {
            BOOL err = ReleaseSemaphore(
                m_handle, // which semaphore to be released
                p_count,  // 
                NULL);    // not interested in previous count
            KULMA_ASSERT(err != 0, "ReleaseSemaphore failed: GetLastError = %d", GetLastError());
        }

        // this shall be called when we need to execute task
        bool wait(int32_t p_millisecs = -1) const
        {
            // returns true if the semaphore object was signaled, false if timeout occured
            return WAIT_OBJECT_0 == WaitForSingleObject(
                m_handle, 
                (0 > p_millisecs) ? INFINITE : (DWORD)p_millisecs);
        }

        HANDLE m_handle;
    };
#else
#   error "Semaphore not implemented"
#endif
}
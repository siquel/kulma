#pragma once

#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#endif

#include <stdint.h>
#include <kulma/debug.h>

namespace kulma
{
    struct Thread
    {
        typedef int8_t(*ThreadFunc)(void*);
        Thread(const Thread&) = delete;
        Thread(Thread&&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread& operator=(const Thread&&) = delete;

        Thread()
            : m_func(nullptr), 
              m_user_data(nullptr), 
              m_is_running(false),
#if KULMA_PLATFORM_WINDOWS
              m_handle(INVALID_HANDLE_VALUE)
#endif
        {

        }

        void start(ThreadFunc p_func, void* p_user_data = nullptr, uint32_t p_stack_size = 0u)
        {
            m_func = p_func;
            m_user_data = p_user_data;
#if KULMA_PLATFORM_WINDOWS
            m_handle = CreateThread(NULL, p_stack_size, thread_proc, this, 0, NULL);
            KULMA_ASSERT(m_handle != INVALID_HANDLE_VALUE, "CreateThread: GetLastError = %d", GetLastError());
#endif
            m_is_running = true;
        }

#if KULMA_PLATFORM_WINDOWS
        static DWORD WINAPI thread_proc(void* p)
        {
            Thread* thread = static_cast<Thread*>(p);
            int32_t result = thread->run();
            return result;
        }
#endif

        void stop()
        {
            KULMA_ASSERT(m_is_running, "Thread isn't running");
#if KULMA_PLATFORM_WINDOWS
            WaitForSingleObject(m_handle, INFINITE);
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
#endif
            m_is_running = false;
        }
      
        int32_t run()
        {
            return m_func(m_user_data);
        }

        ThreadFunc m_func;
        void* m_user_data;
        bool m_is_running;
#if KULMA_PLATFORM_WINDOWS
        HANDLE m_handle;
#endif
    };
}
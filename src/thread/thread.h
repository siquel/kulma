#pragma once

#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#elif KULMA_PLATFORM_LINUX
#   include <pthread.h>
#endif

#include <stdint.h>
#include <kulma/debug.h>
#include "semaphore.h"

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
              m_handle(INVALID_HANDLE_VALUE),
              m_thread_id(UINT32_MAX)
#elif KULMA_PLATFORM_LINUX
              m_handle(0)
#endif
        {

        }

        ~Thread()
        {
            if (m_is_running)
            {
                stop();
            }
        }

        void start(ThreadFunc p_func, void* p_user_data = nullptr, uint32_t p_stack_size = 0u)
        {
            KULMA_ASSERT(!m_is_running, "Thread is already running");

            m_func = p_func;
            m_user_data = p_user_data;
#if KULMA_PLATFORM_WINDOWS
            m_handle = CreateThread(NULL, p_stack_size, thread_proc, this, 0, NULL);
            KULMA_ASSERT(m_handle != INVALID_HANDLE_VALUE, "CreateThread: GetLastError = %d", GetLastError());
#elif KULMA_PLATFORM_LINUX

            int result;
            pthread_attr_t attr;
            result = pthread_attr_init(&attr);
            KULMA_ASSERT(result == 0, "pthread_attr_init failed %d", result);

            if (p_stack_size != 0) 
            {
                result = pthread_attr_setstacksize(&attr, p_stack_size);
                KULMA_ASSERT(result == 0, "pthread_attr_setstacksize failed %d", result);
            }

            result = pthread_create(&m_handle, &attr, thread_proc, this);
            KULMA_ASSERT(result == 0, "pthread_create failed %d", result);

            result = pthread_attr_destroy(&attr);
            KULMA_ASSERT(result == 0, "pthread_attr_destroy failed %d", result);

#endif
            m_is_running = true;
            // try to enter the semaphore gate
            m_semaphore.wait();
        }

#if KULMA_PLATFORM_WINDOWS
        static DWORD WINAPI thread_proc(void* p)
        {
            Thread* thread = static_cast<Thread*>(p);
            int32_t result = thread->run();
            return result;
        }
#elif KULMA_PLATFORM_LINUX
        static void* thread_proc(void* p)
        {
            Thread* thread = (Thread*)p;
            static int32_t result = -1;
            result = thread->run();
            return (void*)&result;
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
#if KULMA_PLATFORM_WINDOWS
            m_thread_id = ::GetCurrentThreadId();
            KULMA_TRACE("Starting thread %d", m_thread_id);
#endif
            // the thread got ownership of sem
            m_semaphore.post();
            return m_func(m_user_data);
        }

        ThreadFunc m_func;
        void* m_user_data;
        bool m_is_running;
        Semaphore m_semaphore;
#if KULMA_PLATFORM_WINDOWS
        DWORD m_thread_id;
        HANDLE m_handle;
#elif KULMA_PLATFORM_LINUX
        pthread_t m_handle;
#endif
    };
}

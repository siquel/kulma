#pragma once

#include "kulma/platform.h"

#if KULMA_PLATFORM_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#   include <limits.h> // LONG_MAX
#elif KULMA_PLATFORM_LINUX
#   include <pthread.h>
#   include <time.h> // timespec
#endif

#include "kulma/debug.h"

namespace kulma
{
    struct Semaphore
    {
        Semaphore(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;

        Semaphore() 
#if KULMA_PLATFORM_WINDOWS
            : m_handle(INVALID_HANDLE_VALUE)
#elif KULMA_PLATFORM_LINUX
            : m_count(0) 
#endif
        {
#if KULMA_PLATFORM_WINDOWS
            m_handle = CreateSemaphoreA(
                NULL,     // default security attributes
                0,        // initial count of semaphores
                LONG_MAX, // maximum count of semaphores
                NULL);    // unnamed semaphore
            KULMA_ASSERT(m_handle != INVALID_HANDLE_VALUE, "Failed to create semaphore: GetLastError = %d", GetLastError());
#elif KULMA_PLATFORM_LINUX
            int result = pthread_mutex_init(&m_mutex, NULL);
            KULMA_ASSERT(result == 0, "pthread_mutex_init %d", result);

            result = pthread_cond_init(&m_cond, NULL);
            KULMA_ASSERT(result == 0, "pthread_cond_init %d", result);
#endif
        }

        ~Semaphore()
        {
#if KULMA_PLATFORM_WINDOWS
            BOOL err = CloseHandle(m_handle);
            KULMA_ASSERT(err != 0, "CloseHandle failed: GetLastError = %d", GetLastError());
#elif KULMA_PLATFORM_LINUX
            int result = pthread_cond_destroy(&m_cond);
            KULMA_ASSERT(result == 0, "pthread_cond_destroy %d", result);

            result = pthread_mutex_destroy(&m_mutex);
            KULMA_ASSERT(result == 0, "pthread_mutex_destroy %d", result);
#endif
        }
        // this shall be called when task is finished
        void post(uint32_t p_count = 1) 
        {
#if KULMA_PLATFORM_WINDOWS
            BOOL err = ReleaseSemaphore(
                m_handle, // which semaphore to be released
                p_count,  // increase count by p_count
                NULL);    // not interested in previous count
            KULMA_ASSERT(err != 0, "ReleaseSemaphore failed: GetLastError = %d", GetLastError());
#elif KULMA_PLATFORM_LINUX
            int result = pthread_mutex_lock(&m_mutex);
            KULMA_ASSERT(result == 0, "pthread_mutex_lock %d", result);

            for (uint32_t i = 0; i < p_count; ++i) 
            {
                result = pthread_cond_signal(&m_cond);
                KULMA_ASSERT(result == 0, "pthread_cond_signal %d", result);
            }

            m_count += p_count;
            
            result = pthread_mutex_unlock(&m_mutex);
            KULMA_ASSERT(result == 0, "pthread_mutex_unlock %d", result);
#endif
        }

        // this shall be called when we need to execute task
        bool wait(int32_t p_millisecs = -1)
        {
#if KULMA_PLATFORM_WINDOWS
            // returns true if the semaphore object was signaled, false if timeout occurred
            return WAIT_OBJECT_0 == WaitForSingleObject(
                m_handle, 
                (0 > p_millisecs) ? INFINITE : (DWORD)p_millisecs);
#elif KULMA_PLATFORM_LINUX
           int result = pthread_mutex_lock(&m_mutex);
           KULMA_ASSERT(result == 0, "pthread_mutex_lock %d", result);
           timespec ts;
           clock_gettime(CLOCK_REALTIME, &ts);
           ts.tv_sec += p_millisecs/1000;
           ts.tv_nsec += (p_millisecs%1000)*1000;

           while (result == 0 && m_count >= 0)
           {
               result = pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
           }

           if (result == 0) --m_count;

           result = pthread_mutex_unlock(&m_mutex);
           KULMA_ASSERT(result == 0, "pthread_mutex_unlock %d", result);

           return result == 0;
#endif
        }

private:
#if KULMA_PLATFORM_WINDOWS
        HANDLE m_handle;
#elif KULMA_PLATFORM_LINUX
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
        int32_t m_count;
#endif
    };
}

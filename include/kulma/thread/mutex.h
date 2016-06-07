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

namespace kulma
{
    struct Mutex
    {
        KULMA_NO(Mutex, COPY, MOVE);

        Mutex()
        {
#if KULMA_PLATFORM_WINDOWS
            InitializeCriticalSection(&m_cs);
#elif KULMA_PLATFORM_LINUX
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&m_mutex, &attr);
            pthread_mutexattr_destroy(&attr);
#endif
        }

        ~Mutex()
        {
#if KULMA_PLATFORM_WINDOWS
            DeleteCriticalSection(&m_cs);
#elif KULMA_PLATFORM_LINUX
            pthread_mutex_destroy(&m_mutex);
#endif
        }

        void lock()
        {
#if KULMA_PLATFORM_WINDOWS
            EnterCriticalSection(&m_cs);
#elif KULMA_PLATFORM_LINUX
            pthread_mutex_lock(&m_mutex);
#endif
        }

        void unlock()
        {
#if KULMA_PLATFORM_WINDOWS
            LeaveCriticalSection(&m_cs);
#elif KULMA_PLATFORM_LINUX
            pthread_mutex_unlock(&m_mutex);
#endif
        }

#if KULMA_PLATFORM_WINDOWS
        CRITICAL_SECTION m_cs;
#elif KULMA_PLATFORM_LINUX
        pthread_mutex_t m_mutex;
#endif

    };

    struct ScopedMutex
    {
        KULMA_NO(ScopedMutex, COPY, MOVE);

        ScopedMutex(Mutex& p_mutex)
            : m_mutex(p_mutex)
        {
            m_mutex.lock();
        }

        ~ScopedMutex()
        {
            m_mutex.unlock();
        }

        Mutex& m_mutex;
    };
}

#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "kulma/debug.h"
#include <windows.h>
#include "kulma/platform/platform.h"
#include "kulma/platform/window.h"
#include <new>
#include "../thread/thread.h"


// TODO
#define KULMA_DEFAULT_WINDOW_WIDTH 1280
#define KULMA_DEFAULT_WINDOW_HEIGHT 720

namespace kulma
{
    static bool s_exit = false;

    int8_t thread_proc(void* p_user_data)
    {
        KULMA_UNUSED(p_user_data);
        kulma::run();
        s_exit = true;
        return EXIT_SUCCESS;
    }

    

    struct WindowsPlatform : public Platform
    {
        WindowsPlatform()
            : m_hwnd(nullptr)
        {

        }

        ~WindowsPlatform() 
        {
        }

        int run() override
        {
            HINSTANCE instance = static_cast<HINSTANCE>(GetModuleHandle(NULL));
            WNDCLASSEX wnd;
            memset(&wnd, 0, sizeof(wnd));
            wnd.cbSize = sizeof(wnd);
            wnd.style = CS_HREDRAW | CS_VREDRAW;
            wnd.lpfnWndProc = window_proc;
            wnd.hInstance = instance;
            wnd.hIcon = LoadIcon(instance, IDI_APPLICATION);
            wnd.hIconSm = LoadIcon(instance, IDI_APPLICATION);
            wnd.hCursor = LoadCursor(instance, IDC_ARROW);
            wnd.lpszClassName = "kulma";
            RegisterClassEx(&wnd);

            m_hwnd = CreateWindowA(
                "kulma", // class
                "Kulma", // name
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0, // x
                0, // y
                KULMA_DEFAULT_WINDOW_WIDTH,
                KULMA_DEFAULT_WINDOW_HEIGHT,
                0,
                NULL,
                instance,
                0
                );
            KULMA_ASSERT(m_hwnd != nullptr, "CreateWindowA: GetLastError = %d", GetLastError());

            Thread thread;
            thread.start(thread_proc, nullptr);

            MSG msg;
            msg.message = WM_NULL;

            while (!s_exit) 
            {
                while (PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE) != 0)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            KULMA_TRACE("Got away from loop");
            thread.stop();

            return EXIT_SUCCESS;
        }

        static LRESULT CALLBACK window_proc(HWND p_hwnd, UINT p_id, WPARAM p_wparam, LPARAM p_lparam) {

            switch (p_id)
            {
            case WM_DESTROY: break;
                
            case WM_QUIT:
            case WM_CLOSE:
            {
                s_exit = true;
                kulma::engine()->stop();
                return 0;
            }
            }

            return DefWindowProc(p_hwnd, p_id, p_wparam, p_lparam);
        }

        HWND m_hwnd;
    };

    static char s_platformMemory[sizeof(WindowsPlatform)];
    static WindowsPlatform* s_winPlatform = nullptr;

    struct WindowWin : public Window
    {
        WindowWin()
            : m_hwnd(nullptr),
              m_width(KULMA_DEFAULT_WINDOW_WIDTH),
              m_height(KULMA_DEFAULT_WINDOW_HEIGHT),
              m_x(0),
              m_y(0)
        {
            KULMA_ASSERT(s_winPlatform->m_hwnd != nullptr, "WindowsPlatform hasn't been initialized yet");
            m_hwnd = s_winPlatform->m_hwnd;
        }

        ~WindowWin() override
        {

        }

        void show() override
        {
            ShowWindow(m_hwnd, SW_SHOW);
        }

        void close() override
        {
            DestroyWindow(m_hwnd);
        }

        void* nwh() override
        {
            return (void*)(uintptr_t)m_hwnd;
        }

        HWND m_hwnd;
        uint16_t m_width;
        uint16_t m_height;
        uint16_t m_x;
        uint16_t m_y;
    };


    namespace window
    {
        static char memory[sizeof(WindowWin)];
        static Window* s_window = nullptr;

        Window* create()
        {
            KULMA_ASSERT(s_window == nullptr, "Window has been already created");
            s_window = new (memory)WindowWin;
            return s_window;
        }

        void destroy()
        {
            s_window->~Window();
            s_window = nullptr;
        }

    }

    namespace platform
    {
        Platform* create()
        {
            KULMA_ASSERT(s_winPlatform == nullptr, "Windows platform has already initialized");
            s_winPlatform = new (s_platformMemory)WindowsPlatform;
            return s_winPlatform;
        }

        void destroy()
        {
            s_winPlatform->~WindowsPlatform();
            s_winPlatform = nullptr;
        }
    }

}

#endif
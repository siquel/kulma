#include "kulma/platform.h"
#if KULMA_PLATFORM_LINUX

#include "kulma/debug.h"
#include "kulma/platform/platform.h"
#include "kulma/platform/window.h"
#include "kulma/platform/engine.h"
#include "../thread/thread.h"
#include <new>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define KULMA_DEFAULT_WINDOW_WIDHT 1280
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

    struct LinuxPlatform : public Platform
    {

        LinuxPlatform()
            : m_x11_display(NULL),
              m_screen_config(NULL)
        {
        }

        int entryPoint() override
        {
            {
                Status status = XInitThreads();
                KULMA_ASSERT(status != 0, "XInitThreads failed");
            }

            m_x11_display = XOpenDisplay(NULL);
            KULMA_ASSERT(m_x11_display != NULL, "XOpenDisplay failed");

            ::Window root = RootWindow(m_x11_display, DefaultScreen(m_x11_display));

            wm_delete_msg = XInternAtom(m_x11_display, "WM_DELETE_WINDOW", false);
            
            // Get screen config and save it for later use
            m_screen_config = XRRGetScreenInfo(m_x11_display, root);

            Thread thread;
            thread.start(thread_proc, NULL);

            while (!s_exit)
            {
            }

            thread.stop();

            XRRFreeScreenConfigInfo(m_screen_config);
            XCloseDisplay(m_x11_display);

            return EXIT_SUCCESS;
        }


        ::Display* m_x11_display;
        XRRScreenConfiguration* m_screen_config;
    };
    
    static char s_platformMemory[sizeof(LinuxPlatform)];
    static LinuxPlatform* s_linuxPlatform = NULL;

    struct WindowLinux : public Window
    {
        WindowLinux() 
        {
        }

        ~WindowLinux() override {}

        void show() override
        {
        }

        void close() override
        {
        }

        void* nwh() override
        {
            return NULL;
        }
    };

    namespace window 
    {
        static char s_memory[sizeof(WindowLinux)];
        static Window* s_window = NULL;

        Window* create()
        {
            KULMA_ASSERT(s_window == NULL, "Window has been already created");
            s_window = new (s_memory)WindowLinux;
            return s_window;
        }

        void destroy()
        {
            s_window->~Window();
            s_window = NULL;
        }
    }

    namespace platform
    {

        Platform* create()
        {
            KULMA_ASSERT(s_linuxPlatform == NULL, "Linux platform has been initialized already");
            s_linuxPlatform = new (s_platformMemory)LinuxPlatform;
            return s_linuxPlatform;
        }

        void destroy()
        {
            s_linuxPlatform->~LinuxPlatform();
            s_linuxPlatform = NULL;
        }
    }
}
 #endif

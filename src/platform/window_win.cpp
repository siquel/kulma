#include "kulma/platform.h"
#if KULMA_PLATFORM_WINDOWS
#include "kulma/platform/window.h"
#include <new>

namespace kulma
{
    struct WindowWin : public Window 
    {
        ~WindowWin() override
        {

        }

        void show() override
        {

        }
    };


    namespace window
    {
        static char memory[sizeof(WindowWin)];
        static Window* s_window = nullptr;

        Window* create()
        {
            s_window = new (memory)WindowWin;
            return s_window;
        }

        void destroy()
        {
            s_window->~Window();
            s_window = nullptr;
        }
    }
}

#endif
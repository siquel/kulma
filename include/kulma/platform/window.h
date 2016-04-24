#pragma once
#include "../macros.h"

namespace kulma
{
    struct KULMA_NO_VTABLE Window 
    {
        virtual ~Window() = 0;
        virtual void show() = 0;
    };
    inline Window::~Window() {}
}

namespace kulma
{
    namespace window
    {
        Window* create();

        void destroy();
    }
}

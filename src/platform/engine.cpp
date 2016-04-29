#include "kulma/platform/platform.h"
#include "kulma/debug.h"
#include <new>
#include <stdio.h>
#include "kulma/platform/window.h"
#include "kulma/platform/engine.h"

namespace kulma
{
    static bool s_exit = false;

    void Engine::stop() { s_exit = true; }
    void Engine::run()
    {
        Window* wnd = window::create();
        wnd->show();

        while (!s_exit) {}

        wnd->close();
        window::destroy();
    }

}


namespace kulma
{
    static char s_memory[sizeof(Engine)];
    static Engine* s_engine = NULL;

    void run()
    {
        s_engine = new(s_memory)Engine;
        s_engine->run();
        s_engine->~Engine();
        s_engine = NULL;
    }

    Engine* engine()
    {
        return s_engine;
    }
}

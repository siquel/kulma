#pragma once


namespace kulma
{
    struct Engine
    {
        void stop();
        void run();
    };
}

namespace kulma
{
    void run();
    Engine* engine();
}
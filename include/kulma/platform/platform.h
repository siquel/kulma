#pragma once

#include "kulma/macros.h"
namespace kulma
{
    struct Engine
    {
        void stop();
        void run();
    };

    class KULMA_NO_VTABLE Platform
    {
    public:
        virtual ~Platform() = 0;
        virtual int run() = 0;
    };
    inline Platform::~Platform() {}

    namespace platform
    {
        Platform* create();
        void destroy();
    }

}

namespace kulma
{
    void run();
    Engine* engine();
}
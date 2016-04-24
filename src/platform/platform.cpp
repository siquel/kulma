#include "kulma/platform/platform.h"
#include "kulma/debug.h"
#include <new.h>

namespace kulma
{

}


namespace kulma
{
    static char s_memory[sizeof(Platform)];
    static Platform* s_platform = nullptr;

    void run()
    {
        KULMA_ASSERT(s_platform == nullptr, "Kulma has been already initialized");
        s_platform = new (s_memory) Platform;
        s_platform->run();
        s_platform->~Platform();
        s_platform = nullptr;
    }
}
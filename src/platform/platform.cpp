#include "kulma/platform/platform.h"
#include "kulma/debug.h"
#include <new.h>

namespace kulma
{

}


namespace kulma
{
    static Platform* s_platform = nullptr;

    void run()
    {
        s_platform = platform::create();
        s_platform->run();
        platform::destroy();
        s_platform = nullptr;
    }
}
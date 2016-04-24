#include <stdio.h>
#include "kulma/macros.h"
#include "kulma/platform.h"
#include "kulma/debug.h"
#include "kulma/platform/window.h"

int main(int argc, char* argv[]) {
    KULMA_UNUSED(argc, argv);
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );
    using namespace kulma;
    Window* window = window::create();
    
    window::destroy();

    return 0; 
}

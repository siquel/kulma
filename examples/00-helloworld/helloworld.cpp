#include <stdio.h>
#include "kulma/macros.h"
#include "kulma/platform.h"
#include "kulma/debug.h"

int main(int argc, char* argv[]) {
    KULMA_UNUSED(argc, argv);
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );
    KULMA_TRACE("top kek %d gg %s", 555, "asd");
    //KULMA_WARN(1 == 0, "1 != 0");
    KULMA_ASSERT(0 == 1, "0 != 1");
    return 0; 
}

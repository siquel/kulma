#include <stdio.h>
#include "kulma/macros.h"
#include "kulma/platform.h"

int main(int argc, char* argv[]) {
    KULMA_UNUSED(argc, argv);
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );
    return 0; 
}

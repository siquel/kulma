#include <stdio.h>
#include "../src/platform.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );
    return 0; 
}

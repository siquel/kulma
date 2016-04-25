#include <stdio.h>
#include <malloc.h>
#include "kulma/macros.h"
#include "kulma/platform.h"
#include "kulma/debug.h"
#include "kulma/allocator.h"

int main(int argc, char* argv[]) {
    KULMA_UNUSED(argc, argv);
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );
    KULMA_TRACE("top kek %d gg %s", 555, "asd");
    //KULMA_WARN(1 == 0, "1 != 0");
    //KULMA_ASSERT(0 == 1, "0 != 1");

    // EBIN TEST

    const size_t size = 5;
    const size_t size_in_bytes = sizeof(int) * 10;
    void* address = malloc(size_in_bytes);

    kulma::LinearAllocator linearAllocator(size_in_bytes, address);

    int* numbers = (int*)linearAllocator.allocate(sizeof(int) * size);

    for (size_t i = 0; i < size; i++)
    {
        numbers[i] = (int)i;
        printf("%d", numbers[i]);
    }

    printf("\n");

    linearAllocator.clear();

    int* number = (int*)linearAllocator.allocate(sizeof(int));

    *number = 1337;

    for (size_t i = 0; i < size; i++)
    {
        printf("%d", numbers[i]);
    }

    linearAllocator.clear();

    free(address);

    // END OF EBIN TEST

    return 0; 
}

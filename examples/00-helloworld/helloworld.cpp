#include <stdio.h>
#include <malloc.h>
#include "kulma/macros.h"
#include "kulma/platform.h"
#include "kulma/debug.h"
#include "kulma/allocator.h"
#include <kulma/thread/thread.h>
#include <kulma/thread/mutex.h>
#include <kulma/os.h>

using namespace kulma;

static kulma::Semaphore s_sem;

struct ThreadParams
{
    int index;
};

int8_t thread_proc(void* userdata) 
{
    ThreadParams* params = (ThreadParams*)userdata;
    printf("Thread %d begins and waits for the sem\n", params->index);
    
    s_sem.wait();

    printf("Thread %d enters the sem\n", params->index);
    
    os::sleep(1000);
    
    printf("Thread %d releases the sem\n", params->index);
    s_sem.post();

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    using namespace kulma;

    KULMA_UNUSED(argc, argv);
    printf("Hello world! Running on %s, target arch %s, compiled with %s\n", 
        KULMA_PLATFORM_NAME,
        KULMA_ARCH_NAME,
        KULMA_COMPILER_NAME
    );

    const uint32_t MaxPath = 4096u;
    char buffer[MaxPath];
    //os::set_working_directory("..");
    char* cwd = os::current_working_directory(buffer, MaxPath);

    printf("current working directory is %s\n", cwd);

    const int32_t ThreadCount = 5;
    Thread entries[ThreadCount];
    ThreadParams params[ThreadCount];

    for (int32_t i = 0; i < ThreadCount; ++i)
    {
        params[i] = { i };
        entries[i].start(thread_proc, &params[i]);
    }

    os::sleep(500);
    printf("Main thread calls post(3)\n");

    s_sem.post(3);

    printf("Main thread exits\n");

    return 0;
}

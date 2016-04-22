#pragma once

#include "platform.h"

#define KULMA_TO_STRING_(_x) #_x
#define KULMA_TO_STRING(_x) KULMA_TO_STRING_(_x)

#define KULMA_CONCAT_(_x, _y) _x ## _y
#define KULMA_CONCAT(_x, _y) KULMA_CONCAT_(_x, _y)

// VS only passes the first element
#if KULMA_PLATFORM_WINDOWS
#define KULMA_VA_ARGS_PASS(...) KULMA_VA_ARGS_PASS_1_ __VA_ARGS__ KULMA_VA_ARGS_PASS_2_
#define KULMA_VA_ARGS_PASS_1_ (
#define KULMA_VA_ARGS_PASS_2_ )
#endif

#define KULMA_VA_ARGS_COUNT_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, last, ...) last

#if KULMA_PLATFORM_WINDOWS
#define KULMA_VA_ARGS_COUNT(...) KULMA_VA_ARGS_COUNT_ KULMA_VA_ARGS_PASS(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#else
#define KULMA_VA_ARGS_COUNT(...) KULMA_VA_ARGS_COUNT_(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

#define KULMA_MACRO_DISPATCHER(_func, ...) KULMA_MACRO_DISPATCHER_1_(_func, KULMA_VA_ARGS_COUNT(__VA_ARGS__) )
#define KULMA_MACRO_DISPATCHER_1_(_func, _argc) KULMA_MACRO_DISPATCHER_2_(_func, _argc)
#define KULMA_MACRO_DISPATCHER_2_(_func, _argc) KULMA_CONCAT(_func, _argc)
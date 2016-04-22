#pragma once

#include "platform.h"

#define KULMA_TO_STRING_(_x) #_x
#define KULMA_TO_STRING(_x) KULMA_TO_STRING_(_x)

#define KULMA_CONCAT_(_x, _y) _x ## _y
#define KULMA_CONCAT(_x, _y) KULMA_CONCAT_(_x, _y)

// VS only passes the first element
#if KULMA_COMPILER_MSVC
#   define KULMA_VA_ARGS_PASS(...) KULMA_VA_ARGS_PASS_1_ __VA_ARGS__ KULMA_VA_ARGS_PASS_2_
#   define KULMA_VA_ARGS_PASS_1_ (
#   define KULMA_VA_ARGS_PASS_2_ )
#endif

#define KULMA_VA_ARGS_COUNT_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, last, ...) last

#if KULMA_COMPILER_MSVC
#   define KULMA_VA_ARGS_COUNT(...) KULMA_VA_ARGS_COUNT_ KULMA_VA_ARGS_PASS(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#else
#   define KULMA_VA_ARGS_COUNT(...) KULMA_VA_ARGS_COUNT_(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

#define KULMA_MACRO_DISPATCHER(_func, ...) KULMA_MACRO_DISPATCHER_1_(_func, KULMA_VA_ARGS_COUNT(__VA_ARGS__) )
#define KULMA_MACRO_DISPATCHER_1_(_func, _argc) KULMA_MACRO_DISPATCHER_2_(_func, _argc)
#define KULMA_MACRO_DISPATCHER_2_(_func, _argc) KULMA_CONCAT(_func, _argc)

#define KULMA_UNUSED_1(_a1) do { (void)sizeof(_a1); } while(0)
#define KULMA_UNUSED_2(_a1, _a2) KULMA_UNUSED_1(_a1); KULMA_UNUSED_1(_a2)
#define KULMA_UNUSED_3(_a1, _a2, _a3) KULMA_UNUSED_2(_a1, _a2); KULMA_UNUSED_1(_a3)
#define KULMA_UNUSED_4(_a1, _a2, _a3, _a4) KULMA_UNUSED_3(_a1, _a2, _a3); KULMA_UNUSED_1(_a4)
#define KULMA_UNUSED_5(_a1, _a2, _a3, _a4, _a5) KULMA_UNUSED_4(_a1, _a2, _a3, _a4); KULMA_UNUSED_1(_a5)
#define KULMA_UNUSED_6(_a1, _a2, _a3, _a4, _a5, _a6) KULMA_UNUSED_5(_a1, _a2, _a3, _a4, _a5); KULMA_UNUSED_1(_a6)

// dispatch workaround
#if KULMA_COMPILER_MSVC
#   define KULMA_UNUSED(...) KULMA_MACRO_DISPATCHER(KULMA_UNUSED_, __VA_ARGS__) KULMA_VA_ARGS_PASS(__VA_ARGS__)
#else
#   define KULMA_UNUSED(...) KULMA_MACRO_DISPATCHER(KULMA_UNUSED_, __VA_ARGS__)(__VA_ARGS__)
#endif

#if KULMA_COMPILER_MSVC
#   define KULMA_NO_VTABLE __declspec(novtable)
#else
#   define KULMA_NO_VTABLE
#endif
#pragma once

#pragma once

#define KULMA_COMPILER_MSVC             0
#define KULMA_COMPILER_CLANG            0
#define KULMA_COMPILER_GCC              0

#define KULMA_PLATFORM_WINDOWS          0
#define KULMA_PLATFORM_LINUX            0

#define KULMA_ARCH_32BIT                0
#define KULMA_ARCH_64BIT                0

#define KULMA_CPU_LITTLE_ENDIAN         0
#define KULMA_CPU_BIG_ENDIAN            0
#define KULMA_CACHE_LINE_SIZE           64

// strings
// KULMA_PLATFORM_NAME
// KULMA_COMPILER_NAME
// KULMA_ARCH_NAME

#if defined(__clang__)
#   undef KULMA_COMPILER_CLANG
#   define KULMA_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(_MSC_VER)
#   undef KULMA_COMPILER_MSVC
#   define KULMA_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#   undef KULMA_COMPILER_GCC
#   define KULMA_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#   error "Unsupported compiler"
#endif

#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#   undef KULMA_ARCH_64BIT
#   define KULMA_ARCH_64BIT 1
#else
#   undef KULMA_ARCH_32BIT
#   define KULMA_ARCH_32BIT 1
#endif

#if defined(_WIN32) || defined(_WIN64)
#     undef KULMA_PLATFORM_WINDOWS
#     ifndef NOMINMAX
#         define NOMINMAX
#     endif
#     if !defined(WINVER) && !defined(_WIN32_WINNT)
// win 7
#         define WINWER 0x0601 
#         define _WIN32_WINNT 0x0601
#     endif // def winver && winnt
#     define KULMA_PLATFORM_WINDOWS _WIN32_WINNT
#elif defined(__linux__)
#     undef KULMA_PLATFORM_LINUX
#     define KULMA_PLATFORM_LINUX       1
#endif


// string impls

#if KULMA_ARCH_32BIT
#   define KULMA_ARCH_NAME  "32-bit"
#else
#   define KULMA_ARCH_NAME  "64-bit"
#endif

#if KULMA_PLATFORM_WINDOWS
#   define KULMA_PLATFORM_NAME      "Windows"
#elif KULMA_PLATFORM_LINUX
#   define KULMA_PLATFORM_NAME      "Linux"
#endif

#if KULMA_COMPILER_MSVC
#   if KULMA_COMPILER_MSVC >= 1900 // vs2015
#       define KULMA_COMPILER_NAME  "MSVC 14.0"
#   elif KULMA_COMPILER_MSVC >= 1800 // vs2013, if we need to compile at school..
#       define KULMA_COMPILER_NAME  "MSVC 12.0"
#   else
#       define KULMA_COMPILER_NAME  "MSVC"
#   endif
#elif KULMA_COMPILER_CLANG
#   define KULMA_COMPILER_NAME      "Clang " \
                                    KULMA_TO_STRING(__clang_major__) "." \
                                    KULMA_TO_STRING(__clang_minor__) "." \
                                    KULMA_TO_STRING(__clang_patchlevel__)
#elif KULMA_COMPILER_GCC
#   define KULMA_COMPILER_NAME      "GCC " \
                                    KULMA_TO_STRING(__GNUC__) "." \
                                    KULMA_TO_STRING(__GNUC_MINOR__) "." \
                                    KULMA_TO_STRING(__GNUC_PATCHLEVEL__) 
#endif

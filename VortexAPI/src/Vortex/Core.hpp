#pragma once

#if defined(_MSC_VER) // microsoft
    #ifdef VX_BUILD_DLL
        #define VX_API __declspec(dllexport)
    #else
        #define VX_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) // gnu compiler, gcc
    #define VX_API __attribute__((visibility("default")))
#else
    #error Vortex only supports Windows and MacOS for now.
//    #define VX_API // Most compilers export all the symbols by default. We hope for the best here.
//    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define BIT(x) (1 << x)

#if defined(_MSC_VER)
    #define VX_CORE_ASSERT(x, ...) {if(!(x)){VX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
    #define VX_ASSERT(x, ...) {if(!(x)){VX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#elif defined(__GNUC__)
#define VX_CORE_ASSERT(x, ...) {if(!(x)){VX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_debugtrap();} }
    #define VX_ASSERT(x, ...) {if(!(x)){VX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_debugtrap();} }
#else

#endif

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

#pragma once
#if defined(_WIN32)
    #ifdef DOTNETDUPE_EXPORTS
        #define DOTNETDUPE_API __declspec(dllexport)
    #else
        #define DOTNETDUPE_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define DOTNETDUPE_API __attribute__ ((visibility ("default")))
    #else
        #define DOTNETDUPE_API
    #endif
#endif
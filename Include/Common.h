#pragma once

typedef unsigned char byte;

#if defined(_WIN32)
    #pragma warning(disable: 4251) // dll-interface warning for private template members
    #pragma warning(disable: 4275) // non dll-interface class used as base for dll-interface class
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
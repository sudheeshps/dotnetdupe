/// \file Common.h
/// \brief Defines common cross-platform macros, export decorators, and fundamental types.

#pragma once

/// \typedef byte
/// \brief Represents an 8-bit unsigned integer.
typedef unsigned char byte;

#if defined(_WIN32)
    #pragma warning(disable: 4251) // dll-interface warning for private template members
    #pragma warning(disable: 4275) // non dll-interface class used as base for dll-interface class
    #ifdef DOTNETDUPE_EXPORTS
        /// \def DOTNETDUPE_API
        /// \brief Platform-specific linkage decoration for exporting or importing library symbols.
        #define DOTNETDUPE_API __declspec(dllexport)
    #else
        /// \def DOTNETDUPE_API
        /// \brief Platform-specific linkage decoration for exporting or importing library symbols.
        #define DOTNETDUPE_API __declspec(dllimport)
    #endif
    #ifdef SetPort
        #undef SetPort
    #endif
    #ifdef GetPort
        #undef GetPort
    #endif
#else
    #if __GNUC__ >= 4
        /// \def DOTNETDUPE_API
        /// \brief Platform-specific linkage decoration for exporting or importing library symbols.
        #define DOTNETDUPE_API __attribute__ ((visibility ("default")))
    #else
        /// \def DOTNETDUPE_API
        /// \brief Platform-specific linkage decoration for exporting or importing library symbols.
        #define DOTNETDUPE_API
    #endif
#endif
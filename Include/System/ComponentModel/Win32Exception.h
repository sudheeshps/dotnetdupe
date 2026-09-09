/// \file Win32Exception.h
/// \brief Exception thrown for a Win32 or platform-native error code.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace ComponentModel {

            /// \class Win32Exception
            /// \brief Exception thrown for a Win32 or POSIX platform error code.
            ///
            /// Encapsulates the native operating system error code obtained via GetLastError()
            /// on Windows or errno on POSIX platforms, formatting a localized system message per ECMA-335.
            class Win32Exception : public SystemException {
            private:
                int m_nNativeErrorCode;

            public:
                /// \brief Initializes a new instance of the Win32Exception class with the last Win32/native error code.
                DOTNETDUPE_API Win32Exception();

                /// \brief Initializes a new instance of the Win32Exception class with the specified native error code.
                /// \param nNativeErrorCode The native error code associated with this exception.
                DOTNETDUPE_API explicit Win32Exception(int nNativeErrorCode);

                /// \brief Initializes a new instance of the Win32Exception class with the specified error code and detailed description.
                /// \param nNativeErrorCode The native error code associated with this exception.
                /// \param sMessage The detailed error description.
                DOTNETDUPE_API Win32Exception(int nNativeErrorCode, const String& sMessage);

                /// \brief Initializes a new instance of the Win32Exception class with the specified detailed description.
                /// \param sMessage The detailed error description.
                DOTNETDUPE_API Win32Exception(const String& sMessage);

                /// \brief Gets the native error code associated with this exception.
                /// \return The native platform error code.
                DOTNETDUPE_API int GetNativeErrorCode() const { return m_nNativeErrorCode; }
            };

        }
    }
}

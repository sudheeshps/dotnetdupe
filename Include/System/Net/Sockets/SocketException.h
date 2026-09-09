/// \file SocketException.h
/// \brief Exception thrown when a network socket error occurs per RFC 793 / RFC 768.

#pragma once

#include "Common.h"
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \class SocketException
                /// \brief Exception thrown when a socket error occurs during network communications.
                ///
                /// Encapsulates native Berkeley sockets error codes (such as WSAE* on Windows,
                /// E* on POSIX) per ECMA-335 and POSIX.1g.
                class SocketException : public SystemException {
                private:
                    int m_errorCode;

                public:
                    /// \brief Initializes a new instance of the SocketException class with the specified error code and message.
                    /// \param errorCode The native socket error code.
                    /// \param message The message that describes the error.
                    DOTNETDUPE_API SocketException(int errorCode, const String& message);

                    /// \brief Gets the error code associated with this exception.
                    /// \return An integer representing the native socket error code.
                    DOTNETDUPE_API int GetErrorCode() const;
                };

            }
        }
    }
}

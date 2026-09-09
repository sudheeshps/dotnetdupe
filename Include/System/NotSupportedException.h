/// \file NotSupportedException.h
/// \brief Defines the exception thrown when an invoked method or stream operation is not supported.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class NotSupportedException
        /// \brief The exception that is thrown when an invoked method is not supported, or when there is an attempt to read, seek, or write to a stream that does not support the related functionality.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.32 (System.NotSupportedException).
        class NotSupportedException : public SystemException {
        public:
            /// \brief Initializes a new instance of the NotSupportedException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            NotSupportedException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class TimeoutException
        /// \brief The exception that is thrown when the time allotted for a process or operation has expired.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.37 (System.TimeoutException).
        class TimeoutException : public SystemException {
        public:
            /// \brief Initializes a new instance of the TimeoutException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            TimeoutException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}

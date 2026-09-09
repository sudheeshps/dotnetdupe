#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class NullReferenceException
        /// \brief The exception that is thrown when there is an attempt to dereference a null object reference.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.33 (System.NullReferenceException).
        class NullReferenceException : public SystemException {
        public:
            /// \brief Initializes a new instance of the NullReferenceException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            NullReferenceException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}

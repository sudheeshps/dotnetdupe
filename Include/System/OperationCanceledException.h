#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class OperationCanceledException
        /// \brief The exception that is thrown in a thread upon cancellation of an operation that the thread was executing.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.35 (System.OperationCanceledException).
        class OperationCanceledException : public SystemException {
        public:
            /// \brief Initializes a new instance of the OperationCanceledException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            OperationCanceledException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}

#pragma once
#include "System/SystemException.h"

namespace DotNetDupe {
    namespace System {

        /// \class NotImplementedException
        /// \brief The exception that is thrown when a requested method or operation is not implemented.
        ///
        /// \note Conforms to ECMA-335 Partition IV Section 5.31 (System.NotImplementedException).
        class NotImplementedException : public SystemException {
        public:
            /// \brief Initializes a new instance of the NotImplementedException class with a specified error message.
            /// \param sMessage The error message that explains the reason for the exception.
            NotImplementedException(const String& sMessage) : SystemException(sMessage) {}
        };

    }
}

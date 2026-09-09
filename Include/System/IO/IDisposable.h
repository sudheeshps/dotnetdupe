#pragma once

#include "Common.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \interface IDisposable
            /// \brief Provides a mechanism for releasing unmanaged resources.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.1 specification for
            /// resource cleanup patterns and deterministic resource deallocation.
            class IDisposable {
            public:
                /// \brief Virtual destructor ensuring proper polymorphic cleanup.
                virtual ~IDisposable() = default;

                /// \brief Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
                virtual void Dispose() = 0;
            };
        }
    }
}

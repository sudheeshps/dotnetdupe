#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "Extensions/DependencyInjection/IServiceScope.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Defines a factory interface used to create instances of IServiceScope.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.IServiceScopeFactory.
            /// Provides a thread-safe mechanism to instantiate child scopes for per-request lifetimes.
            /// \cite Microsoft.Extensions.DependencyInjection
            class IServiceScopeFactory : public DotNetDupe::System::Object {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~IServiceScopeFactory() = default;

                /// \brief Creates an IServiceScope which controls the lifetime of scoped services.
                /// \return SmartPointer to the newly created IServiceScope.
                virtual DotNetDupe::System::SmartPointer<IServiceScope> CreateScope() = 0;
            };
        }
    }
}


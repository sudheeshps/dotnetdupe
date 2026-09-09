#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Controls the lifetime of a scoped service.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.IServiceScope.
            /// Scopes encapsulate child containers where services registered with ServiceLifetime::Scoped
            /// are resolved as singletons within that scope and disposed when the scope is disposed.
            /// \cite Microsoft.Extensions.DependencyInjection
            class IServiceScope : public DotNetDupe::System::Object, public DotNetDupe::System::IO::IDisposable {
            public:
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~IServiceScope() = default;

                /// \brief Gets the scoped IServiceProvider used to resolve dependencies from this scope.
                /// \return SmartPointer to the scoped IServiceProvider.
                virtual DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> GetServiceProvider() const = 0;
            };
        }
    }
}


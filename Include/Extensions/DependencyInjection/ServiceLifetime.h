#pragma once

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Specifies the lifetime of a service in an IServiceCollection.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.ServiceLifetime.
            /// Defines whether a registered service is created once globally (Singleton),
            /// once per child resolution scope (Scoped), or anew upon every request (Transient).
            /// \cite Microsoft.Extensions.DependencyInjection
            enum class ServiceLifetime {
                /// \brief Specifies that a single instance of the service will be created.
                Singleton,
                /// \brief Specifies that a new instance of the service will be created for each scope.
                Scoped,
                /// \brief Specifies that a new instance of the service will be created every time it is requested.
                Transient
            };
        }
    }
}


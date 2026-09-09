#pragma once

#include "Extensions/DependencyInjection/IServiceCollection.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Default implementation of the IServiceCollection interface.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.ServiceCollection.
            /// Serves as the primary registration builder for application services,
            /// which compiles into an immutable ServiceProvider via BuildServiceProvider().
            /// \cite Microsoft.Extensions.DependencyInjection
            class ServiceCollection : public IServiceCollection {
            public:
                /// \brief Default constructor initializing an empty collection.
                ServiceCollection() = default;
                /// \brief Virtual destructor for polymorphic cleanup.
                ~ServiceCollection() override = default;
                /// \brief Copy constructor.
                ServiceCollection(const ServiceCollection&) = default;
                /// \brief Copy assignment operator.
                ServiceCollection& operator=(const ServiceCollection&) = default;
                /// \brief Move constructor.
                ServiceCollection(ServiceCollection&&) noexcept = default;
                /// \brief Move assignment operator.
                ServiceCollection& operator=(ServiceCollection&&) noexcept = default;

                /// \brief Compiles the registered service descriptors into an IServiceProvider.
                /// \return SmartPointer to the root IServiceProvider.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> BuildServiceProvider();
            };
        }
    }
}

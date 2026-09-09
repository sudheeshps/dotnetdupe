#include "pch.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> ServiceCollection::BuildServiceProvider() {
                /// Instantiate the concrete root ServiceProvider with the current descriptor registry.
                DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> pProvider(
                    new ServiceProvider(*this),
                    true
                );

                /// Return the initialized container instance.
                return pProvider;
            }
        }
    }
}

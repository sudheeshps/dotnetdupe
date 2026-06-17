#include "pch.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> ServiceCollection::BuildServiceProvider() {
                return DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                    new ServiceProvider(*this),
                    true
                );
            }
        }
    }
}

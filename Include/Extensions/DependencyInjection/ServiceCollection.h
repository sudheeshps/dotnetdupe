#pragma once

#include "Extensions/DependencyInjection/IServiceCollection.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            class ServiceCollection : public IServiceCollection {
            public:
                ServiceCollection() = default;
                ~ServiceCollection() override = default;
                ServiceCollection(const ServiceCollection&) = default;
                ServiceCollection& operator=(const ServiceCollection&) = default;
                ServiceCollection(ServiceCollection&&) noexcept = default;
                ServiceCollection& operator=(ServiceCollection&&) noexcept = default;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> BuildServiceProvider();
            };
        }
    }
}

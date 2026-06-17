#pragma once

#include "Extensions/DependencyInjection/IServiceCollection.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            class ServiceCollection : public IServiceCollection {
            public:
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> BuildServiceProvider();
            };
        }
    }
}

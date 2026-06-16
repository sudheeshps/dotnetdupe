#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "Extensions/DependencyInjection/IServiceScope.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            class IServiceScopeFactory : public DotNetDupe::System::Object {
            public:
                virtual ~IServiceScopeFactory() = default;
                virtual DotNetDupe::System::SmartPointer<IServiceScope> CreateScope() = 0;
            };
        }
    }
}

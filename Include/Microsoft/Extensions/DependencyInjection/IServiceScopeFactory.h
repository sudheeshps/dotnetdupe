#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "Microsoft/Extensions/DependencyInjection/IServiceScope.h"

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {
                class IServiceScopeFactory : public System::Object {
                public:
                    virtual ~IServiceScopeFactory() = default;
                    virtual System::SmartPointer<IServiceScope> CreateScope() = 0;
                };
            }
        }
    }
}

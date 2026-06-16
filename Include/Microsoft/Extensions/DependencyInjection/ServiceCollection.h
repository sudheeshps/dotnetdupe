#pragma once

#include "Microsoft/Extensions/DependencyInjection/IServiceCollection.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {
                class ServiceCollection : public IServiceCollection {
                public:
                    DOTNETDUPE_API System::SmartPointer<System::IServiceProvider> BuildServiceProvider();
                };
            }
        }
    }
}

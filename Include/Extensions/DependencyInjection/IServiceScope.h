#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            class IServiceScope : public DotNetDupe::System::Object, public DotNetDupe::System::IO::IDisposable {
            public:
                virtual ~IServiceScope() = default;
                virtual DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> GetServiceProvider() const = 0;
            };
        }
    }
}

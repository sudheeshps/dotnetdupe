#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {
                class IServiceScope : public System::Object, public System::IO::IDisposable {
                public:
                    virtual ~IServiceScope() = default;
                    virtual System::SmartPointer<System::IServiceProvider> GetServiceProvider() const = 0;
                };
            }
        }
    }
}

#pragma once

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {
                enum class ServiceLifetime {
                    Singleton,
                    Scoped,
                    Transient
                };
            }
        }
    }
}

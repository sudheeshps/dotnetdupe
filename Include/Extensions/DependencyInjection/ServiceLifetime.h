#pragma once

namespace DotNetDupe {
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

#pragma once

#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"
#include "System/Threading/CriticalSection.h"
#include "Microsoft/Extensions/DependencyInjection/IServiceCollection.h"
#include "Microsoft/Extensions/DependencyInjection/ServiceDescriptor.h"
#include "Microsoft/Extensions/DependencyInjection/IServiceScopeFactory.h"
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {

                class ServiceProvider : public System::IServiceProvider, public System::IO::IDisposable {
                public:
                    DOTNETDUPE_API ServiceProvider(const IServiceCollection& collection);
                    DOTNETDUPE_API ServiceProvider(ServiceProvider* pRootProvider);
                    DOTNETDUPE_API ~ServiceProvider() override;

                    DOTNETDUPE_API System::SmartPointer<System::Object> GetService(const std::type_index& serviceType) override;
                    DOTNETDUPE_API void Dispose() override;

                private:
                    System::SmartPointer<System::Object> ResolveService(const ServiceDescriptor& descriptor);

                    bool m_bIsRoot;
                    ServiceProvider* m_pRootProvider;
                    
                    std::unordered_map<std::type_index, ServiceDescriptor> m_mapDescriptors;
                    std::unordered_map<std::type_index, System::SmartPointer<System::Object>> m_mapSingletons;
                    std::unordered_map<std::type_index, System::SmartPointer<System::Object>> m_mapScoped;
                    std::vector<System::SmartPointer<System::IO::IDisposable>> m_vDisposables;
                    System::Threading::CriticalSection m_csLock;
                };

                class ServiceScope : public IServiceScope {
                public:
                    DOTNETDUPE_API ServiceScope(System::SmartPointer<ServiceProvider> spProvider);
                    DOTNETDUPE_API ~ServiceScope() override;

                    DOTNETDUPE_API System::SmartPointer<System::IServiceProvider> GetServiceProvider() const override;
                    DOTNETDUPE_API void Dispose() override;

                private:
                    System::SmartPointer<ServiceProvider> m_spProvider;
                };

                class ServiceScopeFactory : public IServiceScopeFactory {
                public:
                    DOTNETDUPE_API ServiceScopeFactory(ServiceProvider* pProvider);
                    DOTNETDUPE_API ~ServiceScopeFactory() override = default;

                    DOTNETDUPE_API System::SmartPointer<IServiceScope> CreateScope() override;

                private:
                    ServiceProvider* m_pProvider;
                };
            }
        }
    }
}

#pragma once

#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"
#include "System/Threading/CriticalSection.h"
#include "Extensions/DependencyInjection/IServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceDescriptor.h"
#include "Extensions/DependencyInjection/IServiceScopeFactory.h"
#include <typeindex>
#include <typeindex>

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {

            class ServiceProvider : public DotNetDupe::System::IServiceProvider, public DotNetDupe::System::IO::IDisposable {
            public:
                DOTNETDUPE_API ServiceProvider(const IServiceCollection& collection);
                DOTNETDUPE_API ServiceProvider(ServiceProvider* pRootProvider);
                DOTNETDUPE_API ~ServiceProvider() override;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> GetService(const std::type_index& serviceType) override;
                DOTNETDUPE_API void Dispose() override;

            private:
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveService(const ServiceDescriptor& descriptor);

                bool m_bIsRoot;
                ServiceProvider* m_pRootProvider;
                
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;
            };

            class ServiceScope : public IServiceScope {
            public:
                DOTNETDUPE_API ServiceScope(DotNetDupe::System::SmartPointer<ServiceProvider> pProvider);
                DOTNETDUPE_API ~ServiceScope() override;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> GetServiceProvider() const override;
                DOTNETDUPE_API void Dispose() override;

            private:
                DotNetDupe::System::SmartPointer<ServiceProvider> m_pProvider;
            };

            class ServiceScopeFactory : public IServiceScopeFactory {
            public:
                DOTNETDUPE_API ServiceScopeFactory(ServiceProvider* pProvider);
                DOTNETDUPE_API ~ServiceScopeFactory() override = default;

                DOTNETDUPE_API DotNetDupe::System::SmartPointer<IServiceScope> CreateScope() override;

            private:
                ServiceProvider* m_pProvider;
            };
        }
    }
}

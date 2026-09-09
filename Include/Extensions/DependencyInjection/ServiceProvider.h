#pragma once

#include "System/IServiceProvider.h"
#include "System/IO/IDisposable.h"
#include "System/Threading/CriticalSection.h"
#include "Extensions/DependencyInjection/IServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceDescriptor.h"
#include "Extensions/DependencyInjection/IServiceScopeFactory.h"
#include <typeindex>

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {

            /// \brief Default IoC service provider for resolving dependencies.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.ServiceProvider.
            /// Manages singleton caching, scoped resolution lifecycles, and recursive dependency tree resolution.
            /// Implements IDisposable to dispose resolved disposable singletons on shutdown.
            /// \cite Microsoft.Extensions.DependencyInjection
            class ServiceProvider : public DotNetDupe::System::IServiceProvider, public DotNetDupe::System::IO::IDisposable {
            public:
                /// \brief Initializes a root ServiceProvider from an IServiceCollection.
                /// \param collection The service descriptor collection to build from.
                DOTNETDUPE_API ServiceProvider(const IServiceCollection& collection);

                /// \brief Initializes a child scoped ServiceProvider inheriting descriptors from a root provider.
                /// \param pRootProvider Pointer to the root ServiceProvider.
                DOTNETDUPE_API ServiceProvider(ServiceProvider* pRootProvider);

                /// \brief Destructor releasing resolved services.
                DOTNETDUPE_API ~ServiceProvider() override;

                /// \brief Resolves a service object of the specified type.
                /// \param serviceType The type index of the requested service.
                /// \return SmartPointer to the resolved service Object, or null if unregistered.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> GetService(const std::type_index& serviceType) override;

                /// \brief Disposes all resolved singleton instances and clears cached references.
                DOTNETDUPE_API void Dispose() override;

            private:
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveService(const ServiceDescriptor& descriptor);

                bool m_bIsRoot;
                ServiceProvider* m_pRootProvider;
                
                struct Impl;
                DotNetDupe::System::SmartPointer<Impl> m_pImpl;
            };

            /// \brief Concrete implementation of IServiceScope for managing scoped lifetimes.
            /// \details Owns a child ServiceProvider instance and disposes it when the scope ends.
            /// \cite Microsoft.Extensions.DependencyInjection
            class ServiceScope : public IServiceScope {
            public:
                /// \brief Initializes a ServiceScope wrapping a scoped ServiceProvider.
                /// \param pProvider SmartPointer to the child ServiceProvider.
                DOTNETDUPE_API ServiceScope(DotNetDupe::System::SmartPointer<ServiceProvider> pProvider);

                /// \brief Destructor disposing the child scope provider.
                DOTNETDUPE_API ~ServiceScope() override;

                /// \brief Gets the scoped IServiceProvider.
                /// \return SmartPointer to the scoped IServiceProvider.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> GetServiceProvider() const override;

                /// \brief Disposes the scope and all scoped instances created within it.
                DOTNETDUPE_API void Dispose() override;

            private:
                DotNetDupe::System::SmartPointer<ServiceProvider> m_pProvider;
            };

            /// \brief Concrete implementation of IServiceScopeFactory.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.ServiceScopeFactory.
            /// Creates child ServiceScope instances bound to the root container.
            /// \cite Microsoft.Extensions.DependencyInjection
            class ServiceScopeFactory : public IServiceScopeFactory {
            public:
                /// \brief Initializes a ServiceScopeFactory bound to a root provider.
                /// \param pProvider Raw pointer to the parent ServiceProvider.
                DOTNETDUPE_API ServiceScopeFactory(ServiceProvider* pProvider);

                /// \brief Virtual destructor.
                DOTNETDUPE_API ~ServiceScopeFactory() override = default;

                /// \brief Creates a new child IServiceScope.
                /// \return SmartPointer to the newly created IServiceScope.
                DOTNETDUPE_API DotNetDupe::System::SmartPointer<IServiceScope> CreateScope() override;

            private:
                ServiceProvider* m_pProvider;
            };
        }
    }
}

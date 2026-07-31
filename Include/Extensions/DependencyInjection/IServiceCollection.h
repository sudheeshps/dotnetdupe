#pragma once

#include "System/Collections/Generic/List.h"
#include "Extensions/DependencyInjection/ServiceDescriptor.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            class IServiceCollection : public DotNetDupe::System::Collections::Generic::List<ServiceDescriptor> {
            public:
                IServiceCollection() = default;
                virtual ~IServiceCollection() = default;
                IServiceCollection(const IServiceCollection&) = default;
                IServiceCollection& operator=(const IServiceCollection&) = default;
                IServiceCollection(IServiceCollection&&) noexcept = default;
                IServiceCollection& operator=(IServiceCollection&&) noexcept = default;

                // Singleton
                template <typename TService, typename TImplementation>
                IServiceCollection& AddSingleton() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation>(ServiceLifetime::Singleton));
                    return *this;
                }

                template <typename TService>
                IServiceCollection& AddSingleton(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Singleton));
                    return *this;
                }

                template <typename TService>
                IServiceCollection& AddSingleton(DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> spInstance) {
                    this->Add(ServiceDescriptor(typeid(TService), std::move(spInstance)));
                    return *this;
                }

                // Transient
                template <typename TService, typename TImplementation>
                IServiceCollection& AddTransient() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation>(ServiceLifetime::Transient));
                    return *this;
                }

                template <typename TService>
                IServiceCollection& AddTransient(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Transient));
                    return *this;
                }

                // Scoped
                template <typename TService, typename TImplementation>
                IServiceCollection& AddScoped() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation>(ServiceLifetime::Scoped));
                    return *this;
                }

                template <typename TService>
                IServiceCollection& AddScoped(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Scoped));
                    return *this;
                }
            };
        }
    }
}

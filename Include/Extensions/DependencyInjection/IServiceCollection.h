#pragma once

#include "System/Collections/Generic/List.h"
#include "Extensions/DependencyInjection/ServiceDescriptor.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Specifies the contract for a collection of service descriptors.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.IServiceCollection.
            /// Inherits from List<ServiceDescriptor> to provide fluent registration methods
            /// for Singleton, Transient, and Scoped service dependencies.
            /// \cite Microsoft.Extensions.DependencyInjection
            class IServiceCollection : public DotNetDupe::System::Collections::Generic::List<ServiceDescriptor> {
            public:
                /// \brief Default constructor initializing an empty collection.
                IServiceCollection() = default;
                /// \brief Virtual destructor for polymorphic cleanup.
                virtual ~IServiceCollection() = default;
                /// \brief Copy constructor.
                IServiceCollection(const IServiceCollection&) = default;
                /// \brief Copy assignment operator.
                IServiceCollection& operator=(const IServiceCollection&) = default;
                /// \brief Move constructor.
                IServiceCollection(IServiceCollection&&) noexcept = default;
                /// \brief Move assignment operator.
                IServiceCollection& operator=(IServiceCollection&&) noexcept = default;

                /// \brief Adds a singleton service of type TService with an implementation of TImplementation.
                /// \tparam TService The interface or base type to register.
                /// \tparam TImplementation The concrete type implementing TService.
                /// \tparam TArgs Types of dependencies passed to TImplementation's constructor.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService, typename TImplementation, typename... TArgs>
                IServiceCollection& AddSingleton() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation, TArgs...>(ServiceLifetime::Singleton));
                    return *this;
                }

                /// \brief Adds a singleton service of type TService with a factory delegate.
                /// \tparam TService The interface or base type to register.
                /// \param fnFactory Factory function used to instantiate the service.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService>
                IServiceCollection& AddSingleton(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Singleton));
                    return *this;
                }

                /// \brief Adds a pre-existing singleton instance of type TService.
                /// \tparam TService The interface or base type to register.
                /// \param pInstance SmartPointer to the existing instance.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService>
                IServiceCollection& AddSingleton(DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance) {
                    this->Add(ServiceDescriptor(typeid(TService), std::move(pInstance)));
                    return *this;
                }

                /// \brief Adds a transient service of type TService with an implementation of TImplementation.
                /// \tparam TService The interface or base type to register.
                /// \tparam TImplementation The concrete type implementing TService.
                /// \tparam TArgs Types of dependencies passed to TImplementation's constructor.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService, typename TImplementation, typename... TArgs>
                IServiceCollection& AddTransient() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation, TArgs...>(ServiceLifetime::Transient));
                    return *this;
                }

                /// \brief Adds a transient service of type TService with a factory delegate.
                /// \tparam TService The interface or base type to register.
                /// \param fnFactory Factory function used to instantiate the service.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService>
                IServiceCollection& AddTransient(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Transient));
                    return *this;
                }

                /// \brief Adds a scoped service of type TService with an implementation of TImplementation.
                /// \tparam TService The interface or base type to register.
                /// \tparam TImplementation The concrete type implementing TService.
                /// \tparam TArgs Types of dependencies passed to TImplementation's constructor.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService, typename TImplementation, typename... TArgs>
                IServiceCollection& AddScoped() {
                    this->Add(ServiceDescriptor::Describe<TService, TImplementation, TArgs...>(ServiceLifetime::Scoped));
                    return *this;
                }

                /// \brief Adds a scoped service of type TService with a factory delegate.
                /// \tparam TService The interface or base type to register.
                /// \param fnFactory Factory function used to instantiate the service.
                /// \return Reference to this IServiceCollection for method chaining.
                template <typename TService>
                IServiceCollection& AddScoped(ServiceDescriptor::FactoryType fnFactory) {
                    this->Add(ServiceDescriptor::Describe(typeid(TService), std::move(fnFactory), ServiceLifetime::Scoped));
                    return *this;
                }
            };
        }
    }
}

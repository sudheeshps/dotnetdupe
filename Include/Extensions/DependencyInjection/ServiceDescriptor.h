#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "Extensions/DependencyInjection/ServiceLifetime.h"
#include <typeindex>
#include <functional>
#include <utility>

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {
            /// \brief Describes a service with its service type, implementation, and lifetime.
            /// \details Modeled after Microsoft.Extensions.DependencyInjection.ServiceDescriptor.
            /// Encapsulates metadata required by a ServiceProvider to resolve dependencies,
            /// supporting instance registrations, factory delegates, and type-based instantiation.
            /// \cite Microsoft.Extensions.DependencyInjection
            class ServiceDescriptor : public DotNetDupe::System::Object {
            public:
                /// \brief Function signature for dynamic factory service instantiation delegates.
                using FactoryType = std::function<DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>(const DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>&)>;

                /// \brief Default constructor initializing an empty descriptor.
                ServiceDescriptor() 
                    : m_serviceType(typeid(void)), 
                      m_implementationType(typeid(void)), 
                      m_eLifetime(ServiceLifetime::Transient), 
                      m_fnFactory(nullptr), 
                      m_pInstance(nullptr) {}

                /// \brief Initializes a descriptor with explicit service type, implementation type, and lifetime.
                /// \param serviceType The type of the service interface or abstract class.
                /// \param implementationType The concrete class implementing the service.
                /// \param eLifetime The lifecycle scope of the registered service.
                ServiceDescriptor(
                    const std::type_index& serviceType,
                    const std::type_index& implementationType,
                    ServiceLifetime eLifetime
                ) : m_serviceType(serviceType),
                    m_implementationType(implementationType),
                    m_eLifetime(eLifetime),
                    m_fnFactory(nullptr),
                    m_pInstance(nullptr) {}

                /// \brief Initializes a descriptor with a factory delegate.
                /// \param serviceType The type of the service to register.
                /// \param fnFactory The factory function invoked to instantiate the service.
                /// \param eLifetime The lifecycle scope of the registered service.
                ServiceDescriptor(
                    const std::type_index& serviceType,
                    FactoryType fnFactory,
                    ServiceLifetime eLifetime
                ) : m_serviceType(serviceType),
                    m_implementationType(serviceType),
                    m_eLifetime(eLifetime),
                    m_fnFactory(std::move(fnFactory)),
                    m_pInstance(nullptr) {}

                /// \brief Initializes a descriptor with a pre-existing singleton instance.
                /// \param serviceType The type of the service to register.
                /// \param pInstance SmartPointer to the pre-existing singleton instance.
                ServiceDescriptor(
                    const std::type_index& serviceType,
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance
                ) : m_serviceType(serviceType),
                    m_implementationType(serviceType),
                    m_eLifetime(ServiceLifetime::Singleton),
                    m_fnFactory(nullptr),
                    m_pInstance(std::move(pInstance)) {}

                /// \brief Gets the service type represented by this descriptor.
                /// \return The std::type_index of the service interface or base class.
                std::type_index GetServiceType() const { return m_serviceType; }

                /// \brief Gets the concrete implementation type for this service.
                /// \return The std::type_index of the concrete implementation class.
                std::type_index GetImplementationType() const { return m_implementationType; }

                /// \brief Gets the lifetime scope configured for this service.
                /// \return ServiceLifetime enumeration value.
                ServiceLifetime GetLifetime() const { return m_eLifetime; }

                /// \brief Gets the optional factory function delegate.
                /// \return Reference to the factory function delegate, or empty if not registered.
                const FactoryType& GetFactory() const { return m_fnFactory; }

                /// \brief Gets the optional pre-existing singleton instance.
                /// \return Reference to the singleton instance pointer, or null if type-based.
                const DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>& GetInstance() const { return m_pInstance; }

                /// \brief Creates a ServiceDescriptor with the specified service type, implementation type, and lifetime.
                /// \param serviceType The service contract type.
                /// \param implementationType The concrete implementation type.
                /// \param eLifetime The service lifetime.
                /// \return A newly configured ServiceDescriptor.
                static ServiceDescriptor Describe(
                    const std::type_index& serviceType,
                    const std::type_index& implementationType,
                    ServiceLifetime eLifetime
                ) {
                    return ServiceDescriptor(serviceType, implementationType, eLifetime);
                }

                /// \brief Creates a ServiceDescriptor using a factory function delegate.
                /// \param serviceType The service contract type.
                /// \param fnFactory The factory delegate.
                /// \param eLifetime The service lifetime.
                /// \return A newly configured ServiceDescriptor.
                static ServiceDescriptor Describe(
                    const std::type_index& serviceType,
                    FactoryType fnFactory,
                    ServiceLifetime eLifetime
                ) {
                    return ServiceDescriptor(serviceType, std::move(fnFactory), eLifetime);
                }

                /// \brief Creates a ServiceDescriptor resolving constructor dependencies via IServiceProvider.
                /// \tparam TService The service contract interface or base class.
                /// \tparam TImplementation The concrete class implementing TService.
                /// \tparam TArgs Constructor argument types resolved from the container.
                /// \param eLifetime The service lifetime.
                /// \return A newly configured ServiceDescriptor.
                template <typename TService, typename TImplementation, typename... TArgs>
                static ServiceDescriptor Describe(ServiceLifetime eLifetime) {
                    return ServiceDescriptor(
                        typeid(TService),
                        [](const DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>& sp) {
                            return DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>(DotNetDupe::System::SmartPointer<TImplementation>::NewShared(sp->GetRequiredService<TArgs>()...));
                        },
                        eLifetime
                    );
                }

            private:
                std::type_index m_serviceType;
                std::type_index m_implementationType;
                ServiceLifetime m_eLifetime;
                FactoryType m_fnFactory;
                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> m_pInstance;
            };
        }
    }
}


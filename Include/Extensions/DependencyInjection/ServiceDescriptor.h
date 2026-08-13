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
            class ServiceDescriptor : public DotNetDupe::System::Object {
            public:
                using FactoryType = std::function<DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>(const DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>&)>;

                ServiceDescriptor() 
                    : m_serviceType(typeid(void)), 
                      m_implementationType(typeid(void)), 
                      m_eLifetime(ServiceLifetime::Transient), 
                      m_fnFactory(nullptr), 
                      m_pInstance(nullptr) {}

                ServiceDescriptor(
                    const std::type_index& serviceType,
                    const std::type_index& implementationType,
                    ServiceLifetime eLifetime
                ) : m_serviceType(serviceType),
                    m_implementationType(implementationType),
                    m_eLifetime(eLifetime),
                    m_fnFactory(nullptr),
                    m_pInstance(nullptr) {}

                ServiceDescriptor(
                    const std::type_index& serviceType,
                    FactoryType fnFactory,
                    ServiceLifetime eLifetime
                ) : m_serviceType(serviceType),
                    m_implementationType(serviceType),
                    m_eLifetime(eLifetime),
                    m_fnFactory(std::move(fnFactory)),
                    m_pInstance(nullptr) {}

                ServiceDescriptor(
                    const std::type_index& serviceType,
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance
                ) : m_serviceType(serviceType),
                    m_implementationType(serviceType),
                    m_eLifetime(ServiceLifetime::Singleton),
                    m_fnFactory(nullptr),
                    m_pInstance(std::move(pInstance)) {}

                // Getters
                std::type_index GetServiceType() const { return m_serviceType; }
                std::type_index GetImplementationType() const { return m_implementationType; }
                ServiceLifetime GetLifetime() const { return m_eLifetime; }
                const FactoryType& GetFactory() const { return m_fnFactory; }
                const DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>& GetInstance() const { return m_pInstance; }

                // Describe static helpers
                static ServiceDescriptor Describe(
                    const std::type_index& serviceType,
                    const std::type_index& implementationType,
                    ServiceLifetime eLifetime
                ) {
                    return ServiceDescriptor(serviceType, implementationType, eLifetime);
                }

                static ServiceDescriptor Describe(
                    const std::type_index& serviceType,
                    FactoryType fnFactory,
                    ServiceLifetime eLifetime
                ) {
                    return ServiceDescriptor(serviceType, std::move(fnFactory), eLifetime);
                }

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

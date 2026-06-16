#pragma once

#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/IServiceProvider.h"
#include "Microsoft/Extensions/DependencyInjection/ServiceLifetime.h"
#include <typeindex>
#include <functional>
#include <utility>

namespace DotNetDupe {
    namespace Microsoft {
        namespace Extensions {
            namespace DependencyInjection {
                class ServiceDescriptor : public System::Object {
                public:
                    using FactoryType = std::function<System::SmartPointer<System::Object>(const System::SmartPointer<System::IServiceProvider>&)>;

                    ServiceDescriptor(
                        const std::type_index& serviceType,
                        const std::type_index& implementationType,
                        ServiceLifetime eLifetime
                    ) : m_serviceType(serviceType),
                        m_implementationType(implementationType),
                        m_eLifetime(eLifetime),
                        m_fnFactory(nullptr),
                        m_spInstance(nullptr) {}

                    ServiceDescriptor(
                        const std::type_index& serviceType,
                        FactoryType fnFactory,
                        ServiceLifetime eLifetime
                    ) : m_serviceType(serviceType),
                        m_implementationType(serviceType),
                        m_eLifetime(eLifetime),
                        m_fnFactory(std::move(fnFactory)),
                        m_spInstance(nullptr) {}

                    ServiceDescriptor(
                        const std::type_index& serviceType,
                        System::SmartPointer<System::Object> spInstance
                    ) : m_serviceType(serviceType),
                        m_implementationType(serviceType),
                        m_eLifetime(ServiceLifetime::Singleton),
                        m_fnFactory(nullptr),
                        m_spInstance(std::move(spInstance)) {}

                    // Getters
                    std::type_index GetServiceType() const { return m_serviceType; }
                    std::type_index GetImplementationType() const { return m_implementationType; }
                    ServiceLifetime GetLifetime() const { return m_eLifetime; }
                    const FactoryType& GetFactory() const { return m_fnFactory; }
                    const System::SmartPointer<System::Object>& GetInstance() const { return m_spInstance; }

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

                    template <typename TService, typename TImplementation>
                    static ServiceDescriptor Describe(ServiceLifetime eLifetime) {
                        return ServiceDescriptor(
                            typeid(TService),
                            [](const System::SmartPointer<System::IServiceProvider>&) {
                                return System::SmartPointer<System::Object>(System::SmartPointer<TImplementation>::NewShared());
                            },
                            eLifetime
                        );
                    }

                private:
                    std::type_index m_serviceType;
                    std::type_index m_implementationType;
                    ServiceLifetime m_eLifetime;
                    FactoryType m_fnFactory;
                    System::SmartPointer<System::Object> m_spInstance;
                };
            }
        }
    }
}

#include "pch.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace Extensions {
        namespace DependencyInjection {

            // Lightweight proxy class to safely wrap a ServiceProvider pointer
            // without ownership issues or double deletion.
            class ServiceProviderProxy : public DotNetDupe::System::IServiceProvider {
            public:
                ServiceProviderProxy(ServiceProvider* pProvider) : m_pProvider(pProvider) {}
                ~ServiceProviderProxy() override = default;

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> GetService(const std::type_index& serviceType) override {
                    return m_pProvider->GetService(serviceType);
                }

            private:
                ServiceProvider* m_pProvider;
            };

            // --- ServiceProvider Implementation ---
            
            struct ServiceProvider::Impl {
                std::unordered_map<std::type_index, ServiceDescriptor> mapDescriptors;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> mapSingletons;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> mapScoped;
                std::vector<DotNetDupe::System::SmartPointer<DotNetDupe::System::IO::IDisposable>> vDisposables;
                DotNetDupe::System::Threading::CriticalSection csLock;
            };

            ServiceProvider::ServiceProvider(const IServiceCollection& collection)
                : m_bIsRoot(true), m_pRootProvider(nullptr), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                for (int iIdx = 0; iIdx < collection.GetCount(); ++iIdx) {
                    const ServiceDescriptor& descriptor = collection[iIdx];
                    m_pImpl->mapDescriptors.insert({ descriptor.GetServiceType(), descriptor });
                }
            }

            ServiceProvider::ServiceProvider(ServiceProvider* pRootProvider)
                : m_bIsRoot(false), m_pRootProvider(pRootProvider), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
            }

            ServiceProvider::~ServiceProvider() {
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::GetService(const std::type_index& serviceType) {
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                // 1. Special services
                if (serviceType == typeid(DotNetDupe::System::IServiceProvider)) {
                    return DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>(
                        DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                    );
                }
                if (serviceType == typeid(IServiceScopeFactory)) {
                    return DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>(
                        DotNetDupe::System::SmartPointer<ServiceScopeFactory>::NewShared(this)
                    );
                }

                // 2. Find descriptor
                ServiceProvider* pRoot = m_bIsRoot ? this : m_pRootProvider;
                auto it = pRoot->m_pImpl->mapDescriptors.find(serviceType);
                if (it == pRoot->m_pImpl->mapDescriptors.end()) {
                    return nullptr;
                }

                return ResolveService(it->second);
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::ResolveService(const ServiceDescriptor& descriptor) {
                if (descriptor.GetLifetime() == ServiceLifetime::Singleton) {
                    ServiceProvider* pRoot = m_bIsRoot ? this : m_pRootProvider;
                    DotNetDupe::System::Threading::CriticalSectionLock lock(pRoot->m_pImpl->csLock);

                    // Check cache
                    auto it = pRoot->m_pImpl->mapSingletons.find(descriptor.GetServiceType());
                    if (it != pRoot->m_pImpl->mapSingletons.end()) {
                        return it->second;
                    }

                    // Create
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance;
                    if (descriptor.GetInstance()) {
                        pInstance = descriptor.GetInstance();
                    } else if (descriptor.GetFactory()) {
                        auto pProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        pInstance = descriptor.GetFactory()(pProxy);
                    }

                    if (!pInstance.IsNull()) {
                        pRoot->m_pImpl->mapSingletons[descriptor.GetServiceType()] = pInstance;

                        // Track IDisposable
                        auto pDisposable = pInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!pDisposable.IsNull()) {
                            pRoot->m_pImpl->vDisposables.push_back(pDisposable);
                        }
                    }
                    return pInstance;
                }
                else if (descriptor.GetLifetime() == ServiceLifetime::Scoped) {
                    if (m_bIsRoot) {
                        throw DotNetDupe::System::InvalidOperationException("Cannot resolve scoped service from root provider.");
                    }

                    DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                    // Check cache
                    auto it = m_pImpl->mapScoped.find(descriptor.GetServiceType());
                    if (it != m_pImpl->mapScoped.end()) {
                        return it->second;
                    }

                    // Create
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance;
                    if (descriptor.GetFactory()) {
                        auto pProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        pInstance = descriptor.GetFactory()(pProxy);
                    }

                    if (!pInstance.IsNull()) {
                        m_pImpl->mapScoped[descriptor.GetServiceType()] = pInstance;

                        // Track IDisposable
                        auto pDisposable = pInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!pDisposable.IsNull()) {
                            m_pImpl->vDisposables.push_back(pDisposable);
                        }
                    }
                    return pInstance;
                }
                else {
                    // Transient
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> pInstance;
                    if (descriptor.GetFactory()) {
                        auto pProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        pInstance = descriptor.GetFactory()(pProxy);
                    }

                    if (!pInstance.IsNull()) {
                        // Track transient disposables in the resolving provider to clean up on Dispose
                        auto pDisposable = pInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!pDisposable.IsNull()) {
                            DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);
                            m_pImpl->vDisposables.push_back(pDisposable);
                        }
                    }
                    return pInstance;
                }
            }

            void ServiceProvider::Dispose() {
                if (m_pImpl.IsNull()) return;
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                // Dispose in reverse order of registration
                for (auto it = m_pImpl->vDisposables.rbegin(); it != m_pImpl->vDisposables.rend(); ++it) {
                    if (!it->IsNull()) {
                        (*it)->Dispose();
                    }
                }
                m_pImpl->vDisposables.clear();

                m_pImpl->mapSingletons.clear();
                m_pImpl->mapScoped.clear();
            }

            // --- ServiceScope Implementation ---

            ServiceScope::ServiceScope(DotNetDupe::System::SmartPointer<ServiceProvider> pProvider)
                : m_pProvider(std::move(pProvider)) {}

            ServiceScope::~ServiceScope() {
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> ServiceScope::GetServiceProvider() const {
                return m_pProvider;
            }

            void ServiceScope::Dispose() {
                if (!m_pProvider.IsNull()) {
                    m_pProvider->Dispose();
                    m_pProvider = nullptr;
                }
            }

            // --- ServiceScopeFactory Implementation ---

            ServiceScopeFactory::ServiceScopeFactory(ServiceProvider* pProvider)
                : m_pProvider(pProvider) {}

            DotNetDupe::System::SmartPointer<IServiceScope> ServiceScopeFactory::CreateScope() {
                auto spScopedProvider = DotNetDupe::System::SmartPointer<ServiceProvider>::NewShared(m_pProvider);
                auto spScope = DotNetDupe::System::SmartPointer<ServiceScope>::NewShared(std::move(spScopedProvider));
                return DotNetDupe::System::SmartPointer<IServiceScope>(spScope);
            }
        }
    }
}

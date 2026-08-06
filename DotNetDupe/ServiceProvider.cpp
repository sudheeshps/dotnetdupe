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

            ServiceProvider::ServiceProvider(const IServiceCollection& collection)
                : m_bIsRoot(true), m_pRootProvider(nullptr) {
                for (int iIdx = 0; iIdx < collection.GetCount(); ++iIdx) {
                    const ServiceDescriptor& descriptor = collection[iIdx];
                    m_mapDescriptors.insert({ descriptor.GetServiceType(), descriptor });
                }
            }

            ServiceProvider::ServiceProvider(ServiceProvider* pRootProvider)
                : m_bIsRoot(false), m_pRootProvider(pRootProvider) {
            }

            ServiceProvider::~ServiceProvider() {
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::GetService(const std::type_index& serviceType) {
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_csLock);

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
                auto it = pRoot->m_mapDescriptors.find(serviceType);
                if (it == pRoot->m_mapDescriptors.end()) {
                    return nullptr;
                }

                return ResolveService(it->second);
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::ResolveService(const ServiceDescriptor& descriptor) {
                if (descriptor.GetLifetime() == ServiceLifetime::Singleton) {
                    ServiceProvider* pRoot = m_bIsRoot ? this : m_pRootProvider;
                    DotNetDupe::System::Threading::CriticalSectionLock lock(pRoot->m_csLock);

                    // Check cache
                    auto it = pRoot->m_mapSingletons.find(descriptor.GetServiceType());
                    if (it != pRoot->m_mapSingletons.end()) {
                        return it->second;
                    }

                    // Create
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> spInstance;
                    if (descriptor.GetInstance()) {
                        spInstance = descriptor.GetInstance();
                    } else if (descriptor.GetFactory()) {
                        auto spProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        spInstance = descriptor.GetFactory()(spProxy);
                    }

                    if (!spInstance.IsNull()) {
                        pRoot->m_mapSingletons[descriptor.GetServiceType()] = spInstance;

                        // Track IDisposable
                        auto spDisposable = spInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!spDisposable.IsNull()) {
                            pRoot->m_vDisposables.push_back(spDisposable);
                        }
                    }
                    return spInstance;
                }
                else if (descriptor.GetLifetime() == ServiceLifetime::Scoped) {
                    if (m_bIsRoot) {
                        throw DotNetDupe::System::InvalidOperationException("Cannot resolve scoped service from root provider.");
                    }

                    DotNetDupe::System::Threading::CriticalSectionLock lock(m_csLock);

                    // Check cache
                    auto it = m_mapScoped.find(descriptor.GetServiceType());
                    if (it != m_mapScoped.end()) {
                        return it->second;
                    }

                    // Create
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> spInstance;
                    if (descriptor.GetFactory()) {
                        auto spProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        spInstance = descriptor.GetFactory()(spProxy);
                    }

                    if (!spInstance.IsNull()) {
                        m_mapScoped[descriptor.GetServiceType()] = spInstance;

                        // Track IDisposable
                        auto spDisposable = spInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!spDisposable.IsNull()) {
                            m_vDisposables.push_back(spDisposable);
                        }
                    }
                    return spInstance;
                }
                else {
                    // Transient
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> spInstance;
                    if (descriptor.GetFactory()) {
                        auto spProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                            DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(this)
                        );
                        spInstance = descriptor.GetFactory()(spProxy);
                    }

                    if (!spInstance.IsNull()) {
                        // Track transient disposables in the resolving provider to clean up on Dispose
                        auto spDisposable = spInstance.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                        if (!spDisposable.IsNull()) {
                            DotNetDupe::System::Threading::CriticalSectionLock lock(m_csLock);
                            m_vDisposables.push_back(spDisposable);
                        }
                    }
                    return spInstance;
                }
            }

            void ServiceProvider::Dispose() {
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_csLock);

                // Dispose in reverse order of registration
                for (auto it = m_vDisposables.rbegin(); it != m_vDisposables.rend(); ++it) {
                    if (!it->IsNull()) {
                        (*it)->Dispose();
                    }
                }
                m_vDisposables.clear();

                m_mapSingletons.clear();
                m_mapScoped.clear();
            }

            // --- ServiceScope Implementation ---

            ServiceScope::ServiceScope(DotNetDupe::System::SmartPointer<ServiceProvider> spProvider)
                : m_spProvider(std::move(spProvider)) {}

            ServiceScope::~ServiceScope() {
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> ServiceScope::GetServiceProvider() const {
                return m_spProvider;
            }

            void ServiceScope::Dispose() {
                if (!m_spProvider.IsNull()) {
                    m_spProvider->Dispose();
                    m_spProvider = nullptr;
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

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
            
            static DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> InstantiateService(ServiceProvider* pSelf, const ServiceDescriptor& descriptor) {
                if (descriptor.GetInstance()) return descriptor.GetInstance();
                if (descriptor.GetFactory()) {
                    auto pProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                        DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(pSelf)
                    );
                    return descriptor.GetFactory()(pProxy);
                }
                return nullptr;
            }

            struct ServiceProvider::Impl {
                std::unordered_map<std::type_index, ServiceDescriptor> mapDescriptors;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> pMapSingletons;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> pMapScoped;
                std::vector<DotNetDupe::System::SmartPointer<DotNetDupe::System::IO::IDisposable>> pvDisposables;
                DotNetDupe::System::Threading::CriticalSection csLock;

                void Track(const DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>& pInst) {
                    if (pInst.IsNull()) return;
                    auto pDisp = pInst.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                    if (!pDisp.IsNull()) pvDisposables.push_back(pDisp);
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveSingleton(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);
                    auto it = pMapSingletons.find(desc.GetServiceType());
                    if (it != pMapSingletons.end()) return it->second;
                    auto pInst = InstantiateService(pSelf, desc);
                    if (!pInst.IsNull()) {
                        pMapSingletons[desc.GetServiceType()] = pInst;
                        Track(pInst);
                    }
                    return pInst;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveScoped(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);
                    auto it = pMapScoped.find(desc.GetServiceType());
                    if (it != pMapScoped.end()) return it->second;
                    auto pInst = InstantiateService(pSelf, desc);
                    if (!pInst.IsNull()) {
                        pMapScoped[desc.GetServiceType()] = pInst;
                        Track(pInst);
                    }
                    return pInst;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveTransient(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    auto pInst = InstantiateService(pSelf, desc);
                    if (!pInst.IsNull()) {
                        DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);
                        Track(pInst);
                    }
                    return pInst;
                }
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
                    return pRoot->m_pImpl->ResolveSingleton(this, descriptor);
                }
                if (descriptor.GetLifetime() == ServiceLifetime::Scoped) {
                    if (m_bIsRoot) throw DotNetDupe::System::InvalidOperationException("Cannot resolve scoped service from root provider.");
                    return m_pImpl->ResolveScoped(this, descriptor);
                }
                return m_pImpl->ResolveTransient(this, descriptor);
            }

            void ServiceProvider::Dispose() {
                if (m_pImpl.IsNull()) return;
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                // Dispose in reverse order of registration
                for (auto it = m_pImpl->pvDisposables.rbegin(); it != m_pImpl->pvDisposables.rend(); ++it) {
                    if (!it->IsNull()) {
                        (*it)->Dispose();
                    }
                }
                m_pImpl->pvDisposables.clear();

                m_pImpl->pMapSingletons.clear();
                m_pImpl->pMapScoped.clear();
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

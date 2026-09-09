#include "pch.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
#include "System/Threading/Lock.h"
#include <unordered_map>
#include <vector>
#include <typeindex>

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
                    /// Delegate resolution to the encapsulated provider.
                    return m_pProvider->GetService(serviceType);
                }

            private:
                ServiceProvider* m_pProvider;
            };

            // --- ServiceProvider Implementation ---
            
            static DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> InstantiateService(ServiceProvider* pSelf, const ServiceDescriptor& descriptor) {
                /// Guard: Return pre-instantiated singleton instance if available.
                if (descriptor.GetInstance()) {
                    return descriptor.GetInstance();
                }

                /// Invoke factory delegate with a scoped proxy if registered.
                if (descriptor.GetFactory()) {
                    auto pProxy = DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider>(
                        DotNetDupe::System::SmartPointer<ServiceProviderProxy>::NewShared(pSelf)
                    );
                    return descriptor.GetFactory()(pProxy);
                }

                /// Return null if service cannot be instantiated.
                return nullptr;
            }

            struct ServiceProvider::Impl {
                std::unordered_map<std::type_index, ServiceDescriptor> mapDescriptors;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> pMapSingletons;
                std::unordered_map<std::type_index, DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>> pMapScoped;
                std::vector<DotNetDupe::System::SmartPointer<DotNetDupe::System::IO::IDisposable>> pvDisposables;
                DotNetDupe::System::Threading::CriticalSection csLock;

                void Track(const DotNetDupe::System::SmartPointer<DotNetDupe::System::Object>& pInst) {
                    /// Guard: Ignore null instances.
                    if (pInst.IsNull()) return;

                    /// Cast to IDisposable and record for container shutdown cleanup.
                    auto pDisp = pInst.DynamicCast<DotNetDupe::System::IO::IDisposable>();
                    if (!pDisp.IsNull()) {
                        pvDisposables.push_back(pDisp);
                    }
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveSingleton(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    /// Synchronize concurrent singleton resolution.
                    DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);

                    /// Check if singleton has already been resolved and cached.
                    auto it = pMapSingletons.find(desc.GetServiceType());
                    if (it != pMapSingletons.end()) return it->second;

                    /// Instantiate singleton and register for disposal.
                    auto pInst = InstantiateService(pSelf, desc);
                    if (!pInst.IsNull()) {
                        pMapSingletons[desc.GetServiceType()] = pInst;
                        Track(pInst);
                    }
                    return pInst;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveScoped(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    /// Synchronize concurrent scoped resolution.
                    DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);

                    /// Check if scoped instance has already been created in this scope.
                    auto it = pMapScoped.find(desc.GetServiceType());
                    if (it != pMapScoped.end()) return it->second;

                    /// Instantiate scoped service and track in this scope.
                    auto pInst = InstantiateService(pSelf, desc);
                    if (!pInst.IsNull()) {
                        pMapScoped[desc.GetServiceType()] = pInst;
                        Track(pInst);
                    }
                    return pInst;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ResolveTransient(ServiceProvider* pSelf, const ServiceDescriptor& desc) {
                    /// Instantiate a fresh transient service instance.
                    auto pInst = InstantiateService(pSelf, desc);

                    /// Track instance if disposable.
                    if (!pInst.IsNull()) {
                        DotNetDupe::System::Threading::CriticalSectionLock lock(csLock);
                        Track(pInst);
                    }
                    return pInst;
                }
            };

            ServiceProvider::ServiceProvider(const IServiceCollection& collection)
                : m_bIsRoot(true), m_pRootProvider(nullptr), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Populate descriptor lookup map from collection.
                for (int iIdx = 0; iIdx < collection.GetCount(); ++iIdx) {
                    const ServiceDescriptor& descriptor = collection[iIdx];
                    m_pImpl->mapDescriptors.insert({ descriptor.GetServiceType(), descriptor });
                }
            }

            ServiceProvider::ServiceProvider(ServiceProvider* pRootProvider)
                : m_bIsRoot(false), m_pRootProvider(pRootProvider), m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                /// Initialize child scoped container bound to parent root.
            }

            ServiceProvider::~ServiceProvider() {
                /// Ensure all resolved disposable services are cleaned up.
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::GetService(const std::type_index& serviceType) {
                /// Acquire synchronization lock for container resolution.
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                /// Resolve built-in container interfaces.
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

                /// Locate service descriptor in registry.
                ServiceProvider* pRoot = m_bIsRoot ? this : m_pRootProvider;
                auto it = pRoot->m_pImpl->mapDescriptors.find(serviceType);
                if (it == pRoot->m_pImpl->mapDescriptors.end()) {
                    return nullptr;
                }

                /// Resolve instance according to descriptor lifetime.
                return ResolveService(it->second);
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::Object> ServiceProvider::ResolveService(const ServiceDescriptor& descriptor) {
                /// Delegate singleton resolution to root container.
                if (descriptor.GetLifetime() == ServiceLifetime::Singleton) {
                    ServiceProvider* pRoot = m_bIsRoot ? this : m_pRootProvider;
                    return pRoot->m_pImpl->ResolveSingleton(this, descriptor);
                }

                /// Resolve scoped service from local container scope.
                if (descriptor.GetLifetime() == ServiceLifetime::Scoped) {
                    if (m_bIsRoot) {
                        throw DotNetDupe::System::InvalidOperationException("Cannot resolve scoped service from root provider.");
                    }
                    return m_pImpl->ResolveScoped(this, descriptor);
                }

                /// Resolve transient service on each request.
                return m_pImpl->ResolveTransient(this, descriptor);
            }

            void ServiceProvider::Dispose() {
                /// Guard: Check if implementation is already disposed or null.
                if (m_pImpl.IsNull()) return;
                DotNetDupe::System::Threading::CriticalSectionLock lock(m_pImpl->csLock);

                /// Dispose tracked services in reverse order of creation.
                for (auto it = m_pImpl->pvDisposables.rbegin(); it != m_pImpl->pvDisposables.rend(); ++it) {
                    if (!it->IsNull()) {
                        (*it)->Dispose();
                    }
                }
                m_pImpl->pvDisposables.clear();

                /// Clear cached singleton and scoped instances.
                m_pImpl->pMapSingletons.clear();
                m_pImpl->pMapScoped.clear();
            }

            // --- ServiceScope Implementation ---

            ServiceScope::ServiceScope(DotNetDupe::System::SmartPointer<ServiceProvider> pProvider)
                : m_pProvider(std::move(pProvider)) {
                /// Initialize scope with child service provider.
            }

            ServiceScope::~ServiceScope() {
                /// Dispose child scoped services on scope destruction.
                Dispose();
            }

            DotNetDupe::System::SmartPointer<DotNetDupe::System::IServiceProvider> ServiceScope::GetServiceProvider() const {
                /// Return the scoped container provider.
                return m_pProvider;
            }

            void ServiceScope::Dispose() {
                /// Dispose child container and release provider reference.
                if (!m_pProvider.IsNull()) {
                    m_pProvider->Dispose();
                    m_pProvider = nullptr;
                }
            }

            // --- ServiceScopeFactory Implementation ---

            ServiceScopeFactory::ServiceScopeFactory(ServiceProvider* pProvider)
                : m_pProvider(pProvider) {
                /// Bind factory to root provider.
            }

            DotNetDupe::System::SmartPointer<IServiceScope> ServiceScopeFactory::CreateScope() {
                /// Instantiate child scoped provider inheriting parent descriptors.
                auto spScopedProvider = DotNetDupe::System::SmartPointer<ServiceProvider>::NewShared(m_pProvider);

                /// Wrap scoped provider in a ServiceScope.
                auto spScope = DotNetDupe::System::SmartPointer<ServiceScope>::NewShared(std::move(spScopedProvider));
                return DotNetDupe::System::SmartPointer<IServiceScope>(spScope);
            }
        }
    }
}

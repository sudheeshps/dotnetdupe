#include "pch.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/InvalidOperationException.h"
#include "System/ArgumentException.h"
#include <mutex>
#include <unordered_map>
#include <memory>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                struct DatabaseEngine::Impl {
                    std::unordered_map<std::string, DotNetDupe::System::SmartPointer<IDatabaseBackend>> m_pBackends;
                    std::mutex m_mutex;
                };

                DatabaseEngine::DatabaseEngine() : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                }

                DatabaseEngine::~DatabaseEngine() = default;

                DatabaseEngine& DatabaseEngine::Instance() {
                    /// Step: Maintain thread-safe static singleton instance.
                    static DatabaseEngine instance;
                    return instance;
                }

                void DatabaseEngine::RegisterBackend(const DotNetDupe::System::String& dbName, DotNetDupe::System::SmartPointer<IDatabaseBackend> backend) {
                    /// Step: Synchronize access to backend map.
                    std::lock_guard<std::mutex> lock(m_pImpl->m_mutex);

                    /// Step: Store backend reference keyed by database name.
                    m_pImpl->m_pBackends[dbName.GetRawString()] = backend;
                }

                DotNetDupe::System::SmartPointer<IDatabaseBackend> DatabaseEngine::GetBackend(const DotNetDupe::System::String& dbName) {
                    /// Step: Synchronize access to backend registry.
                    std::lock_guard<std::mutex> lock(m_pImpl->m_mutex);

                    /// Step: Look up backend by name.
                    auto it = m_pImpl->m_pBackends.find(dbName.GetRawString());
                    if (it != m_pImpl->m_pBackends.end()) {
                        return it->second;
                    }

                    /// Step: Return null pointer if backend is unregistered.
                    return DotNetDupe::System::SmartPointer<IDatabaseBackend>(nullptr);
                }

                void DatabaseEngine::ClearDatabase(const DotNetDupe::System::String& dbName) {
                    /// Step: Resolve backend by name.
                    auto spBackend = GetBackend(dbName);

                    /// Step: Invoke backend-specific purge if registered.
                    if (spBackend) {
                        spBackend->ClearDatabase();
                    }
                }

                Collections::Generic::List<Row> DatabaseEngine::Execute(
                    const DotNetDupe::System::String& dbName,
                    const String& sql,
                    const Collections::Generic::Dictionary<String, String>& parameters,
                    Collections::Generic::List<String>& columnNames,
                    int& rowsAffected
                ) {
                    /// Step: Locate target database backend.
                    auto spBackend = GetBackend(dbName);
                    if (spBackend) {
                        return spBackend->Execute(sql, parameters, columnNames, rowsAffected);
                    }

                    /// Step: Reset outputs and return empty list on missing backend.
                    rowsAffected = 0;
                    columnNames.Clear();
                    return Collections::Generic::List<Row>();
                }

            }
        }
    }
}

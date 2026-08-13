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
                    std::unordered_map<std::string, DotNetDupe::System::SmartPointer<IDatabaseBackend>> m_backends;
                    std::mutex m_mutex;
                };

                DatabaseEngine::DatabaseEngine() : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                }

                DatabaseEngine::~DatabaseEngine() = default;

                DatabaseEngine& DatabaseEngine::Instance() {
                    static DatabaseEngine instance;
                    return instance;
                }

                void DatabaseEngine::RegisterBackend(const DotNetDupe::System::String& dbName, DotNetDupe::System::SmartPointer<IDatabaseBackend> backend) {
                    std::lock_guard<std::mutex> lock(m_pImpl->m_mutex);
                    m_pImpl->m_backends[dbName.GetRawString()] = backend;
                }

                DotNetDupe::System::SmartPointer<IDatabaseBackend> DatabaseEngine::GetBackend(const DotNetDupe::System::String& dbName) {
                    std::lock_guard<std::mutex> lock(m_pImpl->m_mutex);
                    auto it = m_pImpl->m_backends.find(dbName.GetRawString());
                    if (it != m_pImpl->m_backends.end()) {
                        return it->second;
                    }
                    return DotNetDupe::System::SmartPointer<IDatabaseBackend>(nullptr);
                }

                void DatabaseEngine::ClearDatabase(const DotNetDupe::System::String& dbName) {
                    auto backend = GetBackend(dbName);
                    if (backend) {
                        backend->ClearDatabase();
                    }
                }

                Collections::Generic::List<Row> DatabaseEngine::Execute(
                    const DotNetDupe::System::String& dbName,
                    const String& sql,
                    const Collections::Generic::Dictionary<String, String>& parameters,
                    Collections::Generic::List<String>& columnNames,
                    int& rowsAffected
                ) {
                    auto backend = GetBackend(dbName);
                    if (backend) {
                        return backend->Execute(sql, parameters, columnNames, rowsAffected);
                    }
                    rowsAffected = 0;
                    columnNames.Clear();
                    return Collections::Generic::List<Row>();
                }

            }
        }
    }
}

#include "pch.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/Data/Internal/InMemoryDatabaseBackend.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                DatabaseEngine& DatabaseEngine::Instance() {
                    static DatabaseEngine instance;
                    return instance;
                }

                void DatabaseEngine::RegisterBackend(const std::string& dbName, std::shared_ptr<IDatabaseBackend> backend) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_backends[dbName] = backend;
                }

                std::shared_ptr<IDatabaseBackend> DatabaseEngine::GetBackend(const std::string& dbName) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    auto it = m_backends.find(dbName);
                    if (it != m_backends.end()) {
                        return it->second;
                    }
                    // Default fallback to InMemory if no backend registered
                    auto defaultBackend = std::make_shared<InMemoryDatabaseBackend>();
                    m_backends[dbName] = defaultBackend;
                    return defaultBackend;
                }

                void DatabaseEngine::ClearDatabase(const std::string& dbName) {
                    auto backend = GetBackend(dbName);
                    if (backend) {
                        backend->ClearDatabase();
                    }
                }

                std::vector<Row> DatabaseEngine::Execute(
                    const std::string& dbName,
                    const std::string& sql,
                    const std::unordered_map<std::string, std::string>& parameters,
                    std::vector<std::string>& columnNames,
                    int& rowsAffected
                ) {
                    auto backend = GetBackend(dbName);
                    if (backend) {
                        return backend->Execute(sql, parameters, columnNames, rowsAffected);
                    }
                    rowsAffected = 0;
                    columnNames.clear();
                    return {};
                }

            }
        }
    }
}

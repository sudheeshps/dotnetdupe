#pragma once
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include <mutex>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {
                struct DatabaseEngine::Impl {
                    std::unordered_map<std::string, std::shared_ptr<IDatabaseBackend>> m_backends;
                    std::mutex m_mutex;

                    void RegisterBackend(const std::string& dbName, std::shared_ptr<IDatabaseBackend> backend);
                    std::shared_ptr<IDatabaseBackend> GetBackend(const std::string& dbName);
                    std::vector<Row> Execute(
                        const std::string& dbName,
                        const std::string& sql,
                        const std::unordered_map<std::string, std::string>& parameters,
                        std::vector<std::string>& columnNames,
                        int& rowsAffected
                    );
                };
            }
        }
    }
}

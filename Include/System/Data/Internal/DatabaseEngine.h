#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                class DatabaseEngine {
                private:
                    std::unordered_map<std::string, std::shared_ptr<IDatabaseBackend>> m_backends;
                    std::mutex m_mutex;

                    DatabaseEngine() = default;

                public:
                    DOTNETDUPE_API static DatabaseEngine& Instance();

                    DOTNETDUPE_API void RegisterBackend(const std::string& dbName, std::shared_ptr<IDatabaseBackend> backend);

                    DOTNETDUPE_API std::shared_ptr<IDatabaseBackend> GetBackend(const std::string& dbName);

                    DOTNETDUPE_API void ClearDatabase(const std::string& dbName);

                    DOTNETDUPE_API std::vector<Row> Execute(
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

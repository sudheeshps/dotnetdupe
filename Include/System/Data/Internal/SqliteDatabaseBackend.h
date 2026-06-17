#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include <string>
#include <vector>
#include <unordered_map>

#if defined(DOTNETDUPE_USE_SQLITE)
#include <sqlite3.h>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                class SqliteDatabaseBackend : public IDatabaseBackend {
                private:
                    sqlite3* m_db = nullptr;
                    std::string m_connectionString;

                public:
                    DOTNETDUPE_API SqliteDatabaseBackend(const std::string& connStr);
                    DOTNETDUPE_API ~SqliteDatabaseBackend() override;

                    DOTNETDUPE_API void ClearDatabase() override;

                    DOTNETDUPE_API std::vector<Row> Execute(
                        const std::string& sql,
                        const std::unordered_map<std::string, std::string>& parameters,
                        std::vector<std::string>& columnNames,
                        int& rowsAffected
                    ) override;
                };

            }
        }
    }
}
#endif

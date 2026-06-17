#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                class InMemoryDatabaseBackend : public IDatabaseBackend {
                private:
                    std::unordered_map<std::string, Table> m_tables;

                    // Private helper methods for CRUD operations
                    void ExecuteCreate(const std::vector<std::string>& tokens);
                    void ExecuteInsert(const std::vector<std::string>& tokens,
                                       const std::unordered_map<std::string, std::string>& parameters,
                                       int& rowsAffected);
                    std::vector<Row> ExecuteSelect(const std::vector<std::string>& tokens,
                                                   const std::unordered_map<std::string, std::string>& parameters,
                                                   std::vector<std::string>& columnNames);
                    void ExecuteUpdate(const std::vector<std::string>& tokens,
                                       const std::unordered_map<std::string, std::string>& parameters,
                                       int& rowsAffected);
                    void ExecuteDelete(const std::vector<std::string>& tokens,
                                       const std::unordered_map<std::string, std::string>& parameters,
                                       int& rowsAffected);

                    // Utility methods
                    std::vector<std::string> Tokenize(const std::string& sql) const;
                    std::string ToUpper(std::string s) const;
                    std::string ResolveValue(const std::string& val, const std::unordered_map<std::string, std::string>& parameters) const;

                public:
                    DOTNETDUPE_API InMemoryDatabaseBackend();
                    DOTNETDUPE_API ~InMemoryDatabaseBackend() override;

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

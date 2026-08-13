#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                class DatabaseEngine {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                    DatabaseEngine();
                    ~DatabaseEngine();

                public:
                    DOTNETDUPE_API static DatabaseEngine& Instance();

                    DOTNETDUPE_API void RegisterBackend(const DotNetDupe::System::String& dbName, DotNetDupe::System::SmartPointer<IDatabaseBackend> backend);

                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<IDatabaseBackend> GetBackend(const DotNetDupe::System::String& dbName);

                    DOTNETDUPE_API void ClearDatabase(const DotNetDupe::System::String& dbName);

                    // Re-adding Execute using std::vector since we decided to leave Internal engines as is.
                    DOTNETDUPE_API std::vector<Row> Execute(
                        const DotNetDupe::System::String& dbName,
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

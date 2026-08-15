#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"

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

                    DOTNETDUPE_API Collections::Generic::List<Row> Execute(
                        const DotNetDupe::System::String& dbName,
                        const String& sql,
                        const Collections::Generic::Dictionary<String, String>& parameters,
                        Collections::Generic::List<String>& columnNames,
                        int& rowsAffected
                    );
                };

            }
        }
    }
}

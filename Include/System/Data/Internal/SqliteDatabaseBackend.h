#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/SmartPointer.h"

#if defined(DOTNETDUPE_USE_SQLITE)

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                class SqliteDatabaseBackend : public IDatabaseBackend {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    DOTNETDUPE_API SqliteDatabaseBackend(const DotNetDupe::System::String& connStr);
                    DOTNETDUPE_API ~SqliteDatabaseBackend() override;

                    DOTNETDUPE_API void ClearDatabase() override;

                    DOTNETDUPE_API Collections::Generic::List<Row> Execute(
                        const String& sql,
                        const Collections::Generic::Dictionary<String, String>& parameters,
                        Collections::Generic::List<String>& columnNames,
                        int& rowsAffected
                    ) override;
                };

            }
        }
    }
}
#endif

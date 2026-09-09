#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                /// \class InMemoryDatabaseBackend
                /// \brief In-memory volatile database backend supporting basic relational DDL and DML operations.
                ///
                /// Provides SQL parsing, execution, and table storage entirely in RAM for unit testing
                /// and lightweight mock data persistence without disk I/O overhead.
                class InMemoryDatabaseBackend : public IDatabaseBackend {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Initializes a new instance of InMemoryDatabaseBackend.
                    DOTNETDUPE_API InMemoryDatabaseBackend();

                    /// \brief Destructor releasing internal in-memory tables.
                    DOTNETDUPE_API ~InMemoryDatabaseBackend() override;

                    /// \brief Clears all tables and rows in the in-memory database.
                    DOTNETDUPE_API void ClearDatabase() override;

                    /// \brief Executes a SQL statement (CREATE, INSERT, SELECT, UPDATE, DELETE) in memory.
                    /// \param sql The SQL statement text.
                    /// \param parameters Dictionary of query parameters.
                    /// \param columnNames Output list filled with selected column names.
                    /// \param rowsAffected Output integer tracking the number of affected rows.
                    /// \return List of rows matching the query.
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

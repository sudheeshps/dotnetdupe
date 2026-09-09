#pragma once
#include "Common.h"
#include "System/Data/Internal/IDatabaseBackend.h"
#include "System/SmartPointer.h"

#if defined(DOTNETDUPE_USE_SQLITE)

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                /// \class SqliteDatabaseBackend
                /// \brief SQLite storage backend implementing the IDatabaseBackend contract.
                ///
                /// Interfaces directly with native `sqlite3` C APIs to execute queries, prepare statements,
                /// bind parameters, and stream result sets against persistent or `:memory:` SQLite databases.
                class SqliteDatabaseBackend : public IDatabaseBackend {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Initializes a new instance with the specified connection string.
                    /// \param connStr The SQLite connection string containing "Data Source=...".
                    /// \throws DotNetDupe::System::InvalidOperationException If opening SQLite file fails.
                    DOTNETDUPE_API SqliteDatabaseBackend(const DotNetDupe::System::String& connStr);

                    /// \brief Destructor closing open SQLite database handles.
                    DOTNETDUPE_API ~SqliteDatabaseBackend() override;

                    /// \brief Clears all tables in the SQLite database.
                    DOTNETDUPE_API void ClearDatabase() override;

                    /// \brief Prepares and executes SQL queries through sqlite3 statement APIs.
                    /// \param sql The SQL statement text.
                    /// \param parameters Dictionary of query parameters to bind.
                    /// \param columnNames Output list populated with returned column names.
                    /// \param rowsAffected Output integer tracking modified rows.
                    /// \return List of retrieved rows.
                    /// \throws DotNetDupe::System::InvalidOperationException If SQLite execution encounters errors.
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

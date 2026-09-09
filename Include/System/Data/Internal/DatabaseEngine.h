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

                /// \class DatabaseEngine
                /// \brief Central registry and dispatcher for named database backends.
                ///
                /// Manages registration and routing of database operations to active backends
                /// (e.g. in-memory or SQLite) based on connection string database names.
                ///
                /// \note Thread Safety: Thread-safe. Internal registry lookup is guarded by synchronization mutex.
                class DatabaseEngine {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                    DatabaseEngine();
                    ~DatabaseEngine();

                public:
                    /// \brief Retrieves the singleton instance of the DatabaseEngine.
                    /// \return Reference to the global DatabaseEngine.
                    DOTNETDUPE_API static DatabaseEngine& Instance();

                    /// \brief Registers a backend instance for a specific database name.
                    /// \param dbName The database identifier or connection name.
                    /// \param backend Smart pointer to the IDatabaseBackend implementation.
                    DOTNETDUPE_API void RegisterBackend(const DotNetDupe::System::String& dbName, DotNetDupe::System::SmartPointer<IDatabaseBackend> backend);

                    /// \brief Retrieves the backend associated with a database name.
                    /// \param dbName The database identifier.
                    /// \return Smart pointer to the backend, or nullptr if not found.
                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<IDatabaseBackend> GetBackend(const DotNetDupe::System::String& dbName);

                    /// \brief Clears all contents of the specified database.
                    /// \param dbName The database identifier to clear.
                    DOTNETDUPE_API void ClearDatabase(const DotNetDupe::System::String& dbName);

                    /// \brief Executes a SQL command against the named database backend.
                    /// \param dbName The database identifier.
                    /// \param sql The SQL statement to execute.
                    /// \param parameters Dictionary of query parameters.
                    /// \param columnNames Output list populated with column header names.
                    /// \param rowsAffected Output count of modified rows.
                    /// \return List of rows returned by query execution.
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

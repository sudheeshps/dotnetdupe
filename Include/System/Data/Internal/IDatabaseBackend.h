#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                /// \struct Row
                /// \brief Represents an internal data row consisting of a list of column values.
                struct Row {
                    Collections::Generic::List<String> Values; ///< Ordered list of column values as strings.
                };

                /// \struct Table
                /// \brief Represents an internal in-memory relational table.
                struct Table {
                    Collections::Generic::List<String> Columns; ///< Ordered list of column header names.
                    Collections::Generic::List<Row> Rows;       ///< List of data rows contained within the table.
                };

                /// \class IDatabaseBackend
                /// \brief Interface defining the contract for database storage backends in DotNetDupe.
                ///
                /// Implementations include in-memory volatile storage and SQLite persistent storage.
                class IDatabaseBackend {
                public:
                    /// \brief Virtual destructor for polymorphic cleanup.
                    virtual ~IDatabaseBackend() = default;

                    /// \brief Executes a SQL query or command against the database backend.
                    /// \param sql The SQL statement text to execute.
                    /// \param parameters The dictionary of parameter key-value pairs.
                    /// \param columnNames Output list populated with returned column names.
                    /// \param rowsAffected Output integer containing the count of affected rows.
                    /// \return List of rows returned by query execution.
                    virtual Collections::Generic::List<Row> Execute(
                        const String& sql,
                        const Collections::Generic::Dictionary<String, String>& parameters,
                        Collections::Generic::List<String>& columnNames,
                        int& rowsAffected
                    ) = 0;

                    /// \brief Clears all tables and records in the backend database.
                    virtual void ClearDatabase() = 0;
                };

            }
        }
    }
}

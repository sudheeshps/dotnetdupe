#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Data/Common/DbDataReader.h"
#include "System/Data/Common/DbParameter.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                /// \class DbCommand
                /// \brief Represents an abstract SQL statement or stored procedure to execute against a data source.
                ///
                /// Modeled after .NET System.Data.Common.DbCommand and adhering to the ADO.NET specification.
                /// DbCommand serves as the base abstraction for database commands across in-memory, SQLite,
                /// and relational database providers.
                ///
                /// \note Thread Safety: DbCommand instances are generally not guaranteed to be thread-safe.
                /// Concurrent execution of commands on the same instance must be synchronized externally.
                class DbCommand : public virtual DotNetDupe::System::Object {
                public:
                    /// \brief Initializes a new instance of the DbCommand class.
                    DbCommand() = default;

                    /// \brief Virtual destructor for polymorphic cleanup.
                    ~DbCommand() override = default;

                    /// \brief Gets the text command to run against the data source.
                    /// \return The SQL statement or stored procedure name to execute.
                    virtual DotNetDupe::System::String GetCommandText() const = 0;

                    /// \brief Sets the text command to run against the data source.
                    /// \param sText The SQL statement or stored procedure name to execute.
                    virtual void SetCommandText(const DotNetDupe::System::String& sText) = 0;

                    /// \brief Gets the collection of parameters associated with the command.
                    /// \return A smart pointer to the DbParameterCollection.
                    virtual DotNetDupe::System::SmartPointer<DbParameterCollection> GetParameters() const = 0;

                    /// \brief Executes the command text against the connection and returns a forward-only reader.
                    /// \return A smart pointer to a DbDataReader object.
                    /// \throws DotNetDupe::System::InvalidOperationException If connection is closed or SQL syntax error occurs.
                    virtual DotNetDupe::System::SmartPointer<DbDataReader> ExecuteReader() = 0;

                    /// \brief Executes a SQL statement against the connection and returns the number of rows affected.
                    /// \return The number of rows affected by INSERT, UPDATE, or DELETE operations.
                    /// \throws DotNetDupe::System::InvalidOperationException If execution fails.
                    virtual int ExecuteNonQuery() = 0;

                    /// \brief Executes the query and returns the first column of the first row in the result set.
                    /// \return The value of the first column of the first row formatted as a string.
                    /// \throws DotNetDupe::System::InvalidOperationException If execution fails.
                    virtual DotNetDupe::System::String ExecuteScalar() = 0;
                };

            }
        }
    }
}

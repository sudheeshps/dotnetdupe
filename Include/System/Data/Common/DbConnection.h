#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Data/Common/DbCommand.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                /// \class DbConnection
                /// \brief Represents an abstract connection to an ADO.NET compatible data source.
                ///
                /// Modeled after .NET System.Data.Common.DbConnection. Provides a standard abstraction
                /// for managing database connection states, connection string configuration, and
                /// instantiating provider-specific command objects.
                ///
                /// \note Thread Safety: Connection objects are not thread-safe. Concurrent operations
                /// must be executed across separate connection instances or guarded by locks.
                class DbConnection : public virtual DotNetDupe::System::Object {
                public:
                    /// \brief Initializes a new instance of the DbConnection class.
                    DbConnection() = default;

                    /// \brief Virtual destructor ensuring clean disposal of connection resources.
                    ~DbConnection() override = default;

                    /// \brief Gets the string used to open a connection to the data source.
                    /// \return The connection string settings.
                    virtual DotNetDupe::System::String GetConnectionString() const = 0;

                    /// \brief Sets the string used to open a connection to the data source.
                    /// \param sConnStr The connection string settings to configure.
                    virtual void SetConnectionString(const DotNetDupe::System::String& sConnStr) = 0;

                    /// \brief Opens a database connection with the settings specified by the ConnectionString.
                    /// \throws DotNetDupe::System::InvalidOperationException If connection fails to open.
                    virtual void Open() = 0;

                    /// \brief Closes the connection to the data source and releases internal resources.
                    virtual void Close() = 0;

                    /// \brief Creates and returns a DbCommand object associated with the current connection.
                    /// \return A smart pointer to an initialized DbCommand instance.
                    virtual DotNetDupe::System::SmartPointer<DbCommand> CreateCommand() = 0;
                };

            }
        }
    }
}

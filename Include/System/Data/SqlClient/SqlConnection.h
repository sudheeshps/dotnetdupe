#pragma once
#include "Common.h"
#include "System/Data/Common/DbConnection.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                /// \class SqlConnection
                /// \brief Represents a connection to a SQL database.
                ///
                /// \details Complies with ADO.NET DbConnection provider patterns and ISO/IEC 9075 SQL
                /// connection semantics. Manages connection string parsing, database backend selection
                /// (in-memory or SQLite), and command factory creation via CreateCommand.
                class SqlConnection : public DotNetDupe::System::Data::Common::DbConnection {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Initializes a new instance of the SqlConnection class.
                    DOTNETDUPE_API SqlConnection();

                    /// \brief Initializes a new instance of the SqlConnection class given a string that contains the connection string.
                    /// \param sConnectionString The connection used to open the SQL database.
                    DOTNETDUPE_API SqlConnection(const DotNetDupe::System::String& sConnectionString);

                    /// \brief Destructor ensuring connection closure.
                    DOTNETDUPE_API ~SqlConnection() override;

                    /// \brief Gets the string used to open a SQL database.
                    /// \return The connection string that includes parameters such as server name and database name.
                    DOTNETDUPE_API DotNetDupe::System::String GetConnectionString() const override;

                    /// \brief Sets the string used to open a SQL database.
                    /// \param sConnStr The connection string.
                    DOTNETDUPE_API void SetConnectionString(const DotNetDupe::System::String& sConnStr) override;

                    /// \brief Gets the name of the current database.
                    /// \return The name of the database.
                    DOTNETDUPE_API DotNetDupe::System::String GetDatabaseName() const;

                    /// \brief Gets whether the connection is currently open.
                    /// \return true if open; otherwise, false.
                    DOTNETDUPE_API bool IsOpen() const;

                    /// \brief Opens a database connection with the property settings specified by the ConnectionString.
                    DOTNETDUPE_API void Open() override;

                    /// \brief Closes the connection to the database.
                    DOTNETDUPE_API void Close() override;

                    /// \brief Creates and returns a SqlCommand object associated with the SqlConnection.
                    /// \return A SqlCommand object.
                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> CreateCommand() override;
                };

            }
        }
    }
}

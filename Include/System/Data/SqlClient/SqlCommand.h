#pragma once
#include "Common.h"
#include "System/Data/Common/DbCommand.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection;

                /// \class SqlCommand
                /// \brief Represents a Transact-SQL statement or stored procedure to execute against a SQL database.
                ///
                /// \details Complies with ADO.NET DbCommand specifications and ISO/IEC 9075 SQL standard.
                /// Manages parameter binding, statement execution (ExecuteNonQuery, ExecuteReader, ExecuteScalar),
                /// and result set dispatch.
                class SqlCommand : public DotNetDupe::System::Data::Common::DbCommand {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Initializes a new instance of the SqlCommand class.
                    DOTNETDUPE_API SqlCommand();

                    /// \brief Initializes a new instance of the SqlCommand class with the text of the query and a SqlConnection.
                    /// \param sText The text of the query.
                    /// \param connection A SqlConnection that represents the connection to an instance of SQL database.
                    DOTNETDUPE_API SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection);

                    /// \brief Destructor releasing command resources.
                    DOTNETDUPE_API ~SqlCommand() override;

                    /// \brief Gets the Transact-SQL statement or stored procedure to execute at the data source.
                    /// \return The Transact-SQL statement or stored procedure to execute.
                    DOTNETDUPE_API DotNetDupe::System::String GetCommandText() const override;

                    /// \brief Sets the Transact-SQL statement or stored procedure to execute at the data source.
                    /// \param sText The Transact-SQL statement to execute.
                    DOTNETDUPE_API void SetCommandText(const DotNetDupe::System::String& sText) override;

                    /// \brief Gets the SqlParameterCollection.
                    /// \return The parameters of the Transact-SQL statement or stored procedure.
                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> GetParameters() const override;

                    /// \brief Sends the CommandText to the Connection and builds a SqlDataReader.
                    /// \return A SqlDataReader object.
                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> ExecuteReader() override;

                    /// \brief Executes a Transact-SQL statement against the connection and returns the number of rows affected.
                    /// \return The number of rows affected.
                    DOTNETDUPE_API int ExecuteNonQuery() override;

                    /// \brief Executes the query, and returns the first column of the first row in the result set returned by the query.
                    /// \return The first column of the first row in the result set, or empty string if result set is empty.
                    DOTNETDUPE_API DotNetDupe::System::String ExecuteScalar() override;
                };

            }
        }
    }
}

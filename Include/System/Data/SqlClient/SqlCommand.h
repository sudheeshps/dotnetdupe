#pragma once
#include "System/Data/Common/DbCommand.h"
#include "System/Data/Internal/DatabaseEngine.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection;

                class SqlCommand : public DotNetDupe::System::Data::Common::DbCommand {
                private:
                    DotNetDupe::System::String m_sCommandText;
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> m_parameters;
                    SqlConnection* m_connection = nullptr;

                public:
                    SqlCommand() {
                        m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                    }
                    SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection)
                        : m_sCommandText(sText), m_connection(connection) {
                        m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                    }
                    ~SqlCommand() override = default;

                    DotNetDupe::System::String GetCommandText() const override { return m_sCommandText; }
                    void SetCommandText(const DotNetDupe::System::String& sText) override { m_sCommandText = sText; }

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> GetParameters() const override { return m_parameters; }

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> ExecuteReader() override;
                    int ExecuteNonQuery() override;
                    DotNetDupe::System::String ExecuteScalar() override;
                };

            }
        }
    }
}

#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlDataReader.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                inline DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> SqlCommand::ExecuteReader() {
                    std::string dbName = (m_connection == nullptr) ? "DefaultDb" : m_connection->GetDatabaseName();
                    
                    std::unordered_map<std::string, std::string> params;
                    for (int i = 0; i < m_parameters->GetCount(); ++i) {
                        auto p = m_parameters->GetAt(i);
                        params[p->GetParameterName().GetRawString()] = p->GetValue().GetRawString();
                    }

                    std::vector<std::string> columns;
                    int rowsAffected = 0;
                    auto resultRows = DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().Execute(
                        dbName,
                        m_sCommandText.GetRawString(),
                        params,
                        columns,
                        rowsAffected
                    );

                    return DotNetDupe::System::SmartPointer<SqlDataReader>::NewShared(resultRows, columns);
                }

                inline int SqlCommand::ExecuteNonQuery() {
                    std::string dbName = (m_connection == nullptr) ? "DefaultDb" : m_connection->GetDatabaseName();

                    std::unordered_map<std::string, std::string> params;
                    for (int i = 0; i < m_parameters->GetCount(); ++i) {
                        auto p = m_parameters->GetAt(i);
                        params[p->GetParameterName().GetRawString()] = p->GetValue().GetRawString();
                    }

                    std::vector<std::string> columns;
                    int rowsAffected = 0;
                    DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().Execute(
                        dbName,
                        m_sCommandText.GetRawString(),
                        params,
                        columns,
                        rowsAffected
                    );

                    return rowsAffected;
                }

                inline DotNetDupe::System::String SqlCommand::ExecuteScalar() {
                    auto reader = ExecuteReader();
                    if (reader->Read()) {
                        return reader->GetString(0);
                    }
                    return "";
                }

            }
        }
    }
}

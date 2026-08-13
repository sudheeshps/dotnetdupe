#include "pch.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Data/Internal/DatabaseEngine.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                SqlCommand::SqlCommand() {
                    m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                }

                SqlCommand::SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection)
                    : m_sCommandText(sText), m_connection(connection) {
                    m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                }

                DotNetDupe::System::String SqlCommand::GetCommandText() const {
                    return m_sCommandText;
                }

                void SqlCommand::SetCommandText(const DotNetDupe::System::String& sText) {
                    m_sCommandText = sText;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> SqlCommand::GetParameters() const {
                    return m_parameters;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> SqlCommand::ExecuteReader() {
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

                    return DotNetDupe::System::SmartPointer<SqlDataReader>::NewShared((void*)&resultRows, (void*)&columns);
                }

                int SqlCommand::ExecuteNonQuery() {
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

                DotNetDupe::System::String SqlCommand::ExecuteScalar() {
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

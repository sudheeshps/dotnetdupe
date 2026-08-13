#include "pch.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Data/Internal/DatabaseEngine.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                struct SqlCommand::Impl {
                    DotNetDupe::System::String m_sCommandText;
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> m_parameters;
                    SqlConnection* m_connection = nullptr;
                };

                SqlCommand::SqlCommand() : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                    m_pImpl->m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                }

                SqlCommand::SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection)
                    : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                    m_pImpl->m_sCommandText = sText;
                    m_pImpl->m_connection = connection;
                    m_pImpl->m_parameters = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection>::NewShared();
                }

                SqlCommand::~SqlCommand() = default;

                DotNetDupe::System::String SqlCommand::GetCommandText() const {
                    return m_pImpl->m_sCommandText;
                }

                void SqlCommand::SetCommandText(const DotNetDupe::System::String& sText) {
                    m_pImpl->m_sCommandText = sText;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> SqlCommand::GetParameters() const {
                    return m_pImpl->m_parameters;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> SqlCommand::ExecuteReader() {
                    DotNetDupe::System::String dbName = (m_pImpl->m_connection == nullptr) ? DotNetDupe::System::String("DefaultDb") : m_pImpl->m_connection->GetDatabaseName();
                    
                    std::unordered_map<std::string, std::string> params;
                    for (int i = 0; i < m_pImpl->m_parameters->GetCount(); ++i) {
                        auto p = m_pImpl->m_parameters->GetAt(i);
                        params[p->GetParameterName().GetRawString()] = p->GetValue().GetRawString();
                    }

                    std::vector<std::string> columns;
                    int rowsAffected = 0;
                    auto resultRows = DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().Execute(
                        dbName,
                        m_pImpl->m_sCommandText.GetRawString(),
                        params,
                        columns,
                        rowsAffected
                    );

                    return DotNetDupe::System::SmartPointer<SqlDataReader>::NewShared((void*)&resultRows, (void*)&columns);
                }

                int SqlCommand::ExecuteNonQuery() {
                    DotNetDupe::System::String dbName = (m_pImpl->m_connection == nullptr) ? DotNetDupe::System::String("DefaultDb") : m_pImpl->m_connection->GetDatabaseName();

                    std::unordered_map<std::string, std::string> params;
                    for (int i = 0; i < m_pImpl->m_parameters->GetCount(); ++i) {
                        auto p = m_pImpl->m_parameters->GetAt(i);
                        params[p->GetParameterName().GetRawString()] = p->GetValue().GetRawString();
                    }

                    std::vector<std::string> columns;
                    int rowsAffected = 0;
                    DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().Execute(
                        dbName,
                        m_pImpl->m_sCommandText.GetRawString(),
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

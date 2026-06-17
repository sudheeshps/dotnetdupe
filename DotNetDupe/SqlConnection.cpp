#include "pch.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                SqlConnection::SqlConnection(const DotNetDupe::System::String& sConnectionString) {
                    SetConnectionString(sConnectionString);
                }

                DotNetDupe::System::String SqlConnection::GetConnectionString() const {
                    return m_sConnectionString;
                }

                void SqlConnection::SetConnectionString(const DotNetDupe::System::String& sConnStr) {
                    m_sConnectionString = sConnStr;
                    std::string raw = sConnStr.GetRawString();
                    size_t dbIdx = raw.find("Database=");
                    if (dbIdx == std::string::npos) {
                        dbIdx = raw.find("Data Source=");
                        if (dbIdx != std::string::npos) {
                            dbIdx += 12;
                        }
                    } else {
                        dbIdx += 9;
                    }

                    if (dbIdx != std::string::npos) {
                        size_t endIdx = raw.find(";", dbIdx);
                        if (endIdx != std::string::npos) {
                            m_dbName = raw.substr(dbIdx, endIdx - dbIdx);
                        } else {
                            m_dbName = raw.substr(dbIdx);
                        }
                    } else {
                        m_dbName = "DefaultDb";
                    }
                }

                std::string SqlConnection::GetDatabaseName() const {
                    return m_dbName;
                }

                bool SqlConnection::IsOpen() const {
                    return m_bIsOpen;
                }

                void SqlConnection::Open() {
                    m_bIsOpen = true;
                }

                void SqlConnection::Close() {
                    m_bIsOpen = false;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> SqlConnection::CreateCommand() {
                    return DotNetDupe::System::SmartPointer<SqlCommand>::NewShared("", this);
                }

            }
        }
    }
}

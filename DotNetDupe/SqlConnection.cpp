#include "pch.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/Data/Internal/SqliteDatabaseBackend.h"
#include "System/Data/Internal/InMemoryDatabaseBackend.h"
#include "System/InvalidOperationException.h"
#include <algorithm>

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

                    m_engineType = "InMemory"; // Default
                    size_t engIdx = raw.find("Engine=");
                    if (engIdx != std::string::npos) {
                        engIdx += 7;
                        size_t endIdx = raw.find(";", engIdx);
                        if (endIdx != std::string::npos) {
                            m_engineType = raw.substr(engIdx, endIdx - engIdx);
                        } else {
                            m_engineType = raw.substr(engIdx);
                        }
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

                    std::string engineUpper = m_engineType;
                    std::transform(engineUpper.begin(), engineUpper.end(), engineUpper.begin(), ::toupper);

                    if (engineUpper == "SQLITE") {
#if defined(DOTNETDUPE_USE_SQLITE)
                        auto sqliteBackend = std::make_shared<DotNetDupe::System::Data::Internal::SqliteDatabaseBackend>(m_sConnectionString.GetRawString());
                        DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().RegisterBackend(m_dbName, sqliteBackend);
#else
                        throw DotNetDupe::System::InvalidOperationException("SQLite database engine is not compiled in this build.");
#endif
                    } else {
                        auto inMemoryBackend = std::make_shared<DotNetDupe::System::Data::Internal::InMemoryDatabaseBackend>();
                        DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().RegisterBackend(m_dbName, inMemoryBackend);
                    }
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

#include "pch.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/Data/Internal/SqliteDatabaseBackend.h"
#include "System/Data/Internal/InMemoryDatabaseBackend.h"
#include "System/InvalidOperationException.h"
#include <algorithm>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                struct SqlConnection::Impl {
                    DotNetDupe::System::String m_sConnectionString;
                    std::string m_dbName;
                    std::string m_engineType;
                    bool m_bIsOpen = false;
                };

                SqlConnection::SqlConnection() : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                }

                SqlConnection::SqlConnection(const DotNetDupe::System::String& sConnectionString) 
                    : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                    SetConnectionString(sConnectionString);
                }

                SqlConnection::~SqlConnection() = default;

                DotNetDupe::System::String SqlConnection::GetConnectionString() const {
                    return m_pImpl->m_sConnectionString;
                }

                static std::string ExtractConnectionStringValue(const std::string& raw, const std::string& key, const std::string& defaultVal) {
                    size_t idx = raw.find(key);
                    if (idx == std::string::npos) return defaultVal;
                    idx += key.length();
                    size_t endIdx = raw.find(";", idx);
                    return (endIdx != std::string::npos) ? raw.substr(idx, endIdx - idx) : raw.substr(idx);
                }

                static std::string ExtractDatabaseName(const std::string& raw) {
                    if (raw.find("Database=") != std::string::npos) return ExtractConnectionStringValue(raw, "Database=", "DefaultDb");
                    return ExtractConnectionStringValue(raw, "Data Source=", "DefaultDb");
                }

                void SqlConnection::SetConnectionString(const DotNetDupe::System::String& sConnStr) {
                    m_pImpl->m_sConnectionString = sConnStr;
                    std::string raw = sConnStr.GetRawString();
                    m_pImpl->m_dbName = ExtractDatabaseName(raw);
                    m_pImpl->m_engineType = ExtractConnectionStringValue(raw, "Engine=", "InMemory");
                }

                DotNetDupe::System::String SqlConnection::GetDatabaseName() const {
                    return DotNetDupe::System::String(m_pImpl->m_dbName.c_str());
                }

                bool SqlConnection::IsOpen() const {
                    return m_pImpl->m_bIsOpen;
                }

                void SqlConnection::Open() {
                    m_pImpl->m_bIsOpen = true;

                    std::string engineUpper = m_pImpl->m_engineType;
                    std::transform(engineUpper.begin(), engineUpper.end(), engineUpper.begin(), ::toupper);

                    if (engineUpper == "SQLITE") {
#if defined(DOTNETDUPE_USE_SQLITE)
                        auto sqliteBackend = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Internal::SqliteDatabaseBackend>::NewShared(m_pImpl->m_sConnectionString.GetRawString());
                        DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().RegisterBackend(DotNetDupe::System::String(m_pImpl->m_dbName.c_str()), sqliteBackend.DynamicCast<DotNetDupe::System::Data::Internal::IDatabaseBackend>());
#else
                        throw DotNetDupe::System::InvalidOperationException("SQLite database engine is not compiled in this build.");
#endif
                    } else {
                        auto inMemoryBackend = DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Internal::InMemoryDatabaseBackend>::NewShared();
                        DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().RegisterBackend(DotNetDupe::System::String(m_pImpl->m_dbName.c_str()), inMemoryBackend.DynamicCast<DotNetDupe::System::Data::Internal::IDatabaseBackend>());
                    }
                }

                void SqlConnection::Close() {
                    m_pImpl->m_bIsOpen = false;
                }

                DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> SqlConnection::CreateCommand() {
                    return DotNetDupe::System::SmartPointer<SqlCommand>::NewShared("", this);
                }

            }
        }
    }
}

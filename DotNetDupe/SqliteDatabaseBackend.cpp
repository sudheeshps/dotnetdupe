#include "pch.h"
#include "System/Data/Internal/SqliteDatabaseBackend.h"
#include "System/InvalidOperationException.h"

#if defined(DOTNETDUPE_USE_SQLITE)
#include <sqlite3.h>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                struct SqliteDatabaseBackend::Impl : public DotNetDupe::System::Object {
                    sqlite3* m_db = nullptr;
                    std::string m_connectionString;

                    ~Impl() {
                        if (m_db) sqlite3_close(m_db);
                    }
                };

                SqliteDatabaseBackend::SqliteDatabaseBackend(const DotNetDupe::System::String& connStr) : m_pImpl(SmartPointer<Impl>::NewShared()) {
                    std::string connStrStd = connStr.GetRawString() ? connStr.GetRawString() : "";
                    m_pImpl->m_connectionString = connStrStd;
                    std::string filename = "sqlite_default.db";
                    size_t srcIdx = connStrStd.find("Data Source=");
                    if (srcIdx != std::string::npos) {
                        srcIdx += 12;
                        size_t endIdx = connStrStd.find(";", srcIdx);
                        if (endIdx != std::string::npos) {
                            filename = connStrStd.substr(srcIdx, endIdx - srcIdx);
                        } else {
                            filename = connStrStd.substr(srcIdx);
                        }
                    }

                    if (sqlite3_open(filename.c_str(), &m_pImpl->m_db) != SQLITE_OK) {
                        if (m_pImpl->m_db) {
                            std::string errorMsg = sqlite3_errmsg(m_pImpl->m_db);
                            sqlite3_close(m_pImpl->m_db);
                            m_pImpl->m_db = nullptr;
                            throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                        }
                        throw DotNetDupe::System::InvalidOperationException("Failed to open SQLite database.");
                    }
                }

                SqliteDatabaseBackend::~SqliteDatabaseBackend() {
                }

                void SqliteDatabaseBackend::ClearDatabase() {
                    // SQLite drop table command or cleanup
                }

                Collections::Generic::List<Row> SqliteDatabaseBackend::Execute(
                    const String& sql,
                    const Collections::Generic::Dictionary<String, String>& parameters,
                    Collections::Generic::List<String>& columnNames,
                    int& rowsAffected
                ) {
                    rowsAffected = 0;
                    columnNames.Clear();
                    Collections::Generic::List<Row> resultRows;

                    if (!m_pImpl->m_db) {
                        throw DotNetDupe::System::InvalidOperationException("Database is not open.");
                    }

                    sqlite3_stmt* stmt = nullptr;
                    std::string stdSql = sql.GetRawString() ? sql.GetRawString() : "";
                    if (sqlite3_prepare_v2(m_pImpl->m_db, stdSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                        std::string errorMsg = sqlite3_errmsg(m_pImpl->m_db);
                        throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                    }

                    // Bind parameters if any
                    for (auto const& [paramName, paramVal] : parameters) {
                        std::string stdParamName = paramName.GetRawString() ? paramName.GetRawString() : "";
                        std::string stdParamVal = paramVal.GetRawString() ? paramVal.GetRawString() : "";
                        int idx = sqlite3_bind_parameter_index(stmt, stdParamName.c_str());
                        if (idx > 0) {
                            sqlite3_bind_text(stmt, idx, stdParamVal.c_str(), -1, SQLITE_TRANSIENT);
                        }
                    }

                    int rc = sqlite3_step(stmt);
                    
                    // Retrieve column names first
                    int colCount = sqlite3_column_count(stmt);
                    for (int i = 0; i < colCount; ++i) {
                        const char* colName = sqlite3_column_name(stmt, i);
                        columnNames.Add(colName ? String(colName) : String(""));
                    }

                    while (rc == SQLITE_ROW) {
                        Row r;
                        for (int i = 0; i < colCount; ++i) {
                            const unsigned char* valText = sqlite3_column_text(stmt, i);
                            r.Values.Add(valText ? String(reinterpret_cast<const char*>(valText)) : String(""));
                        }
                        resultRows.Add(std::move(r));
                        rc = sqlite3_step(stmt);
                    }

                    if (rc == SQLITE_DONE) {
                        rowsAffected = sqlite3_changes(m_pImpl->m_db);
                    } else {
                        std::string errorMsg = sqlite3_errmsg(m_pImpl->m_db);
                        sqlite3_finalize(stmt);
                        throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                    }

                    sqlite3_finalize(stmt);
                    return resultRows;
                }

            }
        }
    }
}
#endif

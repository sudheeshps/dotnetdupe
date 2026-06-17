#include "pch.h"
#include "System/Data/Internal/SqliteDatabaseBackend.h"
#include "System/InvalidOperationException.h"

#if defined(DOTNETDUPE_USE_SQLITE)
#include <sqlite3.h>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                SqliteDatabaseBackend::SqliteDatabaseBackend(const std::string& connStr) : m_connectionString(connStr) {
                    std::string filename = "sqlite_default.db";
                    size_t srcIdx = connStr.find("Data Source=");
                    if (srcIdx != std::string::npos) {
                        srcIdx += 12;
                        size_t endIdx = connStr.find(";", srcIdx);
                        if (endIdx != std::string::npos) {
                            filename = connStr.substr(srcIdx, endIdx - srcIdx);
                        } else {
                            filename = connStr.substr(srcIdx);
                        }
                    }

                    if (sqlite3_open(filename.c_str(), &m_db) != SQLITE_OK) {
                        if (m_db) {
                            std::string errorMsg = sqlite3_errmsg(m_db);
                            sqlite3_close(m_db);
                            m_db = nullptr;
                            throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                        }
                        throw DotNetDupe::System::InvalidOperationException("Failed to open SQLite database.");
                    }
                }

                SqliteDatabaseBackend::~SqliteDatabaseBackend() {
                    if (m_db) {
                        sqlite3_close(m_db);
                    }
                }

                void SqliteDatabaseBackend::ClearDatabase() {
                    // SQLite drop table command or cleanup
                }

                std::vector<Row> SqliteDatabaseBackend::Execute(
                    const std::string& sql,
                    const std::unordered_map<std::string, std::string>& parameters,
                    std::vector<std::string>& columnNames,
                    int& rowsAffected
                ) {
                    rowsAffected = 0;
                    columnNames.clear();
                    std::vector<Row> resultRows;

                    if (!m_db) {
                        throw DotNetDupe::System::InvalidOperationException("Database is not open.");
                    }

                    sqlite3_stmt* stmt = nullptr;
                    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                        std::string errorMsg = sqlite3_errmsg(m_db);
                        throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                    }

                    // Bind parameters if any
                    for (const auto& [paramName, paramVal] : parameters) {
                        int idx = sqlite3_bind_parameter_index(stmt, paramName.c_str());
                        if (idx > 0) {
                            sqlite3_bind_text(stmt, idx, paramVal.c_str(), -1, SQLITE_TRANSIENT);
                        }
                    }

                    int rc = sqlite3_step(stmt);
                    
                    // Retrieve column names first
                    int colCount = sqlite3_column_count(stmt);
                    for (int i = 0; i < colCount; ++i) {
                        const char* colName = sqlite3_column_name(stmt, i);
                        columnNames.push_back(colName ? colName : "");
                    }

                    while (rc == SQLITE_ROW) {
                        Row r;
                        for (int i = 0; i < colCount; ++i) {
                            const unsigned char* valText = sqlite3_column_text(stmt, i);
                            r.Values.push_back(valText ? reinterpret_cast<const char*>(valText) : "");
                        }
                        resultRows.push_back(r);
                        rc = sqlite3_step(stmt);
                    }

                    if (rc == SQLITE_DONE) {
                        rowsAffected = sqlite3_changes(m_db);
                    } else {
                        std::string errorMsg = sqlite3_errmsg(m_db);
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

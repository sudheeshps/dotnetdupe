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

                static std::string ExtractSqliteDataSource(const std::string& connStr) {
                    size_t srcIdx = connStr.find("Data Source=");
                    if (srcIdx == std::string::npos) return "sqlite_default.db";
                    srcIdx += 12;
                    size_t endIdx = connStr.find(";", srcIdx);
                    return (endIdx != std::string::npos) ? connStr.substr(srcIdx, endIdx - srcIdx) : connStr.substr(srcIdx);
                }

                SqliteDatabaseBackend::SqliteDatabaseBackend(const DotNetDupe::System::String& connStr) : m_pImpl(SmartPointer<Impl>::NewShared()) {
                    std::string connStrStd = connStr.GetRawString() ? connStr.GetRawString() : "";
                    m_pImpl->m_connectionString = connStrStd;
                    std::string filename = ExtractSqliteDataSource(connStrStd);
                    if (sqlite3_open(filename.c_str(), &m_pImpl->m_db) != SQLITE_OK) {
                        std::string errorMsg = m_pImpl->m_db ? sqlite3_errmsg(m_pImpl->m_db) : "Failed to open SQLite database.";
                        if (m_pImpl->m_db) { sqlite3_close(m_pImpl->m_db); m_pImpl->m_db = nullptr; }
                        throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                    }
                }

                SqliteDatabaseBackend::~SqliteDatabaseBackend() {
                }

                void SqliteDatabaseBackend::ClearDatabase() {
                }

                static void BindSqliteParameters(sqlite3_stmt* stmt, const Collections::Generic::Dictionary<String, String>& parameters) {
                    for (auto const& [paramName, paramVal] : parameters) {
                        std::string stdParamName = paramName.GetRawString() ? paramName.GetRawString() : "";
                        std::string stdParamVal = paramVal.GetRawString() ? paramVal.GetRawString() : "";
                        int idx = sqlite3_bind_parameter_index(stmt, stdParamName.c_str());
                        if (idx > 0) sqlite3_bind_text(stmt, idx, stdParamVal.c_str(), -1, SQLITE_TRANSIENT);
                    }
                }

                static void FetchSqliteRows(sqlite3_stmt* stmt, int colCount, Collections::Generic::List<Row>& resultRows, int& rc) {
                    while (rc == SQLITE_ROW) {
                        Row r;
                        for (int i = 0; i < colCount; ++i) {
                            const unsigned char* valText = sqlite3_column_text(stmt, i);
                            r.Values.Add(valText ? String(reinterpret_cast<const char*>(valText)) : String(""));
                        }
                        resultRows.Add(std::move(r));
                        rc = sqlite3_step(stmt);
                    }
                }

                static void CollectSqliteColumnNames(sqlite3_stmt* stmt, Collections::Generic::List<String>& columnNames) {
                    int colCount = sqlite3_column_count(stmt);
                    for (int i = 0; i < colCount; ++i) {
                        const char* colName = sqlite3_column_name(stmt, i);
                        columnNames.Add(colName ? String(colName) : String(""));
                    }
                }

                static sqlite3_stmt* PrepareSqliteStatement(sqlite3* db, const String& sql) {
                    if (!db) throw InvalidOperationException("Database is not open.");
                    sqlite3_stmt* stmt = nullptr;
                    if (sqlite3_prepare_v2(db, sql.GetRawString() ? sql.GetRawString() : "", -1, &stmt, nullptr) != SQLITE_OK) {
                        throw InvalidOperationException(sqlite3_errmsg(db));
                    }
                    return stmt;
                }

                static void FinalizeExecution(sqlite3* db, sqlite3_stmt* stmt, int rc, int& rowsAffected) {
                    if (rc == SQLITE_DONE) rowsAffected = sqlite3_changes(db);
                    else { std::string err = sqlite3_errmsg(db); sqlite3_finalize(stmt); throw InvalidOperationException(err.c_str()); }
                    sqlite3_finalize(stmt);
                }

                Collections::Generic::List<Row> SqliteDatabaseBackend::Execute(
                    const String& sql,
                    const Collections::Generic::Dictionary<String, String>& parameters,
                    Collections::Generic::List<String>& columnNames,
                    int& rowsAffected
                ) {
                    rowsAffected = 0; columnNames.Clear();
                    sqlite3_stmt* stmt = PrepareSqliteStatement(m_pImpl->m_db, sql);
                    BindSqliteParameters(stmt, parameters);
                    int rc = sqlite3_step(stmt);
                    CollectSqliteColumnNames(stmt, columnNames);
                    Collections::Generic::List<Row> resultRows;
                    FetchSqliteRows(stmt, sqlite3_column_count(stmt), resultRows, rc);
                    FinalizeExecution(m_pImpl->m_db, stmt, rc, rowsAffected);
                    return resultRows;
                }

            }
        }
    }
}
#endif

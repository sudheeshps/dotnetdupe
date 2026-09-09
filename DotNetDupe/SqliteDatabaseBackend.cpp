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
                    sqlite3* m_pDb = nullptr;
                    std::string m_connectionString;

                    ~Impl() {
                        if (m_pDb) sqlite3_close(m_pDb);
                    }
                };

                static std::string ExtractSqliteDataSource(const std::string& connStr) {
                    /// Step: Locate Data Source key in connection string.
                    size_t srcIdx = connStr.find("Data Source=");
                    if (srcIdx == std::string::npos) return "sqlite_default.db";
                    srcIdx += 12;
                    size_t endIdx = connStr.find(";", srcIdx);
                    return (endIdx != std::string::npos) ? connStr.substr(srcIdx, endIdx - srcIdx) : connStr.substr(srcIdx);
                }

                SqliteDatabaseBackend::SqliteDatabaseBackend(const DotNetDupe::System::String& connStr) : m_pImpl(SmartPointer<Impl>::NewShared()) {
                    /// Step: Extract database file path.
                    std::string connStrStd = connStr.GetRawString() ? connStr.GetRawString() : "";
                    m_pImpl->m_connectionString = connStrStd;
                    std::string filename = ExtractSqliteDataSource(connStrStd);

                    /// Step: Open SQLite connection handle.
                    if (sqlite3_open(filename.c_str(), &m_pImpl->m_pDb) != SQLITE_OK) {
                        std::string errorMsg = m_pImpl->m_pDb ? sqlite3_errmsg(m_pImpl->m_pDb) : "Failed to open SQLite database.";
                        if (m_pImpl->m_pDb) { sqlite3_close(m_pImpl->m_pDb); m_pImpl->m_pDb = nullptr; }
                        throw DotNetDupe::System::InvalidOperationException(errorMsg.c_str());
                    }
                }

                SqliteDatabaseBackend::~SqliteDatabaseBackend() {
                }

                void SqliteDatabaseBackend::ClearDatabase() {
                    /// Step: Clear database operations if necessary.
                }

                static void BindSqliteParameters(sqlite3_stmt* pStmt, const Collections::Generic::Dictionary<String, String>& parameters) {
                    /// Step: Bind named parameters to prepared statement.
                    for (auto const& [paramName, paramVal] : parameters) {
                        std::string stdParamName = paramName.GetRawString() ? paramName.GetRawString() : "";
                        std::string stdParamVal = paramVal.GetRawString() ? paramVal.GetRawString() : "";
                        int idx = sqlite3_bind_parameter_index(pStmt, stdParamName.c_str());
                        if (idx > 0) sqlite3_bind_text(pStmt, idx, stdParamVal.c_str(), -1, SQLITE_TRANSIENT);
                    }
                }

                static void FetchSqliteRows(sqlite3_stmt* pStmt, int colCount, Collections::Generic::List<Row>& resultRows, int& rc) {
                    /// Step: Step through statement records and populate Row structures.
                    while (rc == SQLITE_ROW) {
                        Row r;
                        for (int i = 0; i < colCount; ++i) {
                            const unsigned char* valText = sqlite3_column_text(pStmt, i);
                            r.Values.Add(valText ? String(reinterpret_cast<const char*>(valText)) : String(""));
                        }
                        resultRows.Add(std::move(r));
                        rc = sqlite3_step(pStmt);
                    }
                }

                static void CollectSqliteColumnNames(sqlite3_stmt* pStmt, Collections::Generic::List<String>& columnNames) {
                    /// Step: Retrieve column metadata names from prepared statement.
                    int colCount = sqlite3_column_count(pStmt);
                    for (int i = 0; i < colCount; ++i) {
                        const char* colName = sqlite3_column_name(pStmt, i);
                        columnNames.Add(colName ? String(colName) : String(""));
                    }
                }

                static sqlite3_stmt* PrepareSqliteStatement(sqlite3* pDb, const String& sql) {
                    /// Step: Verify database is initialized and compile SQL statement.
                    if (!pDb) throw InvalidOperationException("Database is not open.");
                    sqlite3_stmt* pStmt = nullptr;
                    if (sqlite3_prepare_v2(pDb, sql.GetRawString() ? sql.GetRawString() : "", -1, &pStmt, nullptr) != SQLITE_OK) {
                        throw InvalidOperationException(sqlite3_errmsg(pDb));
                    }
                    return pStmt;
                }

                static void FinalizeExecution(sqlite3* pDb, sqlite3_stmt* pStmt, int rc, int& rowsAffected) {
                    /// Step: Retrieve affected row changes and finalize statement.
                    if (rc == SQLITE_DONE) rowsAffected = sqlite3_changes(pDb);
                    else { std::string err = sqlite3_errmsg(pDb); sqlite3_finalize(pStmt); throw InvalidOperationException(err.c_str()); }
                    sqlite3_finalize(pStmt);
                }

                Collections::Generic::List<Row> SqliteDatabaseBackend::Execute(
                    const String& sql,
                    const Collections::Generic::Dictionary<String, String>& parameters,
                    Collections::Generic::List<String>& columnNames,
                    int& rowsAffected
                ) {
                    /// Step: Reset outputs and prepare SQL statement.
                    rowsAffected = 0; columnNames.Clear();
                    sqlite3_stmt* pStmt = PrepareSqliteStatement(m_pImpl->m_pDb, sql);

                    /// Step: Bind parameters and step statement.
                    BindSqliteParameters(pStmt, parameters);
                    int rc = sqlite3_step(pStmt);
                    CollectSqliteColumnNames(pStmt, columnNames);

                    /// Step: Fetch result rows and finalize.
                    Collections::Generic::List<Row> resultRows;
                    FetchSqliteRows(pStmt, sqlite3_column_count(pStmt), resultRows, rc);
                    FinalizeExecution(m_pImpl->m_pDb, pStmt, rc, rowsAffected);
                    return resultRows;
                }

            }
        }
    }
}
#endif

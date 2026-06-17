#include "pch.h"
#include "System/Data/Internal/InMemoryDatabaseBackend.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                InMemoryDatabaseBackend::InMemoryDatabaseBackend() = default;
                InMemoryDatabaseBackend::~InMemoryDatabaseBackend() = default;

                void InMemoryDatabaseBackend::ClearDatabase() {
                    m_tables.clear();
                }

                std::vector<std::string> InMemoryDatabaseBackend::Tokenize(const std::string& s) const {
                    std::vector<std::string> tokens;
                    std::string token;
                    bool inQuotes = false;
                    for (size_t i = 0; i < s.length(); ++i) {
                        char c = s[i];
                        if (c == '\'') {
                            inQuotes = !inQuotes;
                            token += c;
                        } else if (!inQuotes && (c == ' ' || c == ',' || c == '(' || c == ')' || c == '=')) {
                            if (!token.empty()) {
                                tokens.push_back(token);
                                token.clear();
                            }
                            if (c == '(' || c == ')' || c == '=') {
                                tokens.push_back(std::string(1, c));
                            }
                        } else {
                            token += c;
                        }
                    }
                    if (!token.empty()) {
                        tokens.push_back(token);
                    }
                    return tokens;
                }

                std::string InMemoryDatabaseBackend::ToUpper(std::string s) const {
                    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                    return s;
                }

                std::string InMemoryDatabaseBackend::ResolveValue(const std::string& val, const std::unordered_map<std::string, std::string>& parameters) const {
                    if (!val.empty() && val[0] == '@') {
                        auto it = parameters.find(val);
                        if (it != parameters.end()) {
                            return it->second;
                        }
                        return std::string("");
                    }
                    if (val.length() >= 2 && val.front() == '\'' && val.back() == '\'') {
                        return val.substr(1, val.length() - 2);
                    }
                    return val;
                }

                void InMemoryDatabaseBackend::ExecuteCreate(const std::vector<std::string>& tokens) {
                    if (tokens.size() > 2 && ToUpper(tokens[1]) == "TABLE") {
                        std::string tableName = tokens[2];
                        Table t;
                        size_t idx = 3;
                        while (idx < tokens.size()) {
                            if (tokens[idx] == "(" || tokens[idx] == ")") {
                                idx++;
                                continue;
                            }
                            std::string colName = tokens[idx];
                            idx++;
                            if (idx < tokens.size() && tokens[idx] != "," && tokens[idx] != ")") {
                                idx++;
                            }
                            t.Columns.push_back(colName);
                            if (idx < tokens.size() && tokens[idx] == ",") {
                                idx++;
                            }
                        }
                        m_tables[tableName] = t;
                    }
                }

                void InMemoryDatabaseBackend::ExecuteInsert(const std::vector<std::string>& tokens,
                                                           const std::unordered_map<std::string, std::string>& parameters,
                                                           int& rowsAffected) {
                    if (tokens.size() > 2 && ToUpper(tokens[1]) == "INTO") {
                        std::string tableName = tokens[2];
                        auto tIt = m_tables.find(tableName);
                        if (tIt == m_tables.end()) return;

                        Table& t = tIt->second;
                        std::vector<std::string> cols;
                        std::vector<std::string> vals;

                        size_t idx = 3;
                        if (idx < tokens.size() && tokens[idx] == "(") {
                            idx++;
                            while (idx < tokens.size() && tokens[idx] != ")") {
                                cols.push_back(tokens[idx]);
                                idx++;
                            }
                            if (idx < tokens.size()) idx++;
                        }

                        if (idx < tokens.size() && ToUpper(tokens[idx]) == "VALUES") {
                            idx++;
                        }

                        if (idx < tokens.size() && tokens[idx] == "(") {
                            idx++;
                            while (idx < tokens.size() && tokens[idx] != ")") {
                                vals.push_back(ResolveValue(tokens[idx], parameters));
                                idx++;
                            }
                        }

                        Row r;
                        r.Values.resize(t.Columns.size());

                        if (cols.empty()) {
                            for (size_t i = 0; i < t.Columns.size() && i < vals.size(); ++i) {
                                r.Values[i] = vals[i];
                            }
                        } else {
                            for (size_t i = 0; i < cols.size() && i < vals.size(); ++i) {
                                auto colName = cols[i];
                                auto cIt = std::find(t.Columns.begin(), t.Columns.end(), colName);
                                if (cIt != t.Columns.end()) {
                                    size_t colIdx = std::distance(t.Columns.begin(), cIt);
                                    r.Values[colIdx] = vals[i];
                                }
                            }
                        }
                        t.Rows.push_back(r);
                        rowsAffected = 1;
                    }
                }

                std::vector<Row> InMemoryDatabaseBackend::ExecuteSelect(const std::vector<std::string>& tokens,
                                                                       const std::unordered_map<std::string, std::string>& parameters,
                                                                       std::vector<std::string>& columnNames) {
                    std::vector<std::string> selectCols;
                    size_t idx = 1;
                    while (idx < tokens.size() && ToUpper(tokens[idx]) != "FROM") {
                        selectCols.push_back(tokens[idx]);
                        idx++;
                    }

                    if (idx >= tokens.size()) return {};
                    idx++;
                    if (idx >= tokens.size()) return {};
                    std::string tableName = tokens[idx];
                    idx++;

                    auto tIt = m_tables.find(tableName);
                    if (tIt == m_tables.end()) return {};

                    Table& t = tIt->second;

                    std::string whereCol;
                    std::string whereVal;
                    if (idx < tokens.size() && ToUpper(tokens[idx]) == "WHERE") {
                        idx++;
                        if (idx < tokens.size()) whereCol = tokens[idx];
                        idx++;
                        if (idx < tokens.size() && tokens[idx] == "=") {
                            idx++;
                            if (idx < tokens.size()) whereVal = ResolveValue(tokens[idx], parameters);
                        }
                    }

                    std::vector<size_t> colIndices;
                    if (selectCols.size() == 1 && selectCols[0] == "*") {
                        columnNames = t.Columns;
                        for (size_t i = 0; i < t.Columns.size(); ++i) colIndices.push_back(i);
                    } else {
                        for (auto const& selCol : selectCols) {
                            auto cIt = std::find(t.Columns.begin(), t.Columns.end(), selCol);
                            if (cIt != t.Columns.end()) {
                                columnNames.push_back(selCol);
                                colIndices.push_back(std::distance(t.Columns.begin(), cIt));
                            }
                        }
                    }

                    int whereColIdx = -1;
                    if (!whereCol.empty()) {
                        auto cIt = std::find(t.Columns.begin(), t.Columns.end(), whereCol);
                        if (cIt != t.Columns.end()) {
                            whereColIdx = static_cast<int>(std::distance(t.Columns.begin(), cIt));
                        }
                    }

                    std::vector<Row> resultRows;
                    for (auto const& row : t.Rows) {
                        if (whereColIdx != -1) {
                            if (row.Values[whereColIdx] != whereVal) {
                                continue;
                            }
                        }
                        Row newRow;
                        for (auto index : colIndices) {
                            newRow.Values.push_back(row.Values[index]);
                        }
                        resultRows.push_back(newRow);
                    }
                    return resultRows;
                }

                void InMemoryDatabaseBackend::ExecuteUpdate(const std::vector<std::string>& tokens,
                                                           const std::unordered_map<std::string, std::string>& parameters,
                                                           int& rowsAffected) {
                    std::string tableName = tokens[1];
                    auto tIt = m_tables.find(tableName);
                    if (tIt == m_tables.end()) return;

                    Table& t = tIt->second;

                    size_t idx = 2;
                    if (idx < tokens.size() && ToUpper(tokens[idx]) == "SET") {
                        idx++;
                    }

                    std::vector<std::pair<std::string, std::string>> updates;
                    while (idx < tokens.size() && ToUpper(tokens[idx]) != "WHERE") {
                        std::string colName = tokens[idx];
                        idx++;
                        if (idx < tokens.size() && tokens[idx] == "=") {
                            idx++;
                            std::string val = ResolveValue(tokens[idx], parameters);
                            updates.push_back({colName, val});
                            idx++;
                        }
                    }

                    std::string whereCol;
                    std::string whereVal;
                    if (idx < tokens.size() && ToUpper(tokens[idx]) == "WHERE") {
                        idx++;
                        if (idx < tokens.size()) whereCol = tokens[idx];
                        idx++;
                        if (idx < tokens.size() && tokens[idx] == "=") {
                            idx++;
                            if (idx < tokens.size()) whereVal = ResolveValue(tokens[idx], parameters);
                        }
                    }

                    int whereColIdx = -1;
                    if (!whereCol.empty()) {
                        auto cIt = std::find(t.Columns.begin(), t.Columns.end(), whereCol);
                        if (cIt != t.Columns.end()) {
                            whereColIdx = static_cast<int>(std::distance(t.Columns.begin(), cIt));
                        }
                    }

                    for (auto& row : t.Rows) {
                        if (whereColIdx != -1) {
                            if (row.Values[whereColIdx] != whereVal) {
                                continue;
                            }
                        }
                        for (auto const& update : updates) {
                            auto cIt = std::find(t.Columns.begin(), t.Columns.end(), update.first);
                            if (cIt != t.Columns.end()) {
                                size_t updateIdx = std::distance(t.Columns.begin(), cIt);
                                row.Values[updateIdx] = update.second;
                            }
                        }
                        rowsAffected++;
                    }
                }

                void InMemoryDatabaseBackend::ExecuteDelete(const std::vector<std::string>& tokens,
                                                           const std::unordered_map<std::string, std::string>& parameters,
                                                           int& rowsAffected) {
                    if (tokens.size() > 2 && ToUpper(tokens[1]) == "FROM") {
                        std::string tableName = tokens[2];
                        auto tIt = m_tables.find(tableName);
                        if (tIt == m_tables.end()) return;

                        Table& t = tIt->second;

                        size_t idx = 3;
                        std::string whereCol;
                        std::string whereVal;
                        if (idx < tokens.size() && ToUpper(tokens[idx]) == "WHERE") {
                            idx++;
                            if (idx < tokens.size()) whereCol = tokens[idx];
                            idx++;
                            if (idx < tokens.size() && tokens[idx] == "=") {
                                idx++;
                                if (idx < tokens.size()) whereVal = ResolveValue(tokens[idx], parameters);
                            }
                        }

                        int whereColIdx = -1;
                        if (!whereCol.empty()) {
                            auto cIt = std::find(t.Columns.begin(), t.Columns.end(), whereCol);
                            if (cIt != t.Columns.end()) {
                                whereColIdx = static_cast<int>(std::distance(t.Columns.begin(), cIt));
                            }
                        }

                        auto it = t.Rows.begin();
                        while (it != t.Rows.end()) {
                            if (whereColIdx != -1) {
                                if (it->Values[whereColIdx] == whereVal) {
                                    it = t.Rows.erase(it);
                                    rowsAffected++;
                                    continue;
                                }
                            } else {
                                it = t.Rows.erase(it);
                                rowsAffected++;
                                continue;
                            }
                            ++it;
                        }
                    }
                }

                std::vector<Row> InMemoryDatabaseBackend::Execute(
                    const std::string& sql,
                    const std::unordered_map<std::string, std::string>& parameters,
                    std::vector<std::string>& columnNames,
                    int& rowsAffected
                ) {
                    rowsAffected = 0;
                    columnNames.clear();

                    std::vector<std::string> tokens = Tokenize(sql);
                    if (tokens.empty()) return {};

                    std::string cmd = ToUpper(tokens[0]);

                    if (cmd == "CREATE") {
                        ExecuteCreate(tokens);
                        return {};
                    }
                    else if (cmd == "INSERT") {
                        ExecuteInsert(tokens, parameters, rowsAffected);
                        return {};
                    }
                    else if (cmd == "SELECT") {
                        return ExecuteSelect(tokens, parameters, columnNames);
                    }
                    else if (cmd == "UPDATE") {
                        ExecuteUpdate(tokens, parameters, rowsAffected);
                        return {};
                    }
                    else if (cmd == "DELETE") {
                        ExecuteDelete(tokens, parameters, rowsAffected);
                        return {};
                    }

                    return {};
                }

            }
        }
    }
}

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include "System/String.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                struct Row {
                    std::vector<std::string> Values;
                };

                struct Table {
                    std::vector<std::string> Columns;
                    std::vector<Row> Rows;
                };

                class DatabaseEngine {
                private:
                    std::unordered_map<std::string, std::unordered_map<std::string, Table>> m_databases;

                    DatabaseEngine() = default;

                public:
                    static DatabaseEngine& Instance() {
                        static DatabaseEngine instance;
                        return instance;
                    }

                    void ClearDatabase(const std::string& dbName) {
                        m_databases[dbName].clear();
                    }

                    std::vector<Row> Execute(
                        const std::string& dbName,
                        const std::string& sql,
                        const std::unordered_map<std::string, std::string>& parameters,
                        std::vector<std::string>& columnNames,
                        int& rowsAffected
                    ) {
                        rowsAffected = 0;
                        columnNames.clear();

                        auto tokenize = [](const std::string& s) {
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
                        };

                        std::vector<std::string> tokens = tokenize(sql);
                        if (tokens.empty()) return {};

                        auto toUpper = [](std::string s) {
                            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                            return s;
                        };

                        std::string cmd = toUpper(tokens[0]);

                        auto resolveVal = [&](const std::string& val) {
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
                        };

                        auto& db = m_databases[dbName];

                        if (cmd == "CREATE") {
                            if (tokens.size() > 2 && toUpper(tokens[1]) == "TABLE") {
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
                                db[tableName] = t;
                            }
                            return {};
                        }
                        else if (cmd == "INSERT") {
                            if (tokens.size() > 2 && toUpper(tokens[1]) == "INTO") {
                                std::string tableName = tokens[2];
                                auto tIt = db.find(tableName);
                                if (tIt == db.end()) return {};

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

                                if (idx < tokens.size() && toUpper(tokens[idx]) == "VALUES") {
                                    idx++;
                                }

                                if (idx < tokens.size() && tokens[idx] == "(") {
                                    idx++;
                                    while (idx < tokens.size() && tokens[idx] != ")") {
                                        vals.push_back(resolveVal(tokens[idx]));
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
                            return {};
                        }
                        else if (cmd == "SELECT") {
                            std::vector<std::string> selectCols;
                            size_t idx = 1;
                            while (idx < tokens.size() && toUpper(tokens[idx]) != "FROM") {
                                selectCols.push_back(tokens[idx]);
                                idx++;
                            }

                            if (idx >= tokens.size()) return {};
                            idx++;
                            if (idx >= tokens.size()) return {};
                            std::string tableName = tokens[idx];
                            idx++;

                            auto tIt = db.find(tableName);
                            if (tIt == db.end()) return {};

                            Table& t = tIt->second;

                            std::string whereCol;
                            std::string whereVal;
                            if (idx < tokens.size() && toUpper(tokens[idx]) == "WHERE") {
                                idx++;
                                if (idx < tokens.size()) whereCol = tokens[idx];
                                idx++;
                                if (idx < tokens.size() && tokens[idx] == "=") {
                                    idx++;
                                    if (idx < tokens.size()) whereVal = resolveVal(tokens[idx]);
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
                        else if (cmd == "UPDATE") {
                            std::string tableName = tokens[1];
                            auto tIt = db.find(tableName);
                            if (tIt == db.end()) return {};

                            Table& t = tIt->second;

                            size_t idx = 2;
                            if (idx < tokens.size() && toUpper(tokens[idx]) == "SET") {
                                idx++;
                            }

                            std::vector<std::pair<std::string, std::string>> updates;
                            while (idx < tokens.size() && toUpper(tokens[idx]) != "WHERE") {
                                std::string colName = tokens[idx];
                                idx++;
                                if (idx < tokens.size() && tokens[idx] == "=") {
                                    idx++;
                                    std::string val = resolveVal(tokens[idx]);
                                    updates.push_back({colName, val});
                                    idx++;
                                }
                            }

                            std::string whereCol;
                            std::string whereVal;
                            if (idx < tokens.size() && toUpper(tokens[idx]) == "WHERE") {
                                idx++;
                                if (idx < tokens.size()) whereCol = tokens[idx];
                                idx++;
                                if (idx < tokens.size() && tokens[idx] == "=") {
                                    idx++;
                                    if (idx < tokens.size()) whereVal = resolveVal(tokens[idx]);
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
                            return {};
                        }
                        else if (cmd == "DELETE") {
                            if (tokens.size() > 2 && toUpper(tokens[1]) == "FROM") {
                                std::string tableName = tokens[2];
                                auto tIt = db.find(tableName);
                                if (tIt == db.end()) return {};

                                Table& t = tIt->second;

                                size_t idx = 3;
                                std::string whereCol;
                                std::string whereVal;
                                if (idx < tokens.size() && toUpper(tokens[idx]) == "WHERE") {
                                    idx++;
                                    if (idx < tokens.size()) whereCol = tokens[idx];
                                    idx++;
                                    if (idx < tokens.size() && tokens[idx] == "=") {
                                        idx++;
                                        if (idx < tokens.size()) whereVal = resolveVal(tokens[idx]);
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
                            return {};
                        }

                        return {};
                    }
                };

            }
        }
    }
}

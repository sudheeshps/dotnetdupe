#pragma once
#include <string>
#include <vector>
#include <unordered_map>

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

                class IDatabaseBackend {
                public:
                    virtual ~IDatabaseBackend() = default;

                    virtual std::vector<Row> Execute(
                        const std::string& sql,
                        const std::unordered_map<std::string, std::string>& parameters,
                        std::vector<std::string>& columnNames,
                        int& rowsAffected
                    ) = 0;

                    virtual void ClearDatabase() = 0;
                };

            }
        }
    }
}

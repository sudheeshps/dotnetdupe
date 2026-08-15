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

            }
        }
    }
}

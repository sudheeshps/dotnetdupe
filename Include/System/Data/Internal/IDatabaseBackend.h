#pragma once
#include "Common.h"
#include "System/String.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Internal {

                struct Row {
                    Collections::Generic::List<String> Values;
                };

                struct Table {
                    Collections::Generic::List<String> Columns;
                    Collections::Generic::List<Row> Rows;
                };

                class IDatabaseBackend {
                public:
                    virtual ~IDatabaseBackend() = default;

                    virtual Collections::Generic::List<Row> Execute(
                        const String& sql,
                        const Collections::Generic::Dictionary<String, String>& parameters,
                        Collections::Generic::List<String>& columnNames,
                        int& rowsAffected
                    ) = 0;

                    virtual void ClearDatabase() = 0;
                };

            }
        }
    }
}

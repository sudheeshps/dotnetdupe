#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Data/Common/DbCommand.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                class DbConnection : public virtual DotNetDupe::System::Object {
                public:
                    DbConnection() = default;
                    ~DbConnection() override = default;

                    virtual DotNetDupe::System::String GetConnectionString() const = 0;
                    virtual void SetConnectionString(const DotNetDupe::System::String& sConnStr) = 0;

                    virtual void Open() = 0;
                    virtual void Close() = 0;

                    virtual DotNetDupe::System::SmartPointer<DbCommand> CreateCommand() = 0;
                };

            }
        }
    }
}

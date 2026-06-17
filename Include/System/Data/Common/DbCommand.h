#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Data/Common/DbDataReader.h"
#include "System/Data/Common/DbParameter.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                class DbCommand : public virtual DotNetDupe::System::Object {
                public:
                    DbCommand() = default;
                    ~DbCommand() override = default;

                    virtual DotNetDupe::System::String GetCommandText() const = 0;
                    virtual void SetCommandText(const DotNetDupe::System::String& sText) = 0;

                    virtual DotNetDupe::System::SmartPointer<DbParameterCollection> GetParameters() const = 0;

                    virtual DotNetDupe::System::SmartPointer<DbDataReader> ExecuteReader() = 0;
                    virtual int ExecuteNonQuery() = 0;
                    virtual DotNetDupe::System::String ExecuteScalar() = 0;
                };

            }
        }
    }
}

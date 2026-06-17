#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                class DbDataReader : public virtual DotNetDupe::System::Object {
                public:
                    DbDataReader() = default;
                    ~DbDataReader() override = default;

                    virtual bool Read() = 0;
                    virtual DotNetDupe::System::String GetString(int iOrdinal) = 0;
                    virtual int GetInt32(int iOrdinal) = 0;
                    virtual double GetDouble(int iOrdinal) = 0;
                    virtual bool IsDBNull(int iOrdinal) = 0;
                    virtual int GetFieldCount() const = 0;
                    virtual DotNetDupe::System::String GetName(int iOrdinal) = 0;
                    virtual int GetOrdinal(const DotNetDupe::System::String& sName) = 0;

                    virtual DotNetDupe::System::String operator[](const DotNetDupe::System::String& sName) = 0;
                    virtual DotNetDupe::System::String operator[](int iOrdinal) = 0;
                };

            }
        }
    }
}

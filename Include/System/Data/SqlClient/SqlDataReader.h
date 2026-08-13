#pragma once
#include "System/Data/Common/DbDataReader.h"
#include "System/Convert.h"
#include "System/ArgumentException.h"
#include "System/SmartPointer.h"
#include "System/Data/Internal/IDatabaseBackend.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlDataReader : public DotNetDupe::System::Data::Common::DbDataReader {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    DOTNETDUPE_API SqlDataReader(Collections::Generic::List<DotNetDupe::System::Data::Internal::Row>&& rows, Collections::Generic::List<DotNetDupe::System::String>&& columns);
                    DOTNETDUPE_API ~SqlDataReader() override;

                    DOTNETDUPE_API bool Read() override;
                    DOTNETDUPE_API DotNetDupe::System::String GetString(int iOrdinal) override;
                    DOTNETDUPE_API int GetInt32(int iOrdinal) override;
                    DOTNETDUPE_API double GetDouble(int iOrdinal) override;
                    DOTNETDUPE_API bool IsDBNull(int iOrdinal) override;
                    DOTNETDUPE_API int GetFieldCount() const override;
                    DOTNETDUPE_API DotNetDupe::System::String GetName(int iOrdinal) override;
                    DOTNETDUPE_API int GetOrdinal(const DotNetDupe::System::String& sName) override;
                    DOTNETDUPE_API DotNetDupe::System::String operator[](const DotNetDupe::System::String& sName) override;
                    DOTNETDUPE_API DotNetDupe::System::String operator[](int iOrdinal) override;
                };

            }
        }
    }
}

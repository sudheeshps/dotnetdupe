#pragma once
#include "Common.h"
#include "System/Data/Common/DbConnection.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection : public DotNetDupe::System::Data::Common::DbConnection {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    DOTNETDUPE_API SqlConnection();
                    DOTNETDUPE_API SqlConnection(const DotNetDupe::System::String& sConnectionString);
                    DOTNETDUPE_API ~SqlConnection() override;

                    DOTNETDUPE_API DotNetDupe::System::String GetConnectionString() const override;
                    DOTNETDUPE_API void SetConnectionString(const DotNetDupe::System::String& sConnStr) override;

                    DOTNETDUPE_API DotNetDupe::System::String GetDatabaseName() const;
                    DOTNETDUPE_API bool IsOpen() const;

                    DOTNETDUPE_API void Open() override;
                    DOTNETDUPE_API void Close() override;

                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> CreateCommand() override;
                };

            }
        }
    }
}

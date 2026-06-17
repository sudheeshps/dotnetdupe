#pragma once
#include "Common.h"
#include "System/Data/Common/DbConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class DOTNETDUPE_API SqlConnection : public DotNetDupe::System::Data::Common::DbConnection {
                private:
                    DotNetDupe::System::String m_sConnectionString;
                    std::string m_dbName;
                    bool m_bIsOpen = false;

                public:
                    SqlConnection() = default;
                    SqlConnection(const DotNetDupe::System::String& sConnectionString);
                    ~SqlConnection() override = default;

                    DotNetDupe::System::String GetConnectionString() const override;
                    void SetConnectionString(const DotNetDupe::System::String& sConnStr) override;

                    std::string GetDatabaseName() const;
                    bool IsOpen() const;

                    void Open() override;
                    void Close() override;

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> CreateCommand() override;
                };

            }
        }
    }
}

#pragma once
#include "Common.h"
#include "System/Data/Common/DbCommand.h"
#include "System/Data/Internal/DatabaseEngine.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection;

                class DOTNETDUPE_API SqlCommand : public DotNetDupe::System::Data::Common::DbCommand {
                private:
                    DotNetDupe::System::String m_sCommandText;
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> m_parameters;
                    SqlConnection* m_connection = nullptr;

                public:
                    SqlCommand();
                    SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection);
                    ~SqlCommand() override = default;

                    DotNetDupe::System::String GetCommandText() const override;
                    void SetCommandText(const DotNetDupe::System::String& sText) override;

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> GetParameters() const override;

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> ExecuteReader() override;
                    int ExecuteNonQuery() override;
                    DotNetDupe::System::String ExecuteScalar() override;
                };

            }
        }
    }
}

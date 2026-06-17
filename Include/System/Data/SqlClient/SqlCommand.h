#pragma once
#include "Common.h"
#include "System/Data/Common/DbCommand.h"
#include "System/Data/Internal/DatabaseEngine.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection;

                class SqlCommand : public DotNetDupe::System::Data::Common::DbCommand {
                private:
                    DotNetDupe::System::String m_sCommandText;
                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> m_parameters;
                    SqlConnection* m_connection = nullptr;

                public:
                    DOTNETDUPE_API SqlCommand();
                    DOTNETDUPE_API SqlCommand(const DotNetDupe::System::String& sText, SqlConnection* connection);
                    DOTNETDUPE_API ~SqlCommand() override = default;

                    DOTNETDUPE_API DotNetDupe::System::String GetCommandText() const override;
                    DOTNETDUPE_API void SetCommandText(const DotNetDupe::System::String& sText) override;

                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbParameterCollection> GetParameters() const override;

                    DOTNETDUPE_API DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbDataReader> ExecuteReader() override;
                    DOTNETDUPE_API int ExecuteNonQuery() override;
                    DOTNETDUPE_API DotNetDupe::System::String ExecuteScalar() override;
                };

            }
        }
    }
}

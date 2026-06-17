#pragma once
#include "System/Data/Common/DbParameter.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlParameter : public DotNetDupe::System::Data::Common::DbParameter {
                public:
                    SqlParameter() = default;
                    SqlParameter(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue)
                        : DbParameter(sName, sValue) {}
                    ~SqlParameter() override = default;
                };

            }
        }
    }
}

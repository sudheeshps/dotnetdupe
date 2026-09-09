#pragma once
#include "System/Data/Common/DbParameter.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                /// \class SqlParameter
                /// \brief Represents a parameter to a SqlCommand in the SqlClient provider namespace.
                ///
                /// Modeled after .NET System.Data.SqlClient.SqlParameter. Inherits from DbParameter
                /// to provide typed parameter mapping for SQL database operations.
                class SqlParameter : public DotNetDupe::System::Data::Common::DbParameter {
                public:
                    /// \brief Initializes a new default instance of SqlParameter.
                    SqlParameter() = default;

                    /// \brief Initializes a new instance with a parameter name and string value.
                    /// \param sName The parameter name (e.g. "@id").
                    /// \param sValue The string value.
                    SqlParameter(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue)
                        : DbParameter(sName, sValue) {}

                    /// \brief Virtual destructor for polymorphic cleanup.
                    ~SqlParameter() override = default;
                };

            }
        }
    }
}

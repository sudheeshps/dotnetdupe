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

                /// \class SqlDataReader
                /// \brief Provides a way of reading a forward-only stream of rows from a SQL database.
                ///
                /// \details Complies with ADO.NET DbDataReader specifications. Provides sequential,
                /// forward-only iteration over rows returned by an executed SQL query command,
                /// with column index / ordinal lookup and type conversion routines.
                class SqlDataReader : public DotNetDupe::System::Data::Common::DbDataReader {
                private:
                    struct Impl;
                    DotNetDupe::System::SmartPointer<Impl> m_pImpl;

                public:
                    /// \brief Initializes a new instance of the SqlDataReader class with row and column collections.
                    /// \param rows The collection of data rows.
                    /// \param columns The collection of column names.
                    DOTNETDUPE_API SqlDataReader(Collections::Generic::List<DotNetDupe::System::Data::Internal::Row>&& rows, Collections::Generic::List<DotNetDupe::System::String>&& columns);

                    /// \brief Destructor releasing reader resources.
                    DOTNETDUPE_API ~SqlDataReader() override;

                    /// \brief Advances the SqlDataReader to the next record.
                    /// \return true if there are more rows; otherwise, false.
                    DOTNETDUPE_API bool Read() override;

                    /// \brief Gets the value of the specified column as a string.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The value of the specified column.
                    DOTNETDUPE_API DotNetDupe::System::String GetString(int iOrdinal) override;

                    /// \brief Gets the value of the specified column as a 32-bit signed integer.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The value of the specified column.
                    DOTNETDUPE_API int GetInt32(int iOrdinal) override;

                    /// \brief Gets the value of the specified column as a double-precision floating point number.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The value of the specified column.
                    DOTNETDUPE_API double GetDouble(int iOrdinal) override;

                    /// \brief Gets a value that indicates whether the column contains non-existent or missing values.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return true if the specified column is equivalent to DBNull; otherwise, false.
                    DOTNETDUPE_API bool IsDBNull(int iOrdinal) override;

                    /// \brief Gets the number of columns in the current row.
                    /// \return The number of fields in the record.
                    DOTNETDUPE_API int GetFieldCount() const override;

                    /// \brief Gets the name of the specified column.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The name of the specified column.
                    DOTNETDUPE_API DotNetDupe::System::String GetName(int iOrdinal) override;

                    /// \brief Gets the column ordinal, given the name of the column.
                    /// \param sName The name of the column.
                    /// \return The zero-based column ordinal, or -1 if not found.
                    DOTNETDUPE_API int GetOrdinal(const DotNetDupe::System::String& sName) override;

                    /// \brief Gets the value of the specified column in its native format given the column name.
                    /// \param sName The column name.
                    /// \return The value of the specified column.
                    DOTNETDUPE_API DotNetDupe::System::String operator[](const DotNetDupe::System::String& sName) override;

                    /// \brief Gets the value of the specified column in its native format given the column ordinal.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The value of the specified column.
                    DOTNETDUPE_API DotNetDupe::System::String operator[](int iOrdinal) override;
                };

            }
        }
    }
}

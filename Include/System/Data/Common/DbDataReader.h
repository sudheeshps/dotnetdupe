#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                /// \class DbDataReader
                /// \brief Reads a forward-only stream of rows from a data source.
                ///
                /// Modeled after .NET System.Data.Common.DbDataReader. DbDataReader provides a high-performance,
                /// non-buffered stream for inspecting query results column by column and row by row.
                ///
                /// \note Thread Safety: DbDataReader is forward-only and single-threaded. Simultaneous access
                /// from multiple threads without synchronization is unsupported.
                class DbDataReader : public virtual DotNetDupe::System::Object {
                public:
                    /// \brief Initializes a new instance of the DbDataReader class.
                    DbDataReader() = default;

                    /// \brief Virtual destructor for polymorphic cleanup.
                    ~DbDataReader() override = default;

                    /// \brief Advances the reader to the next record in the result set.
                    /// \return True if there are more rows; otherwise, false.
                    virtual bool Read() = 0;

                    /// \brief Gets the value of the specified column as a string.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The string value of the column.
                    /// \throws DotNetDupe::System::ArgumentOutOfRangeException If ordinal is out of range.
                    virtual DotNetDupe::System::String GetString(int iOrdinal) = 0;

                    /// \brief Gets the value of the specified column as a 32-bit signed integer.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The integer value of the column.
                    /// \throws DotNetDupe::System::FormatException If value cannot be parsed as integer.
                    virtual int GetInt32(int iOrdinal) = 0;

                    /// \brief Gets the value of the specified column as a double-precision floating point number.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The double value of the column.
                    /// \throws DotNetDupe::System::FormatException If value cannot be parsed as double.
                    virtual double GetDouble(int iOrdinal) = 0;

                    /// \brief Gets a value that indicates whether the column contains non-existent or missing values.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return True if the specified column is equivalent to DBNull; otherwise, false.
                    virtual bool IsDBNull(int iOrdinal) = 0;

                    /// \brief Gets the number of columns in the current row.
                    /// \return The field count.
                    virtual int GetFieldCount() const = 0;

                    /// \brief Gets the name of the column, given the zero-based column ordinal.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The name of the specified column.
                    virtual DotNetDupe::System::String GetName(int iOrdinal) = 0;

                    /// \brief Gets the column ordinal, given the name of the column.
                    /// \param sName The name of the column.
                    /// \return The zero-based column ordinal.
                    /// \throws DotNetDupe::System::IndexOutOfRangeException If column name does not exist.
                    virtual int GetOrdinal(const DotNetDupe::System::String& sName) = 0;

                    /// \brief Gets the value of the specified column by column name.
                    /// \param sName The column name.
                    /// \return The string representation of the column value.
                    virtual DotNetDupe::System::String operator[](const DotNetDupe::System::String& sName) = 0;

                    /// \brief Gets the value of the specified column by ordinal index.
                    /// \param iOrdinal The zero-based column ordinal.
                    /// \return The string representation of the column value.
                    virtual DotNetDupe::System::String operator[](int iOrdinal) = 0;
                };

            }
        }
    }
}

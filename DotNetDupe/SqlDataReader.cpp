#include "pch.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Convert.h"
#include "System/ArgumentException.h"
#include "System/Data/Internal/IDatabaseBackend.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                struct SqlDataReader::Impl : public Object {
                    Collections::Generic::List<DotNetDupe::System::Data::Internal::Row> m_rows;
                    Collections::Generic::List<String> m_columns;
                    int m_nCurrentIndex = -1;
                };

                SqlDataReader::SqlDataReader(Collections::Generic::List<DotNetDupe::System::Data::Internal::Row>&& rows, Collections::Generic::List<String>&& columns) 
                    : m_pImpl(DotNetDupe::System::SmartPointer<Impl>::NewShared()) {
                    m_pImpl->m_rows = std::move(rows);
                    m_pImpl->m_columns = std::move(columns);
                }

                SqlDataReader::~SqlDataReader() = default;

                bool SqlDataReader::Read() {
                    /// Advance cursor to next record if available.
                    if (m_pImpl->m_nCurrentIndex + 1 < m_pImpl->m_rows.GetCount()) {
                        m_pImpl->m_nCurrentIndex++;
                        return true;
                    }
                    return false;
                }

                DotNetDupe::System::String SqlDataReader::GetString(int iOrdinal) {
                    /// Guard: Check row and column bounds.
                    if (m_pImpl->m_nCurrentIndex < 0 || m_pImpl->m_nCurrentIndex >= m_pImpl->m_rows.GetCount()) {
                        return "";
                    }
                    if (iOrdinal < 0 || iOrdinal >= m_pImpl->m_columns.GetCount()) {
                        return "";
                    }

                    /// Return cell value string.
                    return m_pImpl->m_rows[m_pImpl->m_nCurrentIndex].Values[iOrdinal];
                }

                int SqlDataReader::GetInt32(int iOrdinal) {
                    /// Convert column string to int.
                    return DotNetDupe::System::Convert::ToInt32(GetString(iOrdinal));
                }

                double SqlDataReader::GetDouble(int iOrdinal) {
                    /// Convert column string to double.
                    return std::stod(GetString(iOrdinal).GetRawString());
                }

                bool SqlDataReader::IsDBNull(int iOrdinal) {
                    /// Check for NULL representation.
                    DotNetDupe::System::String sVal = GetString(iOrdinal);
                    return sVal.IsEmpty() || sVal == "NULL" || sVal == "null";
                }

                int SqlDataReader::GetFieldCount() const {
                    /// Return number of columns.
                    return m_pImpl->m_columns.GetCount();
                }

                DotNetDupe::System::String SqlDataReader::GetName(int iOrdinal) {
                    /// Guard: Check column index bounds.
                    if (iOrdinal < 0 || iOrdinal >= m_pImpl->m_columns.GetCount()) {
                        return "";
                    }
                    return m_pImpl->m_columns[iOrdinal];
                }

                int SqlDataReader::GetOrdinal(const DotNetDupe::System::String& sName) {
                    /// Search for matching column name.
                    for (int i = 0; i < m_pImpl->m_columns.GetCount(); ++i) {
                        if (m_pImpl->m_columns[i] == sName) {
                            return i;
                        }
                    }
                    return -1;
                }

                DotNetDupe::System::String SqlDataReader::operator[](const DotNetDupe::System::String& sName) {
                    /// Lookup column ordinal and retrieve value.
                    int ord = GetOrdinal(sName);
                    if (ord == -1) {
                        throw DotNetDupe::System::ArgumentException(DotNetDupe::System::String("Column not found: ") + sName);
                    }
                    return GetString(ord);
                }

                DotNetDupe::System::String SqlDataReader::operator[](int iOrdinal) {
                    /// Retrieve value by ordinal.
                    return GetString(iOrdinal);
                }

            }
        }
    }
}

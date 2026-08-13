#include "pch.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Convert.h"
#include "System/ArgumentException.h"
#include <vector>
#include <string>
#include "System/Data/Internal/IDatabaseBackend.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                struct SqlDataReader::Impl {
                    std::vector<DotNetDupe::System::Data::Internal::Row> m_rows;
                    std::vector<std::string> m_columns;
                    int m_nCurrentIndex = -1;
                };

                SqlDataReader::SqlDataReader(void* pRows, void* pColumns) : m_pImpl(new Impl()) {
                    if (pRows) {
                        m_pImpl->m_rows = *static_cast<std::vector<DotNetDupe::System::Data::Internal::Row>*>(pRows);
                    }
                    if (pColumns) {
                        m_pImpl->m_columns = *static_cast<std::vector<std::string>*>(pColumns);
                    }
                }

                SqlDataReader::~SqlDataReader() {
                    delete m_pImpl;
                }

                bool SqlDataReader::Read() {
                    if (m_pImpl->m_nCurrentIndex + 1 < static_cast<int>(m_pImpl->m_rows.size())) {
                        m_pImpl->m_nCurrentIndex++;
                        return true;
                    }
                    return false;
                }

                DotNetDupe::System::String SqlDataReader::GetString(int iOrdinal) {
                    if (m_pImpl->m_nCurrentIndex < 0 || m_pImpl->m_nCurrentIndex >= static_cast<int>(m_pImpl->m_rows.size())) {
                        return "";
                    }
                    if (iOrdinal < 0 || iOrdinal >= static_cast<int>(m_pImpl->m_columns.size())) {
                        return "";
                    }
                    return DotNetDupe::System::String(m_pImpl->m_rows[m_pImpl->m_nCurrentIndex].Values[iOrdinal].c_str());
                }

                int SqlDataReader::GetInt32(int iOrdinal) {
                    return DotNetDupe::System::Convert::ToInt32(GetString(iOrdinal));
                }

                double SqlDataReader::GetDouble(int iOrdinal) {
                    return std::stod(GetString(iOrdinal).GetRawString());
                }

                bool SqlDataReader::IsDBNull(int iOrdinal) {
                    return GetString(iOrdinal).IsEmpty();
                }

                int SqlDataReader::GetFieldCount() const {
                    return static_cast<int>(m_pImpl->m_columns.size());
                }

                DotNetDupe::System::String SqlDataReader::GetName(int iOrdinal) {
                    if (iOrdinal < 0 || iOrdinal >= static_cast<int>(m_pImpl->m_columns.size())) {
                        return "";
                    }
                    return DotNetDupe::System::String(m_pImpl->m_columns[iOrdinal].c_str());
                }

                int SqlDataReader::GetOrdinal(const DotNetDupe::System::String& sName) {
                    std::string target = sName.GetRawString();
                    for (size_t i = 0; i < m_pImpl->m_columns.size(); ++i) {
                        if (m_pImpl->m_columns[i] == target) {
                            return static_cast<int>(i);
                        }
                    }
                    return -1;
                }

                DotNetDupe::System::String SqlDataReader::operator[](const DotNetDupe::System::String& sName) {
                    int ord = GetOrdinal(sName);
                    if (ord == -1) {
                        throw DotNetDupe::System::ArgumentException(DotNetDupe::System::String("Column not found: ") + sName);
                    }
                    return GetString(ord);
                }

                DotNetDupe::System::String SqlDataReader::operator[](int iOrdinal) {
                    return GetString(iOrdinal);
                }

            }
        }
    }
}

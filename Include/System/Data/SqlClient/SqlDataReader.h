#pragma once
#include "System/Data/Common/DbDataReader.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/Convert.h"
#include "System/ArgumentException.h"
#include <vector>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlDataReader : public DotNetDupe::System::Data::Common::DbDataReader {
                private:
                    std::vector<DotNetDupe::System::Data::Internal::Row> m_rows;
                    std::vector<std::string> m_columns;
                    int m_nCurrentIndex = -1;

                public:
                    SqlDataReader(const std::vector<DotNetDupe::System::Data::Internal::Row>& rows, const std::vector<std::string>& columns)
                        : m_rows(rows), m_columns(columns) {}
                    ~SqlDataReader() override = default;

                    bool Read() override {
                        if (m_nCurrentIndex + 1 < static_cast<int>(m_rows.size())) {
                            m_nCurrentIndex++;
                            return true;
                        }
                        return false;
                    }

                    DotNetDupe::System::String GetString(int iOrdinal) override {
                        if (m_nCurrentIndex < 0 || m_nCurrentIndex >= static_cast<int>(m_rows.size())) {
                            return "";
                        }
                        if (iOrdinal < 0 || iOrdinal >= static_cast<int>(m_columns.size())) {
                            return "";
                        }
                        return DotNetDupe::System::String(m_rows[m_nCurrentIndex].Values[iOrdinal].c_str());
                    }

                    int GetInt32(int iOrdinal) override {
                        return DotNetDupe::System::Convert::ToInt32(GetString(iOrdinal));
                    }

                    double GetDouble(int iOrdinal) override {
                        return std::stod(GetString(iOrdinal).GetRawString());
                    }

                    bool IsDBNull(int iOrdinal) override {
                        return GetString(iOrdinal).IsEmpty();
                    }

                    int GetFieldCount() const override {
                        return static_cast<int>(m_columns.size());
                    }

                    DotNetDupe::System::String GetName(int iOrdinal) override {
                        if (iOrdinal < 0 || iOrdinal >= static_cast<int>(m_columns.size())) {
                            return "";
                        }
                        return DotNetDupe::System::String(m_columns[iOrdinal].c_str());
                    }

                    int GetOrdinal(const DotNetDupe::System::String& sName) override {
                        std::string target = sName.GetRawString();
                        for (size_t i = 0; i < m_columns.size(); ++i) {
                            if (m_columns[i] == target) {
                                return static_cast<int>(i);
                            }
                        }
                        return -1;
                    }

                    DotNetDupe::System::String operator[](const DotNetDupe::System::String& sName) override {
                        int ord = GetOrdinal(sName);
                        if (ord == -1) {
                            throw DotNetDupe::System::ArgumentException(DotNetDupe::System::String("Column not found: ") + sName);
                        }
                        return GetString(ord);
                    }

                    DotNetDupe::System::String operator[](int iOrdinal) override {
                        return GetString(iOrdinal);
                    }
                };

            }
        }
    }
}

#pragma once
#include "System/Data/Common/DbConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace SqlClient {

                class SqlConnection : public DotNetDupe::System::Data::Common::DbConnection {
                private:
                    DotNetDupe::System::String m_sConnectionString;
                    std::string m_dbName;
                    bool m_bIsOpen = false;

                public:
                    SqlConnection() = default;
                    SqlConnection(const DotNetDupe::System::String& sConnectionString) {
                        SetConnectionString(sConnectionString);
                    }
                    ~SqlConnection() override = default;

                    DotNetDupe::System::String GetConnectionString() const override { return m_sConnectionString; }
                    void SetConnectionString(const DotNetDupe::System::String& sConnStr) override {
                        m_sConnectionString = sConnStr;
                        std::string raw = sConnStr.GetRawString();
                        size_t dbIdx = raw.find("Database=");
                        if (dbIdx == std::string::npos) {
                            dbIdx = raw.find("Data Source=");
                            if (dbIdx != std::string::npos) {
                                dbIdx += 12;
                            }
                        } else {
                            dbIdx += 9;
                        }

                        if (dbIdx != std::string::npos) {
                            size_t endIdx = raw.find(";", dbIdx);
                            if (endIdx != std::string::npos) {
                                m_dbName = raw.substr(dbIdx, endIdx - dbIdx);
                            } else {
                                m_dbName = raw.substr(dbIdx);
                            }
                        } else {
                            m_dbName = "DefaultDb";
                        }
                    }

                    std::string GetDatabaseName() const { return m_dbName; }
                    bool IsOpen() const { return m_bIsOpen; }

                    void Open() override {
                        m_bIsOpen = true;
                    }
                    void Close() override {
                        m_bIsOpen = false;
                    }

                    DotNetDupe::System::SmartPointer<DotNetDupe::System::Data::Common::DbCommand> CreateCommand() override {
                        return DotNetDupe::System::SmartPointer<SqlCommand>::NewShared("", this);
                    }
                };

            }
        }
    }
}

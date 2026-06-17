#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Convert.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Data {
            namespace Common {

                class DbParameter : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::String m_sParameterName;
                    DotNetDupe::System::String m_sValue;

                public:
                    DbParameter() = default;
                    DbParameter(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue)
                        : m_sParameterName(sName), m_sValue(sValue) {}
                    ~DbParameter() override = default;

                    DotNetDupe::System::String GetParameterName() const { return m_sParameterName; }
                    void SetParameterName(const DotNetDupe::System::String& sName) { m_sParameterName = sName; }

                    DotNetDupe::System::String GetValue() const { return m_sValue; }
                    void SetValue(const DotNetDupe::System::String& sValue) { m_sValue = sValue; }
                };

                class DbParameterCollection : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::Collections::Generic::List<DotNetDupe::System::SmartPointer<DbParameter>> m_parameters;

                public:
                    DbParameterCollection() = default;
                    ~DbParameterCollection() override = default;

                    int GetCount() const { return m_parameters.GetCount(); }

                    void Add(const DotNetDupe::System::SmartPointer<DbParameter>& parameter) {
                        m_parameters.Add(parameter);
                    }

                    void AddWithValue(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, sValue));
                    }

                    void AddWithValue(const DotNetDupe::System::String& sName, int iValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, DotNetDupe::System::Convert::ToString(iValue)));
                    }

                    void AddWithValue(const DotNetDupe::System::String& sName, double dValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, DotNetDupe::System::Convert::ToString(dValue)));
                    }

                    DotNetDupe::System::SmartPointer<DbParameter> GetAt(int index) const {
                        return m_parameters[index];
                    }

                    void Clear() {
                        m_parameters.Clear();
                    }
                };

            }
        }
    }
}

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

                /// \class DbParameter
                /// \brief Represents a parameter to a DbCommand and optionally its mapping to DataSet columns.
                ///
                /// Modeled after .NET System.Data.Common.DbParameter. Encapsulates parameter names
                /// and associated string values for parameterized query execution to prevent SQL injection.
                class DbParameter : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::String m_sParameterName;
                    DotNetDupe::System::String m_sValue;

                public:
                    /// \brief Initializes a new default instance of DbParameter.
                    DbParameter() = default;

                    /// \brief Initializes a new instance of DbParameter with a name and value.
                    /// \param sName The name of the parameter (e.g. "@id").
                    /// \param sValue The string value of the parameter.
                    DbParameter(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue)
                        : m_sParameterName(sName), m_sValue(sValue) {}

                    /// \brief Virtual destructor for polymorphic cleanup.
                    ~DbParameter() override = default;

                    /// \brief Gets the name of the DbParameter.
                    /// \return The parameter name.
                    DotNetDupe::System::String GetParameterName() const { return m_sParameterName; }

                    /// \brief Sets the name of the DbParameter.
                    /// \param sName The parameter name.
                    void SetParameterName(const DotNetDupe::System::String& sName) { m_sParameterName = sName; }

                    /// \brief Gets the value of the parameter.
                    /// \return The string representation of the parameter value.
                    DotNetDupe::System::String GetValue() const { return m_sValue; }

                    /// \brief Sets the value of the parameter.
                    /// \param sValue The string representation of the parameter value.
                    void SetValue(const DotNetDupe::System::String& sValue) { m_sValue = sValue; }
                };

                /// \class DbParameterCollection
                /// \brief Represents a collection of parameters associated with a DbCommand.
                ///
                /// Modeled after .NET System.Data.Common.DbParameterCollection. Manages ordered
                /// parameters and provides convenience overloads for adding parameters with values.
                class DbParameterCollection : public virtual DotNetDupe::System::Object {
                private:
                    DotNetDupe::System::Collections::Generic::List<DotNetDupe::System::SmartPointer<DbParameter>> m_parameters;

                public:
                    /// \brief Initializes a new instance of DbParameterCollection.
                    DbParameterCollection() = default;

                    /// \brief Virtual destructor.
                    ~DbParameterCollection() override = default;

                    /// \brief Gets the number of DbParameter objects in the collection.
                    /// \return The element count.
                    int GetCount() const { return m_parameters.GetCount(); }

                    /// \brief Adds the specified DbParameter object to the DbParameterCollection.
                    /// \param pParameter The DbParameter to add.
                    void Add(const DotNetDupe::System::SmartPointer<DbParameter>& pParameter) {
                        m_parameters.Add(pParameter);
                    }

                    /// \brief Adds a parameter with a specified name and string value.
                    /// \param sName The parameter name.
                    /// \param sValue The string value.
                    void AddWithValue(const DotNetDupe::System::String& sName, const DotNetDupe::System::String& sValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, sValue));
                    }

                    /// \brief Adds a parameter with a specified name and 32-bit integer value.
                    /// \param sName The parameter name.
                    /// \param iValue The integer value.
                    void AddWithValue(const DotNetDupe::System::String& sName, int iValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, DotNetDupe::System::Convert::ToString(iValue)));
                    }

                    /// \brief Adds a parameter with a specified name and double value.
                    /// \param sName The parameter name.
                    /// \param dValue The double value.
                    void AddWithValue(const DotNetDupe::System::String& sName, double dValue) {
                        m_parameters.Add(DotNetDupe::System::SmartPointer<DbParameter>::NewShared(sName, DotNetDupe::System::Convert::ToString(dValue)));
                    }

                    /// \brief Gets the DbParameter object at the specified index.
                    /// \param index The zero-based index of the parameter to retrieve.
                    /// \return A smart pointer to the retrieved DbParameter.
                    DotNetDupe::System::SmartPointer<DbParameter> GetAt(int index) const {
                        return m_parameters[index];
                    }

                    /// \brief Removes all DbParameter objects from the DbParameterCollection.
                    void Clear() {
                        m_parameters.Clear();
                    }
                };

            }
        }
    }
}

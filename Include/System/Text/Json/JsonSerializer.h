/// \file JsonSerializer.h
/// \brief Provides functionality to serialize objects to JSON strings and deserialize JSON strings into objects.
///
/// Standard Citation: RFC 8259 The JavaScript Object Notation (JSON) Data Interchange Format.

#pragma once

#include "Common.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/DateTime.h"
#include "System/Guid.h"
#include "System/Text/Json/JsonElement.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                /// \struct JsonConverter
                /// \brief Primary template for type-specific JSON serialization converters.
                /// \tparam T Type to serialize or deserialize.
                template <typename T, typename Enable = void>
                struct JsonConverter;

                template <typename T, typename = void>
                struct has_to_json : std::false_type {};

                template <typename T>
                struct has_to_json<T, std::void_t<decltype(std::declval<const T&>().ToJson())>> : std::true_type {};

                template <typename T, typename = void>
                struct has_static_from_json : std::false_type {};

                template <typename T>
                struct has_static_from_json<T, std::void_t<decltype(T::FromJson(std::declval<const JsonElement&>()))>> : std::true_type {};

                template <typename T, typename = void>
                struct has_member_from_json : std::false_type {};

                template <typename T>
                struct has_member_from_json<T, std::void_t<decltype(std::declval<T&>().FromJson(std::declval<const JsonElement&>()))>> : std::true_type {};

                /// \brief Generic converter for types implementing ToJson() and static FromJson(const JsonElement&).
                template <typename T>
                struct JsonConverter<T, std::enable_if_t<has_to_json<T>::value && has_static_from_json<T>::value>> {
                    static JsonElement Write(const T& value) {
                        return value.ToJson();
                    }
                    static T Read(const JsonElement& element) {
                        return T::FromJson(element);
                    }
                };

                /// \brief Generic converter for types implementing ToJson() and member FromJson(const JsonElement&).
                template <typename T>
                struct JsonConverter<T, std::enable_if_t<has_to_json<T>::value && !has_static_from_json<T>::value && has_member_from_json<T>::value>> {
                    static JsonElement Write(const T& value) {
                        return value.ToJson();
                    }
                    static T Read(const JsonElement& element) {
                        T obj;
                        obj.FromJson(element);
                        return obj;
                    }
                };

                /// \class JsonSerializer
                /// \brief Provides functionality to serialize objects to JSON strings and deserialize JSON strings to objects.
                ///
                /// \details Complies with RFC 8259 and ECMA-404 specifications. Converts C++ objects and primitives
                /// into JSON text and deserializes JSON text into strongly typed C++ objects via JsonConverter specializations.
                class JsonSerializer {
                public:
                    /// \brief Converts the value of a type specified by a generic type parameter into a JSON string.
                    /// \tparam T The type of the value to serialize.
                    /// \param value The value to convert.
                    /// \return A JSON string representation of the value.
                    template <typename T>
                    static String Serialize(const T& value) {
                        JsonElement element = JsonConverter<T>::Write(value);
                        return element.ToString();
                    }

                    /// \brief Converts the value of a type specified by a generic type parameter into a JsonElement.
                    /// \tparam T The type of the value to serialize.
                    /// \param value The value to convert.
                    /// \return A JsonElement representation of the value.
                    template <typename T>
                    static JsonElement SerializeToElement(const T& value) {
                        return JsonConverter<T>::Write(value);
                    }

                    /// \brief Parses the text representing a single JSON value into an instance of the type specified by a generic type parameter.
                    /// \tparam T The target type of the JSON value.
                    /// \param sJson The JSON text to parse.
                    /// \return A T representation of the JSON value.
                    template <typename T>
                    static T Deserialize(const String& sJson) {
                        JsonElement element = JsonElement::Parse(sJson);
                        return JsonConverter<T>::Read(element);
                    }

                    /// \brief Deserializes the JsonElement into an instance of type T.
                    /// \tparam T Target type.
                    /// \param element JsonElement to deserialize.
                    /// \return Deserialized object of type T.
                    template <typename T>
                    static T Deserialize(const JsonElement& element) {
                        return JsonConverter<T>::Read(element);
                    }
                };

                // Specializations for Primitive types
                
                /// \brief JsonConverter specialization for int.
                template <>
                struct JsonConverter<int> {
                    static JsonElement Write(const int& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static int Read(const JsonElement& element) {
                        return element.GetInt32();
                    }
                };

                /// \brief JsonConverter specialization for short.
                template <>
                struct JsonConverter<short> {
                    static JsonElement Write(const short& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static short Read(const JsonElement& element) {
                        return static_cast<short>(element.GetInt32());
                    }
                };

                /// \brief JsonConverter specialization for unsigned short.
                template <>
                struct JsonConverter<unsigned short> {
                    static JsonElement Write(const unsigned short& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static unsigned short Read(const JsonElement& element) {
                        return static_cast<unsigned short>(element.GetInt32());
                    }
                };

                /// \brief JsonConverter specialization for unsigned int.
                template <>
                struct JsonConverter<unsigned int> {
                    static JsonElement Write(const unsigned int& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static unsigned int Read(const JsonElement& element) {
                        return static_cast<unsigned int>(element.GetInt64());
                    }
                };

                /// \brief JsonConverter specialization for long long.
                template <>
                struct JsonConverter<long long> {
                    static JsonElement Write(const long long& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static long long Read(const JsonElement& element) {
                        return element.GetInt64();
                    }
                };

                /// \brief JsonConverter specialization for unsigned long long.
                template <>
                struct JsonConverter<unsigned long long> {
                    static JsonElement Write(const unsigned long long& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static unsigned long long Read(const JsonElement& element) {
                        return static_cast<unsigned long long>(element.GetInt64());
                    }
                };

                /// \brief JsonConverter specialization for double.
                template <>
                struct JsonConverter<double> {
                    static JsonElement Write(const double& value) {
                        return JsonElement(value);
                    }
                    static double Read(const JsonElement& element) {
                        return element.GetDouble();
                    }
                };

                /// \brief JsonConverter specialization for float.
                template <>
                struct JsonConverter<float> {
                    static JsonElement Write(const float& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static float Read(const JsonElement& element) {
                        return static_cast<float>(element.GetDouble());
                    }
                };

                /// \brief JsonConverter specialization for bool.
                template <>
                struct JsonConverter<bool> {
                    static JsonElement Write(const bool& value) {
                        return JsonElement(value);
                    }
                    static bool Read(const JsonElement& element) {
                        return element.GetBoolean();
                    }
                };

                /// \brief JsonConverter specialization for char.
                template <>
                struct JsonConverter<char> {
                    static JsonElement Write(const char& value) {
                        char szBuf[2] = { value, '\0' };
                        return JsonElement(String(szBuf));
                    }
                    static char Read(const JsonElement& element) {
                        String sStr = element.GetString();
                        return (sStr.GetLength() > 0) ? sStr[0] : '\0';
                    }
                };

                /// \brief JsonConverter specialization for unsigned char.
                template <>
                struct JsonConverter<unsigned char> {
                    static JsonElement Write(const unsigned char& value) {
                        return JsonElement(static_cast<double>(value));
                    }
                    static unsigned char Read(const JsonElement& element) {
                        return static_cast<unsigned char>(element.GetInt32());
                    }
                };

                /// \brief JsonConverter specialization for String.
                template <>
                struct JsonConverter<String> {
                    static JsonElement Write(const String& value) {
                        return JsonElement(value);
                    }
                    static String Read(const JsonElement& element) {
                        return element.GetString();
                    }
                };

                /// \brief JsonConverter specialization for DateTime.
                template <>
                struct JsonConverter<DateTime> {
                    static JsonElement Write(const DateTime& value) {
                        return JsonElement(value.ToString());
                    }
                    static DateTime Read(const JsonElement& element) {
                        return DateTime::Parse(element.GetString());
                    }
                };

                /// \brief JsonConverter specialization for Guid.
                template <>
                struct JsonConverter<Guid> {
                    static JsonElement Write(const Guid& value) {
                        return JsonElement(value.ToString());
                    }
                    static Guid Read(const JsonElement& element) {
                        return Guid(element.GetString());
                    }
                };

                /// \brief JsonConverter specialization for Array<U>.
                template <typename U>
                struct JsonConverter<Array<U>> {
                    static JsonElement Write(const Array<U>& value) {
                        JsonElement arr(JsonValueKind::Array);
                        for (int i = 0; i < value.GetLength(); ++i) {
                            arr.AddArrayElement(JsonConverter<U>::Write(value[i]));
                        }
                        return arr;
                    }
                    static Array<U> Read(const JsonElement& element) {
                        int iLen = element.GetArrayLength();
                        Array<U> arr(iLen);
                        for (int i = 0; i < iLen; ++i) {
                            arr[i] = JsonConverter<U>::Read(element.GetArrayElement(i));
                        }
                        return arr;
                    }
                };

                /// \brief JsonConverter specialization for List<U>.
                template <typename U>
                struct JsonConverter<Collections::Generic::List<U>> {
                    static JsonElement Write(const Collections::Generic::List<U>& value) {
                        JsonElement arr(JsonValueKind::Array);
                        for (int i = 0; i < value.GetCount(); ++i) {
                            arr.AddArrayElement(JsonConverter<U>::Write(value[i]));
                        }
                        return arr;
                    }
                    static Collections::Generic::List<U> Read(const JsonElement& element) {
                        Collections::Generic::List<U> lst;
                        int iLen = element.GetArrayLength();
                        for (int i = 0; i < iLen; ++i) {
                            lst.Add(JsonConverter<U>::Read(element.GetArrayElement(i)));
                        }
                        return lst;
                    }
                };

                /// \brief JsonConverter specialization for Dictionary<String, U>.
                template <typename U>
                struct JsonConverter<Collections::Generic::Dictionary<String, U>> {
                    static JsonElement Write(const Collections::Generic::Dictionary<String, U>& value) {
                        JsonElement obj(JsonValueKind::Object);
                        auto keys = value.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            obj.SetProperty(keys[i], JsonConverter<U>::Write(value[keys[i]]));
                        }
                        return obj;
                    }
                    static Collections::Generic::Dictionary<String, U> Read(const JsonElement& element) {
                        Collections::Generic::Dictionary<String, U> dict;
                        auto keys = element.GetPropertyNames();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            JsonElement prop;
                            if (element.TryGetProperty(keys[i], prop)) {
                                dict.Add(keys[i], JsonConverter<U>::Read(prop));
                            }
                        }
                        return dict;
                    }
                };

                template <typename T>
                inline T JsonElement::Deserialize() const {
                    return JsonConverter<T>::Read(*this);
                }

                template <typename T>
                inline T JsonElement::GetPropertyAs(const String& sPropertyName) const {
                    return GetProperty(sPropertyName).Deserialize<T>();
                }

                template <typename T>
                inline T JsonElement::GetPropertyOrDefault(const String& sPropertyName, const T& defaultValue) const {
                    JsonElement objVal;
                    if (!TryGetProperty(sPropertyName, objVal)) {
                        return defaultValue;
                    }
                    return objVal.Deserialize<T>();
                }

                template <typename T>
                inline bool JsonElement::TryGetPropertyAs(const String& sPropertyName, T& outValue) const {
                    JsonElement objVal;
                    if (!TryGetProperty(sPropertyName, objVal)) {
                        return false;
                    }
                    outValue = objVal.Deserialize<T>();
                    return true;
                }

            }
        }
    }
}

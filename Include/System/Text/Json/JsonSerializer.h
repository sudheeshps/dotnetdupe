/// \file JsonSerializer.h
/// \brief Provides functionality to serialize objects to JSON strings and deserialize JSON strings into objects.
///
/// Standard Citation: RFC 8259 The JavaScript Object Notation (JSON) Data Interchange Format.

#pragma once

#include "Common.h"
#include "System/String.h"
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

                    /// \brief Parses the text representing a single JSON value into an instance of the type specified by a generic type parameter.
                    /// \tparam T The target type of the JSON value.
                    /// \param sJson The JSON text to parse.
                    /// \return A T representation of the JSON value.
                    template <typename T>
                    static T Deserialize(const String& sJson) {
                        JsonElement element = JsonElement::Parse(sJson);
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

            }
        }
    }
}

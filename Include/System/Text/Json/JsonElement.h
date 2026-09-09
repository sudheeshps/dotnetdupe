#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                /// \enum JsonValueKind
                /// \brief Specifies the data type of a JSON value.
                enum class JsonValueKind {
                    Undefined,
                    Object,
                    Array,
                    String,
                    Number,
                    True,
                    False,
                    Null
                };

                class JsonElementImpl;

                /// \class JsonElement
                /// \brief Represents a specific JSON value within a JsonDocument or object hierarchy.
                ///
                /// \details Complies with RFC 8259 and ECMA-404 specifications. Provides type-safe,
                /// DOM-style JSON value inspection and manipulation for primitives, arrays, and nested objects.
                class JsonElement : public Object {
                public:
                    /// \brief Initializes a new instance of the JsonElement class representing undefined.
                    DOTNETDUPE_API JsonElement();

                    /// \brief Destructor releasing element resources.
                    DOTNETDUPE_API ~JsonElement();

                    DOTNETDUPE_API JsonElement(const JsonElement& objOther);
                    DOTNETDUPE_API JsonElement& operator=(const JsonElement& objOther);
                    DOTNETDUPE_API JsonElement(JsonElement&& objOther) noexcept;
                    DOTNETDUPE_API JsonElement& operator=(JsonElement&& objOther) noexcept;

                    /// \brief Initializes a new instance of the JsonElement class with a specified kind.
                    /// \param eKind The JsonValueKind of the element.
                    DOTNETDUPE_API explicit JsonElement(JsonValueKind eKind);

                    /// \brief Initializes a new boolean JsonElement.
                    /// \param bValue The boolean value.
                    DOTNETDUPE_API explicit JsonElement(bool bValue);

                    /// \brief Initializes a new numeric JsonElement.
                    /// \param dValue The numeric floating-point value.
                    DOTNETDUPE_API explicit JsonElement(double dValue);

                    /// \brief Initializes a new string JsonElement.
                    /// \param sValue The string value.
                    DOTNETDUPE_API explicit JsonElement(const String& sValue);

                    /// \brief Initializes a new null JsonElement.
                    DOTNETDUPE_API explicit JsonElement(std::nullptr_t);

                    /// \brief Gets the type of the current JSON value.
                    /// \return The type of the current JSON value.
                    DOTNETDUPE_API JsonValueKind GetValueKind() const;

                    /// \brief Gets the value of the element as a Boolean.
                    /// \return The value of the element as a Boolean.
                    /// \throws InvalidOperationException If kind is not True or False.
                    DOTNETDUPE_API bool GetBoolean() const;

                    /// \brief Gets the current JSON number as a double.
                    /// \return The current JSON number as a double.
                    /// \throws InvalidOperationException If kind is not Number.
                    DOTNETDUPE_API double GetDouble() const;

                    /// \brief Gets the current JSON number as a 32-bit signed integer.
                    /// \return The current JSON number as an int.
                    /// \throws InvalidOperationException If kind is not Number.
                    DOTNETDUPE_API int GetInt32() const;

                    /// \brief Gets the current JSON number as a 64-bit signed integer.
                    /// \return The current JSON number as a long long.
                    /// \throws InvalidOperationException If kind is not Number.
                    DOTNETDUPE_API long long GetInt64() const;

                    /// \brief Gets the value of the element as a string.
                    /// \return The value of the element as a string.
                    /// \throws InvalidOperationException If kind is not String.
                    DOTNETDUPE_API String GetString() const;

                    /// \brief Gets the number of values contained within the current JSON array value.
                    /// \return The number of values contained within the current array.
                    /// \throws InvalidOperationException If kind is not Array.
                    DOTNETDUPE_API int GetArrayLength() const;

                    /// \brief Gets the value at the specified index in the current JSON array.
                    /// \param iIndex The zero-based index of the element to get.
                    /// \return The JsonElement at the specified index.
                    DOTNETDUPE_API JsonElement GetArrayElement(int iIndex) const;

                    /// \brief Adds an element to the current JSON array.
                    /// \param objElement The element to append.
                    DOTNETDUPE_API void AddArrayElement(const JsonElement& objElement);

                    /// \brief Looks for a property named propertyName in the current JSON object.
                    /// \param sPropertyName The name of the property to find.
                    /// \param objValue When this method returns, contains the value of the specified property.
                    /// \return true if the property was found; otherwise, false.
                    DOTNETDUPE_API bool TryGetProperty(const String& sPropertyName, JsonElement& objValue) const;

                    /// \brief Sets or replaces a property on the current JSON object.
                    /// \param sPropertyName The name of the property to set.
                    /// \param objValue The value of the property.
                    DOTNETDUPE_API void SetProperty(const String& sPropertyName, const JsonElement& objValue);

                    /// \brief Gets an array containing the names of all properties on the current JSON object.
                    /// \return An array of property names.
                    DOTNETDUPE_API Array<String> GetPropertyNames() const;

                    /// \brief Serializes the element into a JSON string representation.
                    /// \return A JSON string.
                    DOTNETDUPE_API String ToString() const;

                    /// \brief Parses text representing a single JSON value into a JsonElement.
                    /// \param sJson The JSON text to parse.
                    /// \return A JsonElement representation of the value.
                    /// \throws JsonException If parsing fails.
                    DOTNETDUPE_API static JsonElement Parse(const String& sJson);

                private:
                    SmartPointer<JsonElementImpl> m_pImpl;
                };

            }
        }
    }
}

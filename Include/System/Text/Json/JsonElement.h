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

                class JsonElement : public Object {
                public:
                    DOTNETDUPE_API JsonElement();
                    DOTNETDUPE_API ~JsonElement();

                    DOTNETDUPE_API JsonElement(const JsonElement& objOther);
                    DOTNETDUPE_API JsonElement& operator=(const JsonElement& objOther);
                    DOTNETDUPE_API JsonElement(JsonElement&& objOther) noexcept;
                    DOTNETDUPE_API JsonElement& operator=(JsonElement&& objOther) noexcept;

                    DOTNETDUPE_API explicit JsonElement(JsonValueKind eKind);
                    DOTNETDUPE_API explicit JsonElement(bool bValue);
                    DOTNETDUPE_API explicit JsonElement(double dValue);
                    DOTNETDUPE_API explicit JsonElement(const String& sValue);
                    DOTNETDUPE_API explicit JsonElement(std::nullptr_t);

                    DOTNETDUPE_API JsonValueKind GetValueKind() const;

                    DOTNETDUPE_API bool GetBoolean() const;
                    DOTNETDUPE_API double GetDouble() const;
                    DOTNETDUPE_API int GetInt32() const;
                    DOTNETDUPE_API long long GetInt64() const;
                    DOTNETDUPE_API String GetString() const;

                    DOTNETDUPE_API int GetArrayLength() const;
                    DOTNETDUPE_API JsonElement GetArrayElement(int iIndex) const;
                    DOTNETDUPE_API void AddArrayElement(const JsonElement& objElement);

                    DOTNETDUPE_API bool TryGetProperty(const String& sPropertyName, JsonElement& objValue) const;
                    DOTNETDUPE_API void SetProperty(const String& sPropertyName, const JsonElement& objValue);
                    DOTNETDUPE_API Array<String> GetPropertyNames() const;

                    DOTNETDUPE_API String ToString() const;

                    DOTNETDUPE_API static JsonElement Parse(const String& sJson);

                private:
                    SmartPointer<JsonElementImpl> m_pImpl;
                };

            }
        }
    }
}

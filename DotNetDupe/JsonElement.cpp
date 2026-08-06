#include "pch.h"
#include "System/Text/Json/JsonElement.h"
#include "System/Text/Json/JsonException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Text/StringBuilder.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <string>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                class JsonElementImpl {
                public:
                    JsonValueKind eKind = JsonValueKind::Undefined;
                    bool bBoolValue = false;
                    double dNumValue = 0.0;
                    String sStrValue;
                    Collections::Generic::List<JsonElement> lstArray;
                    Collections::Generic::Dictionary<String, JsonElement> dictObject;

                    JsonElementImpl() = default;
                    JsonElementImpl(JsonValueKind kind) : eKind(kind) {}
                };

                static String EscapeString(const String& sInput) {
                    std::string sResult = "\"";
                    const char* pRaw = sInput.GetRawString();
                    while (*pRaw) {
                        char c = *pRaw;
                        switch (c) {
                            case '\"': sResult += "\\\""; break;
                            case '\\': sResult += "\\\\"; break;
                            case '\b': sResult += "\\b"; break;
                            case '\f': sResult += "\\f"; break;
                            case '\n': sResult += "\\n"; break;
                            case '\r': sResult += "\\r"; break;
                            case '\t': sResult += "\\t"; break;
                            default:
                                if (static_cast<unsigned char>(c) < 32) {
                                    std::stringstream ss;
                                    ss << "\\u" << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(c);
                                    sResult += ss.str();
                                } else {
                                    sResult += c;
                                }
                                break;
                        }
                        pRaw++;
                    }
                    sResult += "\"";
                    return String(sResult.c_str());
                }

                class JsonParser {
                public:
                    static JsonElement Parse(const String& sJson) {
                        std::string s = sJson.GetRawString();
                        size_t index = 0;
                        SkipWhitespace(s, index);
                        JsonElement res = ParseValue(s, index);
                        SkipWhitespace(s, index);
                        if (index < s.length()) {
                            throw JsonException("Extra characters after JSON value");
                        }
                        return res;
                    }

                private:
                    static void SkipWhitespace(const std::string& s, size_t& index) {
                        while (index < s.length() && (s[index] == ' ' || s[index] == '\t' || s[index] == '\n' || s[index] == '\r')) {
                            index++;
                        }
                    }

                    static JsonElement ParseValue(const std::string& s, size_t& index) {
                        SkipWhitespace(s, index);
                        if (index >= s.length()) {
                            throw JsonException("Unexpected end of JSON input");
                        }
                        char c = s[index];
                        if (c == '{') {
                            return ParseObject(s, index);
                        } else if (c == '[') {
                            return ParseArray(s, index);
                        } else if (c == '\"') {
                            return ParseString(s, index);
                        } else if (c == 't' || c == 'f') {
                            return ParseBool(s, index);
                        } else if (c == 'n') {
                            return ParseNull(s, index);
                        } else if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
                            return ParseNumber(s, index);
                        } else {
                            throw JsonException((std::string("Unexpected character: ") + c).c_str());
                        }
                    }

                    static JsonElement ParseObject(const std::string& s, size_t& index) {
                        index++; // Skip '{'
                        JsonElement obj(JsonValueKind::Object);
                        SkipWhitespace(s, index);
                        if (index < s.length() && s[index] == '}') {
                            index++; // Empty object
                            return obj;
                        }
                        while (true) {
                            SkipWhitespace(s, index);
                            if (index >= s.length() || s[index] != '\"') {
                                throw JsonException("Expected string key in object");
                            }
                            JsonElement keyEl = ParseString(s, index);
                            String key = keyEl.GetString();
                            SkipWhitespace(s, index);
                            if (index >= s.length() || s[index] != ':') {
                                throw JsonException("Expected ':' after key in object");
                            }
                            index++; // Skip ':'
                            JsonElement val = ParseValue(s, index);
                            obj.SetProperty(key, val);
                            SkipWhitespace(s, index);
                            if (index < s.length() && s[index] == '}') {
                                index++;
                                break;
                            }
                            if (index >= s.length() || s[index] != ',') {
                                throw JsonException("Expected ',' or '}' in object");
                            }
                            index++; // Skip ','
                        }
                        return obj;
                    }

                    static JsonElement ParseArray(const std::string& s, size_t& index) {
                        index++; // Skip '['
                        JsonElement arr(JsonValueKind::Array);
                        SkipWhitespace(s, index);
                        if (index < s.length() && s[index] == ']') {
                            index++; // Empty array
                            return arr;
                        }
                        while (true) {
                            JsonElement val = ParseValue(s, index);
                            arr.AddArrayElement(val);
                            SkipWhitespace(s, index);
                            if (index < s.length() && s[index] == ']') {
                                index++;
                                break;
                            }
                            if (index >= s.length() || s[index] != ',') {
                                throw JsonException("Expected ',' or ']' in array");
                            }
                            index++; // Skip ','
                        }
                        return arr;
                    }

                    static void ParseUnicodeEscape(const std::string& s, size_t& index, std::string& res) {
                        if (index + 4 > s.length()) {
                            throw JsonException("Invalid unicode escape sequence");
                        }

                        std::string hexStr = s.substr(index, 4);
                        index += 4;
                        unsigned int codePoint = std::stoul(hexStr, nullptr, 16);

                        if (codePoint <= 0x7f) {
                            res += static_cast<char>(codePoint);
                        } else if (codePoint <= 0x7ff) {
                            res += static_cast<char>(0xc0 | ((codePoint >> 6) & 0x1f));
                            res += static_cast<char>(0x80 | (codePoint & 0x3f));
                        } else {
                            res += static_cast<char>(0xe0 | ((codePoint >> 12) & 0x0f));
                            res += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3f));
                            res += static_cast<char>(0x80 | (codePoint & 0x3f));
                        }
                    }

                    static JsonElement ParseString(const std::string& s, size_t& index) {
                        index++; // Skip starting '\"'
                        std::string res;

                        while (index < s.length()) {
                            char c = s[index];
                            if (c == '\"') {
                                index++;
                                return JsonElement(String(res.c_str()));
                            }

                            if (c == '\\') {
                                if (index + 1 >= s.length()) {
                                    throw JsonException("Unterminated escape sequence in string");
                                }
                                char escaped = s[index + 1];
                                index += 2;

                                switch (escaped) {
                                    case '\"': res += '\"'; break;
                                    case '\\': res += '\\'; break;
                                    case '/': res += '/'; break;
                                    case 'b': res += '\b'; break;
                                    case 'f': res += '\f'; break;
                                    case 'n': res += '\n'; break;
                                    case 'r': res += '\r'; break;
                                    case 't': res += '\t'; break;
                                    case 'u': ParseUnicodeEscape(s, index, res); break;
                                    default:
                                        throw JsonException((std::string("Unknown escape sequence: \\") + escaped).c_str());
                                }
                            } else {
                                res += c;
                                index++;
                            }
                        }

                        throw JsonException("Unterminated string");
                    }

                    static JsonElement ParseBool(const std::string& s, size_t& index) {
                        if (s.compare(index, 4, "true") == 0) {
                            index += 4;
                            return JsonElement(true);
                        } else if (s.compare(index, 5, "false") == 0) {
                            index += 5;
                            return JsonElement(false);
                        }
                        throw JsonException("Expected boolean value");
                    }

                    static JsonElement ParseNull(const std::string& s, size_t& index) {
                        if (s.compare(index, 4, "null") == 0) {
                            index += 4;
                            return JsonElement(nullptr);
                        }
                        throw JsonException("Expected null value");
                    }

                    static JsonElement ParseNumber(const std::string& s, size_t& index) {
                        size_t start = index;
                        if (s[index] == '-') {
                            index++;
                        }
                        while (index < s.length() && (std::isdigit(static_cast<unsigned char>(s[index])) || s[index] == '.' || s[index] == 'e' || s[index] == 'E' || s[index] == '+' || s[index] == '-')) {
                            index++;
                        }
                        std::string numStr = s.substr(start, index - start);
                        try {
                            double val = std::stod(numStr);
                            return JsonElement(val);
                        } catch (...) {
                            throw JsonException(("Invalid number format: " + numStr).c_str());
                        }
                    }
                };

                JsonElement::JsonElement() 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(JsonValueKind::Undefined)) {
                }

                JsonElement::~JsonElement() {
                }

                JsonElement::JsonElement(const JsonElement& objOther) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New()) {
                    if (objOther.m_pImpl) {
                        m_pImpl->eKind = objOther.m_pImpl->eKind;
                        m_pImpl->bBoolValue = objOther.m_pImpl->bBoolValue;
                        m_pImpl->dNumValue = objOther.m_pImpl->dNumValue;
                        m_pImpl->sStrValue = objOther.m_pImpl->sStrValue;
                        
                        for (int i = 0; i < objOther.m_pImpl->lstArray.GetCount(); ++i) {
                            m_pImpl->lstArray.Add(objOther.m_pImpl->lstArray[i]);
                        }
                        
                        auto keys = objOther.m_pImpl->dictObject.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            m_pImpl->dictObject.Add(keys[i], objOther.m_pImpl->dictObject[keys[i]]);
                        }
                    }
                }

                JsonElement& JsonElement::operator=(const JsonElement& objOther) {
                    if (this != &objOther) {
                        m_pImpl = SmartPointer<JsonElementImpl>::New();
                        if (objOther.m_pImpl) {
                            m_pImpl->eKind = objOther.m_pImpl->eKind;
                            m_pImpl->bBoolValue = objOther.m_pImpl->bBoolValue;
                            m_pImpl->dNumValue = objOther.m_pImpl->dNumValue;
                            m_pImpl->sStrValue = objOther.m_pImpl->sStrValue;
                            
                            for (int i = 0; i < objOther.m_pImpl->lstArray.GetCount(); ++i) {
                                m_pImpl->lstArray.Add(objOther.m_pImpl->lstArray[i]);
                            }
                            
                            auto keys = objOther.m_pImpl->dictObject.GetKeys();
                            for (int i = 0; i < keys.GetLength(); ++i) {
                                m_pImpl->dictObject.Add(keys[i], objOther.m_pImpl->dictObject[keys[i]]);
                            }
                        }
                    }
                    return *this;
                }

                JsonElement::JsonElement(JsonElement&& objOther) noexcept 
                    : m_pImpl(std::move(objOther.m_pImpl)) {
                }

                JsonElement& JsonElement::operator=(JsonElement&& objOther) noexcept {
                    if (this != &objOther) {
                        m_pImpl = std::move(objOther.m_pImpl);
                    }
                    return *this;
                }

                JsonElement::JsonElement(JsonValueKind eKind) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(eKind)) {
                }

                JsonElement::JsonElement(bool bValue) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(bValue ? JsonValueKind::True : JsonValueKind::False)) {
                    m_pImpl->bBoolValue = bValue;
                }

                JsonElement::JsonElement(double dValue) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(JsonValueKind::Number)) {
                    m_pImpl->dNumValue = dValue;
                }

                JsonElement::JsonElement(const String& sValue) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(JsonValueKind::String)) {
                    m_pImpl->sStrValue = sValue;
                }

                JsonElement::JsonElement(std::nullptr_t) 
                    : m_pImpl(SmartPointer<JsonElementImpl>::New(JsonValueKind::Null)) {
                }

                JsonValueKind JsonElement::GetValueKind() const {
                    return m_pImpl ? m_pImpl->eKind : JsonValueKind::Undefined;
                }

                bool JsonElement::GetBoolean() const {
                    if (GetValueKind() == JsonValueKind::True) return true;
                    if (GetValueKind() == JsonValueKind::False) return false;
                    throw InvalidOperationException("JsonElement is not a boolean.");
                }

                double JsonElement::GetDouble() const {
                    if (GetValueKind() != JsonValueKind::Number) throw InvalidOperationException("JsonElement is not a number.");
                    return m_pImpl->dNumValue;
                }

                int JsonElement::GetInt32() const {
                    return static_cast<int>(GetDouble());
                }

                long long JsonElement::GetInt64() const {
                    return static_cast<long long>(GetDouble());
                }

                String JsonElement::GetString() const {
                    if (GetValueKind() != JsonValueKind::String) throw InvalidOperationException("JsonElement is not a string.");
                    return m_pImpl->sStrValue;
                }

                int JsonElement::GetArrayLength() const {
                    if (GetValueKind() != JsonValueKind::Array) throw InvalidOperationException("JsonElement is not an array.");
                    return m_pImpl->lstArray.GetCount();
                }

                JsonElement JsonElement::GetArrayElement(int iIndex) const {
                    if (GetValueKind() != JsonValueKind::Array) throw InvalidOperationException("JsonElement is not an array.");
                    return m_pImpl->lstArray[iIndex];
                }

                void JsonElement::AddArrayElement(const JsonElement& objElement) {
                    if (GetValueKind() != JsonValueKind::Array) throw InvalidOperationException("JsonElement is not an array.");
                    m_pImpl->lstArray.Add(objElement);
                }

                bool JsonElement::TryGetProperty(const String& sPropertyName, JsonElement& objValue) const {
                    if (GetValueKind() != JsonValueKind::Object) return false;
                    return m_pImpl->dictObject.TryGetValue(sPropertyName, objValue);
                }

                void JsonElement::SetProperty(const String& sPropertyName, const JsonElement& objValue) {
                    if (GetValueKind() != JsonValueKind::Object) throw InvalidOperationException("JsonElement is not an object.");
                    if (m_pImpl->dictObject.ContainsKey(sPropertyName)) {
                        m_pImpl->dictObject[sPropertyName] = objValue;
                    } else {
                        m_pImpl->dictObject.Add(sPropertyName, objValue);
                    }
                }

                Array<String> JsonElement::GetPropertyNames() const {
                    if (GetValueKind() != JsonValueKind::Object) throw InvalidOperationException("JsonElement is not an object.");
                    return m_pImpl->dictObject.GetKeys();
                }

                String JsonElement::ToString() const {
                    if (!m_pImpl) return "null";
                    switch (m_pImpl->eKind) {
                        case JsonValueKind::Null:
                            return "null";
                        case JsonValueKind::True:
                            return "true";
                        case JsonValueKind::False:
                            return "false";
                        case JsonValueKind::Number: {
                            double dVal = m_pImpl->dNumValue;
                            if (dVal == static_cast<long long>(dVal)) {
                                return String(std::to_string(static_cast<long long>(dVal)).c_str());
                            } else {
                                return String(std::to_string(dVal).c_str());
                            }
                        }
                        case JsonValueKind::String:
                            return EscapeString(m_pImpl->sStrValue);
                        case JsonValueKind::Array: {
                            std::string sRes = "[";
                            for (int i = 0; i < m_pImpl->lstArray.GetCount(); ++i) {
                                if (i > 0) sRes += ",";
                                sRes += m_pImpl->lstArray[i].ToString().GetRawString();
                            }
                            sRes += "]";
                            return String(sRes.c_str());
                        }
                        case JsonValueKind::Object: {
                            std::string sRes = "{";
                            auto keys = m_pImpl->dictObject.GetKeys();
                            for (int i = 0; i < keys.GetLength(); ++i) {
                                if (i > 0) sRes += ",";
                                sRes += EscapeString(keys[i]).GetRawString();
                                sRes += ":";
                                sRes += m_pImpl->dictObject[keys[i]].ToString().GetRawString();
                            }
                            sRes += "}";
                            return String(sRes.c_str());
                        }
                        default:
                            return "null";
                    }
                }

                JsonElement JsonElement::Parse(const String& sJson) {
                    return JsonParser::Parse(sJson);
                }

            }
        }
    }
}

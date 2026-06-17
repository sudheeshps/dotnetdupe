#include "pch.h"
#include "gtest/gtest.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/Text/Json/JsonException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text::Json;
using namespace DotNetDupe::System::Collections::Generic;

namespace DotNetDupeTests {

    struct TestPerson {
        String Name;
        int Age;
        bool IsActive;

        bool operator==(const TestPerson& other) const {
            return Name == other.Name && Age == other.Age && IsActive == other.IsActive;
        }
    };

}

// Specialize JsonConverter for TestPerson in DotNetDupe::System::Text::Json namespace
namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<DotNetDupeTests::TestPerson> {
                    static JsonElement Write(const DotNetDupeTests::TestPerson& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("Name", JsonConverter<String>::Write(value.Name));
                        obj.SetProperty("Age", JsonConverter<int>::Write(value.Age));
                        obj.SetProperty("IsActive", JsonConverter<bool>::Write(value.IsActive));
                        return obj;
                    }

                    static DotNetDupeTests::TestPerson Read(const JsonElement& element) {
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw InvalidOperationException("Expected a JSON object");
                        }
                        DotNetDupeTests::TestPerson p;
                        JsonElement prop;
                        if (element.TryGetProperty("Name", prop)) {
                            p.Name = JsonConverter<String>::Read(prop);
                        }
                        if (element.TryGetProperty("Age", prop)) {
                            p.Age = JsonConverter<int>::Read(prop);
                        }
                        if (element.TryGetProperty("IsActive", prop)) {
                            p.IsActive = JsonConverter<bool>::Read(prop);
                        }
                        return p;
                    }
                };
            }
        }
    }
}

namespace DotNetDupeTests {

    // --- Primitive Types Tests ---

    TEST(JsonSerializerTests, GivenInt_WhenSerialized_ThenProducesCorrectJson) {
        // Given
        int input = 42;

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "42");
    }

    TEST(JsonSerializerTests, GivenJsonInt_WhenDeserialized_ThenProducesCorrectValue) {
        // Given
        String json = "42";

        // When
        int result = JsonSerializer::Deserialize<int>(json);

        // Then
        EXPECT_EQ(result, 42);
    }

    TEST(JsonSerializerTests, GivenDouble_WhenSerialized_ThenProducesCorrectJson) {
        // Given
        double input = 3.14159;

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        // Allow formatting variations (like trailing zeroes depending on compiler std::to_string)
        double result = JsonSerializer::Deserialize<double>(json);
        EXPECT_NEAR(result, input, 0.0001);
    }

    TEST(JsonSerializerTests, GivenBoolTrue_WhenSerialized_ThenProducesCorrectJson) {
        // Given
        bool input = true;

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "true");
    }

    TEST(JsonSerializerTests, GivenBoolFalse_WhenSerialized_ThenProducesCorrectJson) {
        // Given
        bool input = false;

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "false");
    }

    TEST(JsonSerializerTests, GivenJsonBool_WhenDeserialized_ThenProducesCorrectValue) {
        // Given
        String jsonTrue = "true";
        String jsonFalse = "false";

        // When
        bool resTrue = JsonSerializer::Deserialize<bool>(jsonTrue);
        bool resFalse = JsonSerializer::Deserialize<bool>(jsonFalse);

        // Then
        EXPECT_TRUE(resTrue);
        EXPECT_FALSE(resFalse);
    }

    // --- String and Escapes Tests ---

    TEST(JsonSerializerTests, GivenString_WhenSerialized_ThenProducesQuotedJson) {
        // Given
        String input = "Hello, World!";

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "\"Hello, World!\"");
    }

    TEST(JsonSerializerTests, GivenEscapedString_WhenSerialized_ThenProducesCorrectEscapes) {
        // Given
        String input = "Line1\nLine2\t\"Quotes\"\\Backslash";

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "\"Line1\\nLine2\\t\\\"Quotes\\\"\\\\Backslash\"");
    }

    TEST(JsonSerializerTests, GivenJsonWithEscapes_WhenDeserialized_ThenProducesUnescapedString) {
        // Given
        String json = "\"Line1\\nLine2\\t\\\"Quotes\\\"\\\\Backslash\"";

        // When
        String result = JsonSerializer::Deserialize<String>(json);

        // Then
        EXPECT_EQ(result, "Line1\nLine2\t\"Quotes\"\\Backslash");
    }

    TEST(JsonSerializerTests, GivenUnicodeEscape_WhenDeserialized_ThenProducesCorrectUtf8) {
        // Given
        // \u0041 is 'A', \u00a9 is '©' (C2 A9 in UTF-8)
        String json = "\"\\u0041 and \\u00a9\"";

        // When
        String result = JsonSerializer::Deserialize<String>(json);

        // Then
        EXPECT_EQ(result, "A and \xc2\xa9");
    }

    // --- Collections Tests ---

    TEST(JsonSerializerTests, GivenListInt_WhenSerialized_ThenProducesJsonArray) {
        // Given
        List<int> input = { 1, 2, 3, 4 };

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "[1,2,3,4]");
    }

    TEST(JsonSerializerTests, GivenJsonArray_WhenDeserialized_ThenProducesListInt) {
        // Given
        String json = " [ 1 , 2 , 3 , 4 ] ";

        // When
        List<int> result = JsonSerializer::Deserialize<List<int>>(json);

        // Then
        EXPECT_EQ(result.GetCount(), 4);
        EXPECT_EQ(result[0], 1);
        EXPECT_EQ(result[3], 4);
    }

    TEST(JsonSerializerTests, GivenDictionary_WhenSerialized_ThenProducesJsonObject) {
        // Given
        Dictionary<String, String> input;
        input.Add("key1", "val1");
        input.Add("key2", "val2");

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        // Dictionary order depends on hash map. We verify by deserializing it back or checking property count.
        Dictionary<String, String> result = JsonSerializer::Deserialize<Dictionary<String, String>>(json);
        EXPECT_EQ(result.GetCount(), 2);
        EXPECT_EQ(result["key1"], "val1");
        EXPECT_EQ(result["key2"], "val2");
    }

    // --- Nested Structures Tests ---

    TEST(JsonSerializerTests, GivenNestedList_WhenSerialized_ThenProducesCorrectJson) {
        // Given
        List<List<int>> input;
        input.Add({ 1, 2 });
        input.Add({ 3, 4 });

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        EXPECT_EQ(json, "[[1,2],[3,4]]");
    }

    TEST(JsonSerializerTests, GivenNestedJson_WhenDeserialized_ThenProducesNestedList) {
        // Given
        String json = "[[1,2],[3,4]]";

        // When
        List<List<int>> result = JsonSerializer::Deserialize<List<List<int>>>(json);

        // Then
        EXPECT_EQ(result.GetCount(), 2);
        EXPECT_EQ(result[0].GetCount(), 2);
        EXPECT_EQ(result[0][0], 1);
        EXPECT_EQ(result[1][1], 4);
    }

    // --- Custom Object Tests ---

    TEST(JsonSerializerTests, GivenCustomStruct_WhenSerialized_ThenProducesObjectJson) {
        // Given
        TestPerson input = { "Alice", 30, true };

        // When
        String json = JsonSerializer::Serialize(input);

        // Then
        TestPerson result = JsonSerializer::Deserialize<TestPerson>(json);
        EXPECT_EQ(result, input);
    }

    // --- Negative / Error Cases Tests ---

    TEST(JsonSerializerTests, GivenInvalidJson_WhenDeserialized_ThenThrowsException) {
        // Given
        String invalidJson = "{ \"Name\": \"Alice\", "; // Unclosed object

        // When & Then
        EXPECT_THROW(JsonSerializer::Deserialize<TestPerson>(invalidJson), JsonException);
    }

    TEST(JsonSerializerTests, GivenMismatchJsonType_WhenDeserialized_ThenThrowsException) {
        // Given
        String mismatchedJson = "[1, 2, 3]"; // Array instead of object

        // When & Then
        EXPECT_THROW(JsonSerializer::Deserialize<TestPerson>(mismatchedJson), InvalidOperationException);
    }

}

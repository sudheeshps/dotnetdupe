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

    // --- Strongly-Typed Generic DTO & Nested Mapping Tests ---

    struct TestStockQuote {
        String Symbol;
        double Price;
        int Volume;
        List<double> History;

        JsonElement ToJson() const {
            JsonElement obj(JsonValueKind::Object);
            obj.SetProperty("Symbol", JsonElement(Symbol));
            obj.SetProperty("Price", JsonElement(Price));
            obj.SetProperty("Volume", JsonElement(static_cast<double>(Volume)));
            obj.SetProperty("History", JsonSerializer::SerializeToElement(History));
            return obj;
        }

        static TestStockQuote FromJson(const JsonElement& elem) {
            TestStockQuote q;
            q.Symbol = elem.GetPropertyString("Symbol");
            q.Price = elem.GetPropertyDouble("Price");
            q.Volume = elem.GetPropertyInt32("Volume");
            q.History = elem.GetPropertyAs<List<double>>("History");
            return q;
        }

        bool operator==(const TestStockQuote& other) const {
            if (Symbol != other.Symbol || Price != other.Price || Volume != other.Volume) return false;
            if (History.GetCount() != other.History.GetCount()) return false;
            for (int i = 0; i < History.GetCount(); ++i) {
                if (History[i] != other.History[i]) return false;
            }
            return true;
        }
    };

    struct TestOrderDto {
        String OrderId;
        double Amount;

        JsonElement ToJson() const {
            JsonElement obj(JsonValueKind::Object);
            obj.SetProperty("OrderId", JsonElement(OrderId));
            obj.SetProperty("Amount", JsonElement(Amount));
            return obj;
        }

        void FromJson(const JsonElement& elem) {
            OrderId = elem.GetPropertyString("OrderId");
            Amount = elem.GetPropertyDouble("Amount");
        }

        bool operator==(const TestOrderDto& other) const {
            return OrderId == other.OrderId && Amount == other.Amount;
        }
    };

    TEST(JsonSerializerTests, GivenDtoWithStaticFromJson_WhenSerializedAndDeserialized_ThenMapsAutomatically) {
        // Given
        TestStockQuote quote;
        quote.Symbol = "AAPL";
        quote.Price = 175.50;
        quote.Volume = 50000;
        quote.History = { 174.0, 174.5, 175.5 };

        // When
        String json = JsonSerializer::Serialize(quote);
        TestStockQuote restored = JsonSerializer::Deserialize<TestStockQuote>(json);

        // Then
        EXPECT_EQ(restored, quote);
    }

    TEST(JsonSerializerTests, GivenNestedListOfDtos_WhenSerializedAndDeserialized_ThenMapsNestedCollections) {
        // Given
        List<TestStockQuote> portfolio;
        TestStockQuote q1 = { "MSFT", 400.0, 1000, { 395.0, 400.0 } };
        TestStockQuote q2 = { "NVDA", 120.0, 2000, { 115.0, 120.0 } };
        portfolio.Add(q1);
        portfolio.Add(q2);

        // When
        String json = JsonSerializer::Serialize(portfolio);
        List<TestStockQuote> restored = JsonSerializer::Deserialize<List<TestStockQuote>>(json);

        // Then
        EXPECT_EQ(restored.GetCount(), 2);
        EXPECT_EQ(restored[0], q1);
        EXPECT_EQ(restored[1], q2);
    }

    TEST(JsonSerializerTests, GivenDtoWithMemberFromJson_WhenSerializedAndDeserialized_ThenMapsAutomatically) {
        // Given
        TestOrderDto order = { "ORD-12345", 99.95 };

        // When
        String json = JsonSerializer::Serialize(order);
        TestOrderDto restored = JsonSerializer::Deserialize<TestOrderDto>(json);

        // Then
        EXPECT_EQ(restored, order);
    }

    // --- Array & Additional Primitives Tests ---

    TEST(JsonSerializerTests, GivenArrayInt_WhenSerializedAndDeserialized_ThenPreservesElements) {
        // Given
        Array<int> arr(3);
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;

        // When
        String json = JsonSerializer::Serialize(arr);
        Array<int> restored = JsonSerializer::Deserialize<Array<int>>(json);

        // Then
        EXPECT_EQ(restored.GetLength(), 3);
        EXPECT_EQ(restored[0], 10);
        EXPECT_EQ(restored[1], 20);
        EXPECT_EQ(restored[2], 30);
    }

    TEST(JsonSerializerTests, GivenNumericPrimitives_WhenSerializedAndDeserialized_ThenPreservesValues) {
        // Given & When & Then
        short sVal = 1234;
        EXPECT_EQ(JsonSerializer::Deserialize<short>(JsonSerializer::Serialize(sVal)), sVal);

        unsigned short usVal = 5678;
        EXPECT_EQ(JsonSerializer::Deserialize<unsigned short>(JsonSerializer::Serialize(usVal)), usVal);

        unsigned int uiVal = 999999U;
        EXPECT_EQ(JsonSerializer::Deserialize<unsigned int>(JsonSerializer::Serialize(uiVal)), uiVal);

        unsigned long long ullVal = 12345678901234ULL;
        EXPECT_EQ(JsonSerializer::Deserialize<unsigned long long>(JsonSerializer::Serialize(ullVal)), ullVal);

        char cVal = 'Z';
        EXPECT_EQ(JsonSerializer::Deserialize<char>(JsonSerializer::Serialize(cVal)), cVal);

        unsigned char ucVal = 200;
        EXPECT_EQ(JsonSerializer::Deserialize<unsigned char>(JsonSerializer::Serialize(ucVal)), ucVal);
    }

    TEST(JsonSerializerTests, GivenDateTimeAndGuid_WhenSerializedAndDeserialized_ThenRoundTrips) {
        // Given
        DateTime dt(2026, 10, 1, 12, 30, 45);
        Guid guid = Guid::NewGuid();

        // When
        String jsonDt = JsonSerializer::Serialize(dt);
        String jsonGuid = JsonSerializer::Serialize(guid);

        DateTime restoredDt = JsonSerializer::Deserialize<DateTime>(jsonDt);
        Guid restoredGuid = JsonSerializer::Deserialize<Guid>(jsonGuid);

        // Then
        EXPECT_EQ(restoredDt.GetYear(), 2026);
        EXPECT_EQ(restoredDt.GetMonth(), 10);
        EXPECT_EQ(restoredDt.GetDay(), 1);
        EXPECT_EQ(restoredGuid, guid);
    }

    // --- JsonElement Helper Methods Tests ---

    TEST(JsonSerializerTests, GivenJsonObject_WhenHelperGettersCalled_ThenReturnsExpectedValues) {
        // Given
        String json = "{\"name\":\"Alpha\",\"age\":25,\"score\":98.5,\"active\":true}";
        JsonElement elem = JsonElement::Parse(json);

        // When & Then
        EXPECT_TRUE(elem.HasProperty("name"));
        EXPECT_FALSE(elem.HasProperty("missing"));

        EXPECT_EQ(elem.GetPropertyString("name"), "Alpha");
        EXPECT_EQ(elem.GetPropertyString("missing", "default"), "default");

        EXPECT_EQ(elem.GetPropertyInt32("age"), 25);
        EXPECT_EQ(elem.GetPropertyInt32("missing", 99), 99);

        EXPECT_DOUBLE_EQ(elem.GetPropertyDouble("score"), 98.5);
        EXPECT_DOUBLE_EQ(elem.GetPropertyDouble("missing", 0.0), 0.0);

        EXPECT_TRUE(elem.GetPropertyBoolean("active"));
        EXPECT_FALSE(elem.GetPropertyBoolean("missing", false));

        // GetProperty and template accessors
        JsonElement prop = elem.GetProperty("name");
        EXPECT_EQ(prop.GetString(), "Alpha");
        EXPECT_THROW(elem.GetProperty("missing"), JsonException);

        EXPECT_EQ(elem.GetPropertyAs<String>("name"), "Alpha");
        EXPECT_EQ(elem.GetPropertyAs<int>("age"), 25);
        EXPECT_EQ(elem.GetPropertyOrDefault<String>("missing", "fallback"), "fallback");

        int outAge = 0;
        EXPECT_TRUE(elem.TryGetPropertyAs("age", outAge));
        EXPECT_EQ(outAge, 25);
        int outMissing = 0;
        EXPECT_FALSE(elem.TryGetPropertyAs("missing", outMissing));
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

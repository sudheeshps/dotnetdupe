#include "System/String.h"
#include "System/Convert.h"
#include "System/Text/Json/JsonSerializer.h"
#include "System/InvalidOperationException.h"

#include "System/Collections/Generic/List.h"
#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

#include "Demos.h"

struct DemoPerson {
    String Name;
    int Age;
};

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {
                template <>
                struct JsonConverter<::DemoPerson> {
                    static JsonElement Write(const ::DemoPerson& value) {
                        JsonElement obj(JsonValueKind::Object);
                        obj.SetProperty("Name", JsonConverter<String>::Write(value.Name));
                        obj.SetProperty("Age", JsonConverter<int>::Write(value.Age));
                        return obj;
                    }

                    static ::DemoPerson Read(const JsonElement& element) {
                        if (element.GetValueKind() != JsonValueKind::Object) {
                            throw InvalidOperationException("Expected a JSON object");
                        }
                        ::DemoPerson p;
                        JsonElement prop;
                        if (element.TryGetProperty("Name", prop)) {
                            p.Name = JsonConverter<String>::Read(prop);
                        }
                        if (element.TryGetProperty("Age", prop)) {
                            p.Age = JsonConverter<int>::Read(prop);
                        }
                        return p;
                    }
                };
            }
        }
    }
}

void DemonstrateJson() {
    using namespace DotNetDupe::System::Text::Json;
    Console::WriteLine("\n--- JSON Serialization/Deserialization Demonstration ---");

    // 1. Primitives
    int iOriginal = 123;
    String sJsonInt = JsonSerializer::Serialize(iOriginal);
    Console::Write("Serialized int: ");
    Console::WriteLine(sJsonInt);

    int iDeserialized = JsonSerializer::Deserialize<int>(sJsonInt);
    Console::Write("Deserialized int: ");
    Console::WriteLine(Convert::ToString(iDeserialized));

    // 2. List
    List<int> lstNumbers = { 10, 20, 30, 40 };
    String sJsonList = JsonSerializer::Serialize(lstNumbers);
    Console::Write("Serialized List<int>: ");
    Console::WriteLine(sJsonList);

    List<int> lstDeserialized = JsonSerializer::Deserialize<List<int>>(sJsonList);
    Console::Write("Deserialized List<int> (count): ");
    Console::WriteLine(Convert::ToString(lstDeserialized.GetCount()));

    // 3. Custom Object
    DemoPerson objPerson = { "Bob", 25 };
    String sJsonObj = JsonSerializer::Serialize(objPerson);
    Console::Write("Serialized DemoPerson: ");
    Console::WriteLine(sJsonObj);

    DemoPerson objDeserialized = JsonSerializer::Deserialize<DemoPerson>(sJsonObj);
    Console::Write("Deserialized DemoPerson - Name: '");
    Console::Write(objDeserialized.Name);
    Console::Write("', Age: ");
    Console::WriteLine(Convert::ToString(objDeserialized.Age));
}

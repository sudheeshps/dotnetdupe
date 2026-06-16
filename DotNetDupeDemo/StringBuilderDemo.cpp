#include "System/Text/StringBuilder.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

#include "Demos.h"

void DemonstrateStringBuilder() {
    Console::WriteLine("\n--- StringBuilder Demonstration ---");

    StringBuilder sbBuilder;
    sbBuilder.Append("User: ")
      .Append("Gemini")
      .AppendLine()
      .Append("Status: ")
      .Append(true)
      .AppendLine()
      .Append("Score: ")
      .Append(98.5);

    Console::Write(sbBuilder.ToString());
    Console::WriteLine();
}

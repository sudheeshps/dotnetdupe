#include "System/Console.h"
#include "System/Convert.h"
#include "System/IO/Directory.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "Demos.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

void DemonstrateDirectory() {
    Console::WriteLine("\n--- Directory Class Demonstration ---");

    String sCurrentDir = Directory::GetCurrentDirectory();
    Console::WriteLine(String("Current Directory: ") + sCurrentDir);

    String sDemoDir = Path::Combine({ sCurrentDir, "DemoDirectorySample" });
    if (!Directory::Exists(sDemoDir)) {
        Directory::CreateDirectory(sDemoDir);
        Console::WriteLine(String("Created Directory: ") + sDemoDir);
    }

    String sSampleFile = Path::Combine({ sDemoDir, "sample.txt" });
    File::WriteAllText(sSampleFile, "Hello DotNetDupe Directory Demo");

    Array<String> files = Directory::GetFiles(sDemoDir);
    Console::WriteLine(String("Files in demo dir count: ") + Convert::ToString(files.GetLength()));

    Directory::Delete(sDemoDir, true);
    Console::WriteLine(String("Deleted Directory: ") + sDemoDir);
}

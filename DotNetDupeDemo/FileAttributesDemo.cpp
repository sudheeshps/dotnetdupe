#include "System/String.h"
#include "System/IO/File.h"
#include "System/Convert.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

#include "Demos.h"

void DemonstrateFileAttributes() {
    Console::WriteLine("\n--- File Attributes Demonstration ---");

    String sPath = "attr_demo.txt";
    File::WriteAllText(sPath, "Demo content for attributes");

    FileAttributes objAttrs;
    if (File::GetAttributes(sPath, objAttrs)) {
        Console::Write("Initial Attributes (int): ");
        Console::WriteLine((int)objAttrs);

        // Set to ReadOnly
        if (File::SetAttributes(sPath, FileAttributes::ReadOnly)) {
            Console::WriteLine("Set attribute to ReadOnly: Success");
            
            FileAttributes objNewAttrs;
            File::GetAttributes(sPath, objNewAttrs);
            bool bIsReadOnly = ((int)objNewAttrs & (int)FileAttributes::ReadOnly) != 0;
            Console::Write("Is ReadOnly now? ");
            Console::WriteLine(Convert::ToString(bIsReadOnly));
        }

        // Restore to Normal
        File::SetAttributes(sPath, FileAttributes::Normal);
        Console::WriteLine("Restored attributes to Normal.");
    }

    File::Delete(sPath);
}

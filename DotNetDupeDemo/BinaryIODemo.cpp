#include "System/Console.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/Convert.h"
#include "System/IO/MemoryStream.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/BinaryWriter.h"
#include <iostream>
#include <iomanip>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

#include "Demos.h"

void DemonstrateBinaryIO() {
    Console::WriteLine("\n--- Binary I/O Demonstration ---");

    // 1. Basic Binary Writing & Reading using MemoryStream
    Console::WriteLine("\n1. Writing and Reading Primitive Data Types:");
    MemoryStream stream;
    {
        BinaryWriter writer(&stream, true, true);
        writer.Write(true);
        writer.Write(static_cast<byte>(0x42));
        writer.Write(static_cast<short>(1024));
        writer.Write(123456789);
        writer.Write(3.1415926535);
        writer.Write(String("DotNetDupe Binary Stream"));
        writer.Flush();
    }

    stream.SetPosition(0);
    {
        BinaryReader reader(&stream, true, true);
        bool bVal = reader.ReadBoolean();
        byte byteVal = reader.ReadByte();
        short sVal = reader.ReadInt16();
        int iVal = reader.ReadInt32();
        double dVal = reader.ReadDouble();
        String strVal = reader.ReadString(24);

        Console::WriteLine("  Read Boolean: {0}", bVal ? "True" : "False");
        Console::WriteLine("  Read Byte:    0x{0}", Convert::ToString(static_cast<int>(byteVal), 16));
        Console::WriteLine("  Read Int16:   {0}", sVal);
        Console::WriteLine("  Read Int32:   {0}", iVal);
        Console::WriteLine("  Read Double:  {0}", dVal);
        Console::WriteLine("  Read String:  \"{0}\"", strVal);
    }

    // 2. Endianness Demonstration (Little-Endian vs Big-Endian)
    Console::WriteLine("\n2. Endianness Control (Little-Endian vs Big-Endian):");
    MemoryStream endianStream;
    int testNumber = 0x12345678;

    {
        BinaryWriter leWriter(&endianStream, true, true);
        leWriter.Write(testNumber);
        leWriter.Flush();
    }
    {
        BinaryWriter beWriter(&endianStream, true, false);
        beWriter.Write(testNumber);
        beWriter.Flush();
    }

    endianStream.SetPosition(0);
    Array<char> rawBytes = endianStream.ToArray();
    Console::Write("  Raw Bytes (Little-Endian then Big-Endian): ");
    for (int i = 0; i < rawBytes.GetLength(); ++i) {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << (static_cast<int>(rawBytes[i]) & 0xFF) << " ";
    }
    std::cout << std::dec << std::endl;

    endianStream.SetPosition(0);
    {
        BinaryReader leReader(&endianStream, true, true);
        int leResult = leReader.ReadInt32();
        Console::WriteLine("  Decoded as Little-Endian: 0x{0}", Convert::ToString(leResult, 16));

        BinaryReader beReader(&endianStream, true, false);
        int beResult = beReader.ReadInt32();
        Console::WriteLine("  Decoded as Big-Endian:    0x{0}", Convert::ToString(beResult, 16));
    }

    // 3. Buffer Operations & Seeking
    Console::WriteLine("\n3. Buffer Operations & Seeking:");
    MemoryStream seekStream;
    {
        BinaryWriter writer(&seekStream, true, true);
        writer.Write(String("DICM"));
        writer.Write(100); // Placeholder offset
        writer.Write(String("Payload Data"));

        // Seek back to overwrite placeholder offset
        writer.Seek(4, 0 /* SEEK_SET */);
        writer.Write(999); // Updated offset
        writer.Flush();
    }

    seekStream.SetPosition(0);
    {
        BinaryReader reader(&seekStream, true, true);
        String magicStr = reader.ReadString(4);
        int offset = reader.ReadInt32();
        String payload = reader.ReadString(12);

        Console::WriteLine("  Magic Header:   {0}", magicStr);
        Console::WriteLine("  Updated Offset: {0}", offset);
        Console::WriteLine("  Payload:        {0}", payload);
    }
}

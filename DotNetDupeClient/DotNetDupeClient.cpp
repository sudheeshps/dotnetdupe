#include "../DotNetDupe/Array.h"
#include "../DotNetDupe/String.h"
#include "../DotNetDupe/IO/File.h"
#include "../DotNetDupe/IO/FileStream.h"
#include "../DotNetDupe/Environment.h"
#include "../DotNetDupe/Buffer.h"
#include "../DotNetDupe/Char.h"
#include "../DotNetDupe/Path.h"
#include "../DotNetDupe/Random.h"
#include "../DotNetDupe/Uri.h"
#include "../DotNetDupe/Version.h"
#include "../DotNetDupe/OperatingSystem.h"
#include <iostream>
//
//
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

void DemonstrateArray() {
    std::cout << "--- Array Demonstration ---" << std::endl;
    Array<int> arr(5);
    arr [0] = 1;
    arr [1] = 2;
    arr [2] = 3;
    arr [3] = 4;
    arr [4] = 5;

    for (int i = 0; i < arr.GetLength(); i++) {
        std::cout << arr [i] << " ";
    }
    std::cout << std::endl;
}

void DemonstrateFile() {
    std::cout << "\n--- File Demonstration ---" << std::endl;
    const String testFile(_T("test.txt"));
    const String content(_T("Hello, World!"));
    File::WriteAllText(testFile, content);
    String readContent = File::ReadAllText(testFile);
    std::wcout << L"Read from file: " << readContent.GetRawString() << std::endl;
    File::Delete(testFile);
}

void DemonstrateFileStream() {
    std::cout << "\n--- FileStream Demonstration ---" << std::endl;
    const String testFile(_T("test_filestream.txt"));
    const String content(_T("Hello FileStream!"));

    FileStream fs(testFile, 0); // FileMode::CreateNew
    std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(content);
    fs.Write(contentBytes.data(), 0, static_cast<int>(contentBytes.size()));
    fs.SetPosition(0);
    std::vector<char> readBuffer(contentBytes.size());
    int bytesRead = fs.Read(readBuffer.data(), 0, static_cast<int>(readBuffer.size()));
    String readContent = Text::TextEncoding::UTF8()->GetString(readBuffer);
    std::wcout << L"Read from FileStream: " << readContent.GetRawString() << std::endl;
    fs.Dispose();
    File::Delete(testFile);
}

void DemonstrateEnvironment() {
    std::cout << "\n--- Environment Demonstration ---" << std::endl;
    std::wcout << L"Current Directory: " << Environment::GetCurrentDirectory().GetRawString() << std::endl;
    std::wcout << L"OS Version: " << Environment::GetOSVersion().GetRawString() << std::endl;
}

void DemonstrateBuffer() {
    std::cout << "\n--- Buffer Demonstration ---" << std::endl;
    Array<byte> src(5);
    src [0] = 1;
    src [1] = 2;
    src [2] = 3;
    src [3] = 4;
    src [4] = 5;
    Array<byte> dst(5);
    Buffer::BlockCopy(src, 0, dst, 0, 5);
    for (int i = 0; i < dst.GetLength(); i++) {
        std::cout << (int)dst [i] << " ";
    }
    std::cout << std::endl;
}

void DemonstrateChar() {
    std::cout << "\n--- Char Demonstration ---" << std::endl;
    Char c = 'a';
    std::wcout << L"Is 'a' a letter? " << (Char::IsLetter(c.GetChar()) ? "Yes" : "No") << std::endl;
    std::wcout << L"Is 'a' a digit? " << (Char::IsDigit(c.GetChar()) ? "Yes" : "No") << std::endl;
}

void DemonstratePath() {
    std::cout << "\n--- Path Demonstration ---" << std::endl;
    String path1 = _T("C:\\Users\\Test");
    String path2 = _T("file.txt");
    String combinedPath = Path::Combine({path1, path2});
    std::wcout << L"Combined Path: " << combinedPath.GetRawString() << std::endl;
    std::wcout << L"File Extension: " << Path::GetExtension(combinedPath).GetRawString() << std::endl;
}

void DemonstrateRandom() {
    std::cout << "\n--- Random Demonstration ---" << std::endl;
    Random rand;
    std::cout << "Random number between 0 and 100: " << rand.Next(101) << std::endl;
}

void DemonstrateUri() {
    std::cout << "\n--- Uri Demonstration ---" << std::endl;
    Uri uri(_T("http://www.google.com:80/search?q=dotnet#fragment"));
    std::wcout << L"Scheme: " << uri.GetScheme().GetRawString() << std::endl;
    std::wcout << L"Host: " << uri.GetHost().GetRawString() << std::endl;
    std::wcout << L"Port: " << uri.GetPort() << std::endl;
    std::wcout << L"AbsolutePath: " << uri.GetAbsolutePath().GetRawString() << std::endl;
    std::wcout << L"Query: " << uri.GetQuery().GetRawString() << std::endl;
    std::wcout << L"Fragment: " << uri.GetFragment().GetRawString() << std::endl;
}

void DemonstrateVersion() {
    std::cout << "\n--- Version Demonstration ---" << std::endl;
    Version version(1, 2, 3, 4);
    std::wcout << L"Version: " << version.ToString().GetRawString() << std::endl;
}

void DemonstrateOperatingSystem() {
    std::cout << "\n--- OperatingSystem Demonstration ---" << std::endl;
    OperatingSystem os = Environment::GetOperatingSystem();
    std::wcout << L"OS Platform: " << (int)os.GetPlatform() << std::endl;
    std::wcout << L"OS Version: " << os.GetVersionString().GetRawString() << std::endl;
}

void DemonstrateString() {
    std::cout << "\n--- String Demonstration ---" << std::endl;
    String str1(_T("Hello"));
    String str2(_T("World"));

    // Concatenation
    String str3 = str1 + _T(" ") + str2;
    std::wcout << L"Concatenated string: " << str3.GetRawString() << std::endl;

    // Comparison
    if (str1.Equals(_T("Hello"))) {
        std::cout << "str1 is equal to \"Hello\"" << std::endl;
    }

    // Substring
    String sub = str3.Substring(6, 5);
    std::wcout << L"Substring: " << sub.GetRawString() << std::endl;

    // Splitting
    String toSplit(_T("one,two,three"));
    auto parts = toSplit.Split(',');
    std::cout << "Splitting \"one,two,three\" by ',':" << std::endl;
    for (const auto& part : parts) {
        std::wcout << L"  " << part.GetRawString() << std::endl;
    }
}

int main() {
    DemonstrateArray();
    DemonstrateFile();
    DemonstrateFileStream();
    DemonstrateEnvironment();
    DemonstrateBuffer();
    DemonstrateChar();
    DemonstratePath();
    DemonstrateRandom();
    DemonstrateUri();
    DemonstrateVersion();
    DemonstrateOperatingSystem();
    DemonstrateString();
    return 0;
}

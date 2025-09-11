#pragma once
#include "Common.h"
#include "String.h"
#include <vector>

using namespace DotNetDupe::System;
namespace DotNetDupe
{
    namespace System
    {
        namespace IO
        {
            class Path
            {
            public:
                DOTNETDUPE_API static String ChangeExtension(const String &filePath, const String &extension);
                DOTNETDUPE_API static String Combine(const std::initializer_list<String> paths);
                DOTNETDUPE_API static bool EndsInDirectorySeparator(const String &filePath);
                DOTNETDUPE_API static bool Exists(const String &filePath);
                DOTNETDUPE_API static String GetDirectoryName(const String &filePath);
                DOTNETDUPE_API static String GetFileName(const String &filePath);
                DOTNETDUPE_API static String GetExtension(const String &filePath);
                DOTNETDUPE_API static String GetFileNameWithoutExtension(const String &filePath);
                DOTNETDUPE_API static String GetFullPath(const String &path);
                DOTNETDUPE_API static std::vector<TCHAR> GetInvalidFileNameChars();
                DOTNETDUPE_API static std::vector<TCHAR> GetInvalidPathChars();
            };
        }
    }
}
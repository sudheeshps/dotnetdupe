#pragma once
#include "Common.h"
#include "System/String.h"
#include <vector>

using namespace DotNetDupe::System;
namespace DotNetDupe {
    namespace System {
        namespace IO {
            class Path {
            public:
                DOTNETDUPE_API static String ChangeExtension(const String& filePath, const String& extension);
                DOTNETDUPE_API static String Combine(const std::initializer_list<String> paths);
                DOTNETDUPE_API static bool EndsInDirectorySeparator(const String& filePath);
                DOTNETDUPE_API static bool Exists(const String& filePath);
                DOTNETDUPE_API static String GetDirectoryName(const String& filePath);
                DOTNETDUPE_API static String GetFileName(const String& filePath);
                DOTNETDUPE_API static String GetExtension(const String& filePath);
                DOTNETDUPE_API static String GetFileNameWithoutExtension(const String& filePath);
                DOTNETDUPE_API static String GetFullPath(const String& path);
                DOTNETDUPE_API static std::vector<TCHAR> GetInvalidFileNameChars();
                DOTNETDUPE_API static std::vector<TCHAR> GetInvalidPathChars();
                DOTNETDUPE_API static String GetPathRoot(const String& path);
                DOTNETDUPE_API static String GetRandomFileName();
                DOTNETDUPE_API static String GetRelativePath(const String& relativeTo, const String& path);
                DOTNETDUPE_API static String GetTempFileName();
                DOTNETDUPE_API static String GetTempPath();
                DOTNETDUPE_API static bool HasExtension(const String& path);
                DOTNETDUPE_API static bool IsPathFullyQualified(const String& path);
                DOTNETDUPE_API static bool IsPathRooted(const String& path);
                DOTNETDUPE_API static String Join(const std::initializer_list<String> paths);
                DOTNETDUPE_API static bool TryJoin(const std::initializer_list<String> paths, String& result);
                DOTNETDUPE_API static String TrimEndingDirectorySeparator(const String& path);
            };
        }
    }
}
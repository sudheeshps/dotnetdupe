#pragma once

#include "../Common.h"
#include "../Object.h"
#include "../String.h"
#include "../Text/TextEncoding.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class File : public Object {
            public:
                DOTNETDUPE_API static bool Exists(const String& path);
                DOTNETDUPE_API static String ReadAllText(const String& path);
                DOTNETDUPE_API static void WriteAllText(const String& path, const String& contents);
                DOTNETDUPE_API static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite);
                DOTNETDUPE_API static void Move(const String& sourceFileName, const String& destFileName);
                DOTNETDUPE_API static void Delete(const String& path);
                DOTNETDUPE_API static void AppendAllText(const String& path, const String& contents);
                DOTNETDUPE_API static void AppendAllLines(const String& path, const std::vector<String>& contents);
                DOTNETDUPE_API static std::vector<String> ReadAllLines(const String& path);
                DOTNETDUPE_API static void WriteAllLines(const String& path, const std::vector<String>& contents);
                DOTNETDUPE_API static void Create(const String& path);
                DOTNETDUPE_API static int GetAttributes(const String& path);
                DOTNETDUPE_API static void SetAttributes(const String& path, int fileAttributes);
            };
        }
    }
}

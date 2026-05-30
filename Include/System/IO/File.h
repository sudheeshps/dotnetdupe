#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Text/TextEncoding.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            enum class FileAttributes {
                Normal = 0,
                ReadOnly = 1,
                Hidden = 2,
                System = 4,
                Directory = 16,
                Archive = 32,
                Device = 64,
                Temporary = 128,
                SparseFile = 256,
                ReparsePoint = 512,
                Compressed = 1024,
                Offline = 2048,
                NotContentIndexed = 4096,
                Encrypted = 8192,
                IntegrityStream = 16384,
                NoScrubData = 32768
            };

            class File : public Object {
            public:
                DOTNETDUPE_API static bool Exists(const String& path);
                DOTNETDUPE_API static String ReadAllText(const String& path);
                DOTNETDUPE_API static void WriteAllText(const String& path, const String& contents);
                DOTNETDUPE_API static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite);
                DOTNETDUPE_API static void Move(const String& sourceFileName, const String& destFileName);
                DOTNETDUPE_API static void Delete(const String& path);
                DOTNETDUPE_API static void AppendAllText(const String& path, const String& contents);
                DOTNETDUPE_API static void AppendAllLines(const String& path, const Array<String>& contents);
                DOTNETDUPE_API static Array<String> ReadAllLines(const String& path);
                DOTNETDUPE_API static void WriteAllLines(const String& path, const Array<String>& contents);
                DOTNETDUPE_API static void Create(const String& path);
                DOTNETDUPE_API static bool GetAttributes(const String& path, FileAttributes& fileAttributes);
                DOTNETDUPE_API static bool SetAttributes(const String& path, FileAttributes fileAttributes);
            private:
                static std::string ToNarrowPath(const String& path);
            };
        }
    }
}

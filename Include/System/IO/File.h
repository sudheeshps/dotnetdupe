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
                DOTNETDUPE_API static bool Exists(const String& sPath);
                DOTNETDUPE_API static String ReadAllText(const String& sPath);
                DOTNETDUPE_API static void WriteAllText(const String& sPath, const String& sContents);
                DOTNETDUPE_API static void Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite);
                DOTNETDUPE_API static void Move(const String& sSourceFileName, const String& sDestFileName);
                DOTNETDUPE_API static void Delete(const String& sPath);
                DOTNETDUPE_API static void AppendAllText(const String& sPath, const String& sContents);
                DOTNETDUPE_API static void AppendAllLines(const String& sPath, const Array<String>& sContents);
                DOTNETDUPE_API static Array<String> ReadAllLines(const String& sPath);
                DOTNETDUPE_API static void WriteAllLines(const String& sPath, const Array<String>& sContents);
                DOTNETDUPE_API static void Create(const String& sPath);
                DOTNETDUPE_API static bool GetAttributes(const String& sPath, FileAttributes& attrAttributes);
                DOTNETDUPE_API static bool SetAttributes(const String& sPath, FileAttributes attrAttributes);
            };
        }
    }
}

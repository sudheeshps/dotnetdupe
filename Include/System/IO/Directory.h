#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/DateTimeOffset.h"

#if defined(_WIN32)
#undef CreateDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#endif

namespace DotNetDupe {
    namespace System {
        namespace IO {
            class Directory : public Object {
            public:
                DOTNETDUPE_API static bool Exists(const String& sPath);
                DOTNETDUPE_API static void CreateDirectory(const String& sPath);
                DOTNETDUPE_API static void CreateDirectory(const String& sPath, bool bRecursive);
                DOTNETDUPE_API static void Delete(const String& sPath);
                DOTNETDUPE_API static void Delete(const String& sPath, bool bRecursive);
                DOTNETDUPE_API static void Move(const String& sSourceDirName, const String& sDestDirName);
                
                DOTNETDUPE_API static Array<String> GetFiles(const String& sPath);
                DOTNETDUPE_API static Array<String> GetFiles(const String& sPath, const String& sSearchPattern);
                
                DOTNETDUPE_API static Array<String> GetDirectories(const String& sPath);
                DOTNETDUPE_API static Array<String> GetDirectories(const String& sPath, const String& sSearchPattern);
                
                DOTNETDUPE_API static Array<String> GetFileSystemEntries(const String& sPath);
                DOTNETDUPE_API static Array<String> GetFileSystemEntries(const String& sPath, const String& sSearchPattern);
                
                DOTNETDUPE_API static String GetCurrentDirectory();
                DOTNETDUPE_API static void SetCurrentDirectory(const String& sPath);
                
                DOTNETDUPE_API static String GetDirectoryRoot(const String& sPath);
                DOTNETDUPE_API static DateTimeOffset GetCreationTime(const String& sPath);
                DOTNETDUPE_API static DateTimeOffset GetLastWriteTime(const String& sPath);
                DOTNETDUPE_API static DateTimeOffset GetLastAccessTime(const String& sPath);
            };
        }
    }
}

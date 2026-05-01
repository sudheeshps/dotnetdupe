#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/OperatingSystem.h"
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <map>

namespace DotNetDupe {
    namespace System {
        class Environment : public Object {
        public:
            enum class SpecialFolder {
                ApplicationData,
                CommonApplicationData,
                CommonProgramFiles,
                Cookies,
                Desktop,
                Favorites,
                History,
                InternetCache,
                LocalApplicationData,
                MyComputer,
                MyDocuments,
                MyMusic,
                MyPictures,
                MyVideos,
                ProgramFiles,
                Programs,
                Recent,
                SendTo,
                StartMenu,
                Startup,
                System,
                Templates,
                UserProfile
            };

            DOTNETDUPE_API static String GetMachineName();
            DOTNETDUPE_API static String GetUserName();
            DOTNETDUPE_API static int GetProcessorCount();
            DOTNETDUPE_API static String GetNewLine();
            DOTNETDUPE_API static String GetCurrentDirectory();
            DOTNETDUPE_API static String GetSystemDirectory();
            DOTNETDUPE_API static String GetOSVersion();
            DOTNETDUPE_API static String GetUserDomainName();
            DOTNETDUPE_API static String GetVersion();
            DOTNETDUPE_API static int64_t GetWorkingSet();
            DOTNETDUPE_API static void Exit(int exitCode);
            DOTNETDUPE_API static String ExpandEnvironmentVariables(const String& name);
            DOTNETDUPE_API static std::vector<String> GetCommandLineArgs();
            DOTNETDUPE_API static String GetEnvironmentVariable(const String& variable);
            DOTNETDUPE_API static std::map<String, String> GetEnvironmentVariables();
            DOTNETDUPE_API static String GetFolderPath(SpecialFolder folder);
            DOTNETDUPE_API static std::vector<String> GetLogicalDrives();
            DOTNETDUPE_API static void SetEnvironmentVariable(const String& variable, const String& value);
            DOTNETDUPE_API static OperatingSystem GetOperatingSystem();
        };
    }
}
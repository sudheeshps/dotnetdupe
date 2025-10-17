#include "pch.h"
#include "Environment.h"

namespace DotNetDupe
{
    namespace System
    {
        String Environment::GetMachineName()
        {
            TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
            GetComputerName(buffer, &size);
            return String(buffer);
        }

        String Environment::GetUserName()
        {
            TCHAR buffer[256];
            DWORD size = 256;
            ::GetUserName(buffer, &size);
            return String(buffer);
        }

        int Environment::GetProcessorCount()
        {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return sysInfo.dwNumberOfProcessors;
        }

        String Environment::GetNewLine()
        {
            return String(_T("\r\n"));
        }
    }
}

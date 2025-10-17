#pragma once

#include "Common.h"
#include "Object.h"
#include "String.h"
#include <Windows.h>
#include <tchar.h>

namespace DotNetDupe
{
    namespace System
    {
        class Environment : public Object
        {
        public:
            DOTNETDUPE_API static String GetMachineName();
            DOTNETDUPE_API static String GetUserName();
            DOTNETDUPE_API static int GetProcessorCount();
            DOTNETDUPE_API static String GetNewLine();
        };
    }
}
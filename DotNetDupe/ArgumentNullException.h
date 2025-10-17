#pragma once

#include "BasicArgumentException.h"

namespace DotNetDupe
{
    namespace System
    {
        class ArgumentNullException : public BasicArgumentException<TCHAR>
        {
        public:
            ArgumentNullException(const TCHAR* message) : BasicArgumentException<TCHAR>(message)
            {
            }
        };
    }
}
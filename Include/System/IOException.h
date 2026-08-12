#pragma once
#include <string>
#include "System/Exception.h"
namespace DotNetDupe {
    namespace System {
        namespace IO {
            class IOException : public BasicException<char> {
            public:
                IOException(const char* pchMessage);
                IOException(const String& sMessage);
            };
            inline IOException::IOException(const char* pchMessage) : BasicException<char>(pchMessage) { }
            inline IOException::IOException(const String& sMessage) : BasicException<char>(sMessage.GetRawString()) { }
        }
    }
}

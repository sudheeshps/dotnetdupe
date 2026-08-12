#include "pch.h"
#include "System/Exception.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        Exception::Exception(const String& sMessage) : std::exception(sMessage.GetRawString() ? sMessage.GetRawString() : "") {
        }
    }
}

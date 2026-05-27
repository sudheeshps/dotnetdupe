#pragma once
#include "System/BasicTimeoutException.h"
namespace DotNetDupe {
    namespace System {
        typedef BasicTimeoutException<char> TimeoutException;
    }
}

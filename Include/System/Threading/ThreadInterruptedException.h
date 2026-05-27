#pragma once
#include "System/Threading/BasicThreadInterruptedException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            typedef BasicThreadInterruptedException<char> ThreadInterruptedException;
        }
    }
}

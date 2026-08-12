#pragma once
#include "System/SystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            class AbandonedMutexException : public SystemException {
            public:
                AbandonedMutexException(const char* pchMessage);
            };
            inline AbandonedMutexException::AbandonedMutexException(const char* pchMessage) : SystemException(pchMessage) { }
        }
    }
}

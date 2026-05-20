#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicSynchronizationLockException : public BasicSystemException<CharT> {
            public:
                BasicSynchronizationLockException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicSynchronizationLockException<CharT>::BasicSynchronizationLockException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}

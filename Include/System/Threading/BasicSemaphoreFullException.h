#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicSemaphoreFullException : public BasicSystemException<CharT> {
            public:
                BasicSemaphoreFullException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicSemaphoreFullException<CharT>::BasicSemaphoreFullException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}

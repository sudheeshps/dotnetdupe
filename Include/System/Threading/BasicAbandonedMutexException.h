#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicAbandonedMutexException : public BasicSystemException<CharT> {
            public:
                BasicAbandonedMutexException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicAbandonedMutexException<CharT>::BasicAbandonedMutexException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}

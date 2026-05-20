#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicThreadInterruptedException : public BasicSystemException<CharT> {
            public:
                BasicThreadInterruptedException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicThreadInterruptedException<CharT>::BasicThreadInterruptedException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}

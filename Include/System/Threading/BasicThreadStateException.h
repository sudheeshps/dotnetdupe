#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        namespace Threading {
            template <class CharT> class BasicThreadStateException : public BasicSystemException<CharT> {
            public:
                BasicThreadStateException(const CharT* pchMessage);
            };
            template <class CharT>
            inline BasicThreadStateException<CharT>::BasicThreadStateException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
        }
    }
}

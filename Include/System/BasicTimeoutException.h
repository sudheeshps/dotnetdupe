#pragma once
#include "System/BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicTimeoutException : public BasicSystemException<CharT> {
        public:
            BasicTimeoutException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicTimeoutException<CharT>::BasicTimeoutException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
    }
}

#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicSystemException : public BasicException<CharT> {
        public:
            BasicSystemException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicSystemException<CharT>::BasicSystemException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}


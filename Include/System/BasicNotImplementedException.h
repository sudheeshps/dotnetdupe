#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicNotImplementedException : public BasicException<CharT> {
        public:
            BasicNotImplementedException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicNotImplementedException<CharT>::BasicNotImplementedException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}


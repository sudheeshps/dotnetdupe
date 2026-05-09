#pragma once
#include "BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicArgumentOutOfRangeException : public BasicException<CharT> {
        public:
            BasicArgumentOutOfRangeException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicArgumentOutOfRangeException<CharT>::BasicArgumentOutOfRangeException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}


#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicArgumentOutOfRangeException : public BasicException<CharT> {
        public:
            BasicArgumentOutOfRangeException(const CharT* pchMessage);
            BasicArgumentOutOfRangeException(const BasicString<CharT>& sMessage);
        };
        template <class CharT>
        inline BasicArgumentOutOfRangeException<CharT>::BasicArgumentOutOfRangeException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
        template <class CharT>
        inline BasicArgumentOutOfRangeException<CharT>::BasicArgumentOutOfRangeException(const BasicString<CharT>& sMessage) : BasicException<CharT>(sMessage) { }
    }
}


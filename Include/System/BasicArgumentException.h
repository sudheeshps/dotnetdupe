#pragma once
#include <string>
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicArgumentException : public BasicException<CharT> {
        public:
            BasicArgumentException(const CharT* pchMessage);
            BasicArgumentException(const BasicString<CharT>& sMessage);
        };
        template <class CharT> 
        inline BasicArgumentException<CharT>::BasicArgumentException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
        template <class CharT> 
        inline BasicArgumentException<CharT>::BasicArgumentException(const BasicString<CharT>& sMessage) : BasicException<CharT>(sMessage) { }
    }
}


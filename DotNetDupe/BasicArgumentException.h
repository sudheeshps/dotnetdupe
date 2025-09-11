#pragma once
#include <string>
#include "BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicArgumentException : public BasicException<CharT> {
        public:
            BasicArgumentException(const CharT* pchMessage);
        };
        template <class CharT> 
        inline BasicArgumentException<CharT>::BasicArgumentException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}


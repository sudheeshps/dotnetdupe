#pragma once
#include <string>
#include "BasicSystemException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicArithmeticException : public BasicSystemException<CharT> {
        public:
            BasicArithmeticException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicArithmeticException<CharT>::BasicArithmeticException(const CharT* pchMessage) : BasicSystemException<CharT>(pchMessage) { }
    }
}


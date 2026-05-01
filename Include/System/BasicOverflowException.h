#pragma once
#include <string>
#include "System/BasicArithmeticException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicOverflowException : public BasicArithmeticException<CharT> {
        public:
            BasicOverflowException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicOverflowException<CharT>::BasicOverflowException(const CharT* pchMessage) : BasicArithmeticException<CharT>(pchMessage) { }
    }
}


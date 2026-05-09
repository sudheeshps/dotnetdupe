#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicFormatException : public BasicException<CharT> {
        public:
            BasicFormatException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicFormatException<CharT>::BasicFormatException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}

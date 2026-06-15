#pragma once
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicInvalidOperationException : public BasicException<CharT> {
        public:
            BasicInvalidOperationException(const CharT* pchMessage);
        };
        template <class CharT>
        inline BasicInvalidOperationException<CharT>::BasicInvalidOperationException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
    }
}

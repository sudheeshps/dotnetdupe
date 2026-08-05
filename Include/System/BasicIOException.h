#pragma once
#include <string>
#include "System/BasicException.h"
namespace DotNetDupe {
    namespace System {
        namespace IO {
            template <class CharT> class BasicIOException : public BasicException<CharT> {
            public:
                BasicIOException(const CharT* pchMessage);
                BasicIOException(const BasicString<CharT>& sMessage);
            };
            template <class CharT> 
            inline BasicIOException<CharT>::BasicIOException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) { }
            template <class CharT>
            inline BasicIOException<CharT>::BasicIOException(const BasicString<CharT>& sMessage) : BasicException<CharT>(sMessage.GetRawString()) { }
        }
    }
}

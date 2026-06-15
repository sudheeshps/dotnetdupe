#pragma once
#include "System/BasicException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                template <class CharT>
                class BasicHttpRequestException : public BasicException<CharT> {
                public:
                    BasicHttpRequestException(const CharT* pchMessage) : BasicException<CharT>(pchMessage) {}
                };

                typedef BasicHttpRequestException<char> HttpRequestException;

            }
        }
    }
}

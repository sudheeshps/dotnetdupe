#pragma once
#include "System/BasicException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                template <class CharT>
                class BasicSocketException : public BasicException<CharT> {
                public:
                    BasicSocketException(const CharT* pchMessage) : BasicException<CharT>(pchMessage), m_errorCode(0) {}
                    BasicSocketException(const CharT* pchMessage, int errorCode) : BasicException<CharT>(pchMessage), m_errorCode(errorCode) {}
                    BasicSocketException(const BasicString<CharT>& sMessage) : BasicException<CharT>(sMessage), m_errorCode(0) {}
                    BasicSocketException(const BasicString<CharT>& sMessage, int errorCode) : BasicException<CharT>(sMessage), m_errorCode(errorCode) {}

                    int GetErrorCode() const { return m_errorCode; }

                private:
                    int m_errorCode;
                };

                typedef BasicSocketException<char> SocketException;

            }
        }
    }
}

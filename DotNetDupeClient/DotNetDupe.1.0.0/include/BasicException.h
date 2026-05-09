#pragma once
#include <string>
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicException {
        public:
            BasicException(const CharT* pchMessage);
            CharT* What() const;
        private:
            std::basic_string<CharT> m_strMessage;
        };

        template <class CharT>
        inline BasicException<CharT>::BasicException(const CharT* pchMessage) {
            m_strMessage = pchMessage;
        }

        template <class CharT>
        inline CharT* BasicException<CharT>::What() const {
            return m_strMessage.c_str();
        }
    }
}


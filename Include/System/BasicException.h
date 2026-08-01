#pragma once
#include <string>
namespace DotNetDupe {
    namespace System {
        template <class CharT> class BasicString;

        template <class CharT> class BasicException {
        public:
            BasicException(const CharT* pchMessage);
            BasicException(const BasicString<CharT>& sMessage);
            const CharT* What() const;
        private:
            std::basic_string<CharT> m_strMessage;
        };

        template <class CharT>
        inline BasicException<CharT>::BasicException(const CharT* pchMessage) {
            m_strMessage = pchMessage ? pchMessage : "";
        }

        template <class CharT>
        inline BasicException<CharT>::BasicException(const BasicString<CharT>& sMessage) {
            m_strMessage = sMessage.GetRawString();
        }

        template <class CharT>
        inline const CharT* BasicException<CharT>::What() const {
            return m_strMessage.c_str();
        }
    }
}


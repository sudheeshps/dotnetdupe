#pragma once
#include <exception>
#include "Common.h"

namespace DotNetDupe {
    namespace System {
        class String;

        class Exception : public std::exception {
        private:
            Exception* m_innerException = nullptr;
        public:
                DOTNETDUPE_API Exception();
                DOTNETDUPE_API Exception(const String& sMessage);
                DOTNETDUPE_API Exception(const String& sMessage, const Exception& innerException);
                DOTNETDUPE_API Exception(const Exception& other);
                DOTNETDUPE_API Exception& operator=(const Exception& other);
                DOTNETDUPE_API virtual ~Exception();

            const char* What() const { return what(); }
        };
    }
}

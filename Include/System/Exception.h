#pragma once
#include <exception>

namespace DotNetDupe {
    namespace System {
        class String;

        class Exception : public std::exception {
        public:
            Exception(const char* pchMessage) : std::exception(pchMessage ? pchMessage : "") {}
            Exception(const String& sMessage);
            const char* What() const { return what(); }
        };
    }
}

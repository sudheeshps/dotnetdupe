#pragma once
#include "System/BasicException.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {
            namespace Json {

                class JsonException : public BasicException<char> {
                public:
                    explicit JsonException(const char* message)
                        : BasicException<char>(message) {}
                };

            }
        }
    }
}

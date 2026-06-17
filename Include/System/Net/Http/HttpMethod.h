#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {
                class HttpMethod : public Object {
                public:
                    DOTNETDUPE_API explicit HttpMethod(const String& method);

                    DOTNETDUPE_API String GetMethod() const;
                    DOTNETDUPE_API String ToString() const;

                    DOTNETDUPE_API bool Equals(const HttpMethod& other) const;
                    DOTNETDUPE_API bool operator==(const HttpMethod& other) const;
                    DOTNETDUPE_API bool operator!=(const HttpMethod& other) const;

                    DOTNETDUPE_API static const HttpMethod Get;
                    DOTNETDUPE_API static const HttpMethod Post;
                    DOTNETDUPE_API static const HttpMethod Put;
                    DOTNETDUPE_API static const HttpMethod Delete;
                    DOTNETDUPE_API static const HttpMethod Head;
                    DOTNETDUPE_API static const HttpMethod Options;
                    DOTNETDUPE_API static const HttpMethod Trace;
                    DOTNETDUPE_API static const HttpMethod Patch;

                private:
                    String m_sMethod;
                };
            }
        }
    }
}

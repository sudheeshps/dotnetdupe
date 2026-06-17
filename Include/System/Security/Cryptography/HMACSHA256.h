#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {

                class HMACSHA256 : public Object {
                public:
                    DOTNETDUPE_API HMACSHA256();
                    DOTNETDUPE_API explicit HMACSHA256(const Array<char>& key);
                    DOTNETDUPE_API ~HMACSHA256() = default;

                    DOTNETDUPE_API Array<char> ComputeHash(const Array<char>& buffer);

                    DOTNETDUPE_API static Array<char> ComputeHash(const Array<char>& buffer, const Array<char>& key);

                private:
                    Array<char> m_key;
                };

            }
        }
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        class Guid : public Object {
        public:
            DOTNETDUPE_API Guid();
            DOTNETDUPE_API Guid(const Array<uint8_t>& b);
            DOTNETDUPE_API Guid(const String& g);

            DOTNETDUPE_API static Guid NewGuid();
            DOTNETDUPE_API static const Guid Empty;

            DOTNETDUPE_API Array<uint8_t> ToByteArray() const;
            DOTNETDUPE_API String ToString() const;

            DOTNETDUPE_API bool operator==(const Guid& other) const;
            DOTNETDUPE_API bool operator!=(const Guid& other) const;

        private:
            uint8_t _data[16];
        };
    }
}

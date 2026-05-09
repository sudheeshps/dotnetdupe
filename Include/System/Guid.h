#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include <cstdint>
#include <array>

namespace DotNetDupe {
    namespace System {
        class Guid : public Object {
        public:
            DOTNETDUPE_API Guid();
            DOTNETDUPE_API Guid(const std::array<uint8_t, 16>& b);
            DOTNETDUPE_API Guid(const String& g);

            DOTNETDUPE_API static Guid NewGuid();
            DOTNETDUPE_API static const Guid Empty;

            DOTNETDUPE_API std::array<uint8_t, 16> ToByteArray() const;
            DOTNETDUPE_API String ToString() const;

            DOTNETDUPE_API bool operator==(const Guid& other) const;
            DOTNETDUPE_API bool operator!=(const Guid& other) const;

        private:
            std::array<uint8_t, 16> _data;
        };
    }
}

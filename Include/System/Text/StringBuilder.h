#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include <string>
#include <sstream>

namespace DotNetDupe {
    namespace System {
        namespace Text {
            class StringBuilder : public Object {
            public:
                DOTNETDUPE_API StringBuilder();
                DOTNETDUPE_API StringBuilder(int capacity);
                DOTNETDUPE_API StringBuilder(const String& value);

                DOTNETDUPE_API int GetLength() const;
                DOTNETDUPE_API void SetLength(int value);
                DOTNETDUPE_API int GetCapacity() const;
                DOTNETDUPE_API void SetCapacity(int value);

                DOTNETDUPE_API StringBuilder& Append(const String& value);
                DOTNETDUPE_API StringBuilder& Append(const TCHAR* value);
                DOTNETDUPE_API StringBuilder& Append(TCHAR value);
                DOTNETDUPE_API StringBuilder& Append(int value);
                DOTNETDUPE_API StringBuilder& Append(long long value);
                DOTNETDUPE_API StringBuilder& Append(double value);
                DOTNETDUPE_API StringBuilder& Append(bool value);

                DOTNETDUPE_API StringBuilder& AppendLine();
                DOTNETDUPE_API StringBuilder& AppendLine(const String& value);

                DOTNETDUPE_API StringBuilder& Clear();
                
                DOTNETDUPE_API String ToString() const;

            private:
                std::basic_string<TCHAR> _buffer;
            };
        }
    }
}

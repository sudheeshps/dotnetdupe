#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Text {

            /// \class StringBuilder
            /// \brief Represents a mutable string of characters.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5.45 (System.Text.StringBuilder).
            ///       Provides high-performance contiguous buffer accumulation to avoid excessive string allocations.
            class StringBuilder : public Object {
            public:
                /// \brief Initializes a new instance of the StringBuilder class.
                DOTNETDUPE_API StringBuilder();

                /// \brief Initializes a new instance of the StringBuilder class using the specified capacity.
                /// \param capacity The suggested starting size of this instance.
                DOTNETDUPE_API StringBuilder(int capacity);

                /// \brief Initializes a new instance of the StringBuilder class using the specified string.
                /// \param value The string used to initialize the value of the instance.
                DOTNETDUPE_API StringBuilder(const String& value);

                /// \brief Destructor. Releases internal buffer.
                DOTNETDUPE_API ~StringBuilder() override;

                /// \brief Gets the length of the current StringBuilder object.
                /// \return The length of this instance.
                DOTNETDUPE_API int GetLength() const;

                /// \brief Sets the length of the current StringBuilder object.
                /// \param value The new length of this instance.
                DOTNETDUPE_API void SetLength(int value);

                /// \brief Gets the maximum number of characters that can be contained in the memory allocated by the current instance.
                /// \return The maximum number of characters that can be contained in the memory allocated by the current instance.
                DOTNETDUPE_API int GetCapacity() const;

                /// \brief Sets the capacity of the current StringBuilder object.
                /// \param value The new capacity of this instance.
                DOTNETDUPE_API void SetCapacity(int value);

                /// \brief Appends a copy of the specified string to this instance.
                /// \param value The string to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(const String& value);

                /// \brief Appends a copy of the specified null-terminated string to this instance.
                /// \param value The C-string to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(const char* value);

                /// \brief Appends the string representation of a specified Unicode character to this instance.
                /// \param value The character to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(char value);

                /// \brief Appends the string representation of a specified 32-bit signed integer to this instance.
                /// \param value The integer to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(int value);

                /// \brief Appends the string representation of a specified 64-bit signed integer to this instance.
                /// \param value The long long value to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(long long value);

                /// \brief Appends the string representation of a specified double-precision floating-point number to this instance.
                /// \param value The double value to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(double value);

                /// \brief Appends the string representation of a specified Boolean value to this instance.
                /// \param value The Boolean value to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& Append(bool value);

                /// \brief Appends the default line terminator to the end of the current StringBuilder object.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& AppendLine();

                /// \brief Appends a copy of the specified string followed by the default line terminator to the end of the current StringBuilder object.
                /// \param value The string to append.
                /// \return A reference to this instance after the append operation has completed.
                DOTNETDUPE_API StringBuilder& AppendLine(const String& value);

                /// \brief Removes all characters from the current StringBuilder instance.
                /// \return A reference to this instance with Length zero.
                DOTNETDUPE_API StringBuilder& Clear();
                
                /// \brief Converts the value of this instance to a String.
                /// \return A string whose value is the same as this instance.
                DOTNETDUPE_API String ToString() const;

            private:
                struct Impl;
                Impl* m_pImpl;
            };
        }
    }
}

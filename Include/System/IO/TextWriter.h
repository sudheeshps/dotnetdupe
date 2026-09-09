#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/IO/IDisposable.h"
#include "System/Text/TextEncoding.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class TextWriter
            /// \brief Represents a writer that can write a sequential series of characters.
            ///
            /// \details Abstract base class for character writers, compliant with
            /// ECMA-335 Partition IV Section 5.57 specifications. Provides primitive
            /// and formatted textual output routines with automatic newline handling.
            class TextWriter : public Object, public IDisposable {
            public:
                /// \brief Virtual destructor ensuring proper polymorphic cleanup.
                virtual ~TextWriter() = default;

                /// \brief Closes the current writer and releases any system resources associated with the writer.
                DOTNETDUPE_API virtual void Close() = 0;

                /// \brief Releases all resources used by the TextWriter object.
                DOTNETDUPE_API virtual void Dispose() override = 0;

                /// \brief Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.
                DOTNETDUPE_API virtual void Flush() = 0;

                /// \brief When overridden in a derived class, returns the character encoding in which the output is written.
                /// \return The character encoding in which the output is written.
                DOTNETDUPE_API virtual Text::EncodingPtr GetEncoding() const = 0;

                /// \brief Writes the text representation of a Boolean value to the text string or stream.
                /// \param bValue The Boolean value to write.
                DOTNETDUPE_API virtual void Write(bool bValue) = 0;

                /// \brief Writes a character to the text stream.
                /// \param chValue The character to write to the text stream.
                DOTNETDUPE_API virtual void Write(char chValue) = 0;

                /// \brief Writes a null-terminated C-string to the text stream.
                /// \param pValue The string to write.
                DOTNETDUPE_API virtual void Write(const char* pValue) = 0;

                /// \brief Writes a string to the text stream.
                /// \param sValue The string to write.
                DOTNETDUPE_API virtual void Write(const String& sValue) = 0;

                /// \brief Writes the text representation of a 4-byte signed integer to the text stream.
                /// \param iValue The 4-byte signed integer to write.
                DOTNETDUPE_API virtual void Write(int iValue) = 0;

                /// \brief Writes the text representation of an 8-byte signed integer to the text stream.
                /// \param llValue The 8-byte signed integer to write.
                DOTNETDUPE_API virtual void Write(long long llValue) = 0;

                /// \brief Writes the text representation of a 4-byte floating-point value to the text stream.
                /// \param fValue The 4-byte floating-point value to write.
                DOTNETDUPE_API virtual void Write(float fValue) = 0;

                /// \brief Writes the text representation of an 8-byte floating-point value to the text stream.
                /// \param dValue The 8-byte floating-point value to write.
                DOTNETDUPE_API virtual void Write(double dValue) = 0;

                /// \brief Writes a line terminator to the text stream.
                DOTNETDUPE_API virtual void WriteLine() = 0;

                /// \brief Writes the text representation of a Boolean value followed by a line terminator to the text stream.
                /// \param bValue The Boolean value to write.
                DOTNETDUPE_API virtual void WriteLine(bool bValue) = 0;

                /// \brief Writes a character followed by a line terminator to the text stream.
                /// \param chValue The character to write to the text stream.
                DOTNETDUPE_API virtual void WriteLine(char chValue) = 0;

                /// \brief Writes a null-terminated C-string followed by a line terminator to the text stream.
                /// \param pValue The string to write.
                DOTNETDUPE_API virtual void WriteLine(const char* pValue) = 0;

                /// \brief Writes a string followed by a line terminator to the text stream.
                /// \param sValue The string to write.
                DOTNETDUPE_API virtual void WriteLine(const String& sValue) = 0;

                /// \brief Writes the text representation of a 4-byte signed integer followed by a line terminator.
                /// \param iValue The integer to write.
                DOTNETDUPE_API virtual void WriteLine(int iValue) = 0;

                /// \brief Writes the text representation of an 8-byte signed integer followed by a line terminator.
                /// \param llValue The integer to write.
                DOTNETDUPE_API virtual void WriteLine(long long llValue) = 0;

                /// \brief Writes the text representation of a 4-byte floating-point value followed by a line terminator.
                /// \param fValue The float to write.
                DOTNETDUPE_API virtual void WriteLine(float fValue) = 0;

                /// \brief Writes the text representation of an 8-byte floating-point value followed by a line terminator.
                /// \param dValue The double to write.
                DOTNETDUPE_API virtual void WriteLine(double dValue) = 0;

            protected:
                /// \brief Initializes a new instance of the TextWriter class.
                TextWriter() = default;
                
                /// \brief Gets the default line terminator string.
                /// \return The default line terminator string (\r\n).
                DOTNETDUPE_API static const String& GetNewLine();
            };
        }
    }
}

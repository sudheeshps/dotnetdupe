#pragma once

#include "Common.h"
#include "System/IO/TextWriter.h"
#include "System/Text/StringBuilder.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class StringWriter
            /// \brief Implements a TextWriter that writes information to a string.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.59 specifications.
            /// Encapsulates a StringBuilder to produce text output dynamically in memory.
            class StringWriter : public TextWriter {
            public:
                /// \brief Initializes a new instance of the StringWriter class.
                DOTNETDUPE_API StringWriter();

                /// \brief Initializes a new instance of the StringWriter class that writes to the specified StringBuilder.
                /// \param sbOutput The StringBuilder object to write to.
                DOTNETDUPE_API StringWriter(Text::StringBuilder& sbOutput);

                /// \brief Virtual destructor ensuring proper cleanup.
                DOTNETDUPE_API virtual ~StringWriter() = default;

                /// \brief Closes the current StringWriter.
                DOTNETDUPE_API void Close() override;

                /// \brief Releases all resources used by the StringWriter.
                DOTNETDUPE_API void Dispose() override;

                /// \brief Clears all buffers for the current writer.
                DOTNETDUPE_API void Flush() override;

                /// \brief Gets the Encoding in which the output is written.
                /// \return The Encoding in which the output is written (UTF-8).
                DOTNETDUPE_API virtual Text::EncodingPtr GetEncoding() const override;

                /// \brief Returns a string containing the characters written to the current StringWriter so far.
                /// \return The string containing the characters written to the current StringWriter.
                DOTNETDUPE_API virtual String ToString() const;

                /// \brief Writes the text representation of a Boolean value to the text string.
                /// \param bValue The Boolean value to write.
                DOTNETDUPE_API void Write(bool bValue) override;

                /// \brief Writes a character to the string.
                /// \param chValue The character to write.
                DOTNETDUPE_API void Write(char chValue) override;

                /// \brief Writes a string to the current string.
                /// \param sValue The string to write.
                DOTNETDUPE_API void Write(const String& sValue) override;

                /// \brief Writes a null-terminated C-string to the current string.
                /// \param pValue The string to write.
                DOTNETDUPE_API void Write(const char* pValue) override;

                /// \brief Writes the text representation of a 4-byte signed integer.
                /// \param iValue The integer to write.
                DOTNETDUPE_API void Write(int iValue) override;

                /// \brief Writes the text representation of an 8-byte signed integer.
                /// \param llValue The integer to write.
                DOTNETDUPE_API void Write(long long llValue) override;

                /// \brief Writes the text representation of a 4-byte floating point value.
                /// \param fValue The float to write.
                DOTNETDUPE_API void Write(float fValue) override;

                /// \brief Writes the text representation of an 8-byte floating point value.
                /// \param dValue The double to write.
                DOTNETDUPE_API void Write(double dValue) override;

                /// \brief Writes a line terminator to the text string.
                DOTNETDUPE_API void WriteLine() override;

                /// \brief Writes the text representation of a Boolean followed by a line terminator.
                /// \param bValue The Boolean value to write.
                DOTNETDUPE_API void WriteLine(bool bValue) override;

                /// \brief Writes a character followed by a line terminator.
                /// \param chValue The character to write.
                DOTNETDUPE_API void WriteLine(char chValue) override;

                /// \brief Writes a null-terminated C-string followed by a line terminator.
                /// \param pValue The string to write.
                DOTNETDUPE_API void WriteLine(const char* pValue) override;

                /// \brief Writes a string followed by a line terminator.
                /// \param sValue The string to write.
                DOTNETDUPE_API void WriteLine(const String& sValue) override;

                /// \brief Writes the text representation of a 4-byte signed integer followed by a line terminator.
                /// \param iValue The integer to write.
                DOTNETDUPE_API void WriteLine(int iValue) override;

                /// \brief Writes the text representation of an 8-byte signed integer followed by a line terminator.
                /// \param llValue The integer to write.
                DOTNETDUPE_API void WriteLine(long long llValue) override;

                /// \brief Writes the text representation of a 4-byte floating-point value followed by a line terminator.
                /// \param fValue The float to write.
                DOTNETDUPE_API void WriteLine(float fValue) override;

                /// \brief Writes the text representation of an 8-byte floating-point value followed by a line terminator.
                /// \param dValue The double to write.
                DOTNETDUPE_API void WriteLine(double dValue) override;

            private:
                Text::StringBuilder* m_pSb;
                bool m_bIsOpen;
            };
        }
    }
}

#pragma once

#include "Common.h"
#include "System/IO/TextReader.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class StringReader
            /// \brief Implements a TextReader that reads from a string.
            ///
            /// \details Complies with ECMA-335 Partition IV Section 5.58 specifications.
            /// Provides high-efficiency character and line-by-line reading from an immutable
            /// String without intermediate stream allocations.
            class StringReader : public TextReader {
            public:
                /// \brief Initializes a new instance of the StringReader class that reads from the specified string.
                /// \param sSource The string to which the StringReader will be pointed.
                DOTNETDUPE_API StringReader(const String& sSource);

                /// \brief Virtual destructor ensuring proper resource cleanup.
                DOTNETDUPE_API virtual ~StringReader() = default;

                /// \brief Closes the StringReader.
                DOTNETDUPE_API void Close() override;

                /// \brief Releases all resources used by the StringReader.
                DOTNETDUPE_API void Dispose() override;

                /// \brief Returns the next available character but does not consume it.
                /// \return An integer representing the next character to be read, or -1 if no characters are available.
                DOTNETDUPE_API int Peek() override;

                /// \brief Reads the next character from the input string and advances the character position by one character.
                /// \return The next character from the underlying string, or -1 if no characters are available.
                DOTNETDUPE_API int Read() override;

                /// \brief Reads a block of characters from the input string and advances the character position by count.
                /// \param pBuffer When this method returns, contains the specified character array with values between index and (index + count - 1).
                /// \param iIndex The starting index in the buffer.
                /// \param nCount The number of characters to read.
                /// \return The total number of characters read into the buffer.
                DOTNETDUPE_API int Read(char* pBuffer, int iIndex, int nCount) override;

                /// \brief Reads a line of characters from the current string and returns the data as a string.
                /// \return The next line from the current string, or an empty string if the end of the string is reached.
                DOTNETDUPE_API String ReadLine() override;

                /// \brief Reads all characters from the current position to the end of the string and returns them as a single string.
                /// \return The content from the current position to the end of the underlying string.
                DOTNETDUPE_API String ReadToEnd() override;

            private:
                String m_sSource;
                int m_iPos;
                int m_nLength;
            };
        }
    }
}

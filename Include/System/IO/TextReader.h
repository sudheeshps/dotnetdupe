#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class TextReader
            /// \brief Represents a reader that can read a sequential series of characters.
            ///
            /// \details Abstract base class for character readers, compliant with
            /// ECMA-335 Partition IV Section 5.56 specifications. Provides high-level
            /// text consumption routines including Read, Peek, ReadLine, and ReadToEnd.
            class TextReader : public Object, public IDisposable {
            public:
                /// \brief Virtual destructor ensuring proper polymorphic cleanup.
                virtual ~TextReader() = default;

                /// \brief Closes the TextReader and releases any system resources associated with the reader.
                DOTNETDUPE_API virtual void Close() = 0;

                /// \brief Releases all resources used by the TextReader.
                DOTNETDUPE_API virtual void Dispose() override = 0;

                /// \brief Reads the next character without changing the state of the reader or the character source.
                /// \return An integer representing the next character to be read, or -1 if no more characters are available.
                DOTNETDUPE_API virtual int Peek() = 0;

                /// \brief Reads the next character from the text reader and advances the character position by one character.
                /// \return The next character from the text reader, or -1 if no more characters are available.
                DOTNETDUPE_API virtual int Read() = 0;

                /// \brief Reads a specified maximum number of characters from the current text reader and writes the data to a buffer.
                /// \param pBuffer When this method returns, contains the specified character array with values between index and (index + count - 1).
                /// \param iIndex The position in buffer at which to begin writing.
                /// \param nCount The maximum number of characters to read.
                /// \return The number of characters that have been read.
                DOTNETDUPE_API virtual int Read(char* pBuffer, int iIndex, int nCount) = 0;

                /// \brief Reads a line of characters from the text reader and returns the data as a string.
                /// \return The next line from the reader, or an empty string if all characters have been read.
                DOTNETDUPE_API virtual String ReadLine() = 0;

                /// \brief Reads all characters from the current position to the end of the text reader and returns them as one string.
                /// \return A string that contains all characters from the current position to the end of the text reader.
                DOTNETDUPE_API virtual String ReadToEnd() = 0;

            protected:
                /// \brief Initializes a new instance of the TextReader class.
                TextReader() = default;
            };
        }
    }
}

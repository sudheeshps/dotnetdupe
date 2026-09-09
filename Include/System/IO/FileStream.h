#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class FileStream
            /// \brief Provides a Stream for a file, supporting both synchronous read and write operations.
            ///
            /// \details Implements file I/O operations compliant with ECMA-335 Partition IV Section 5.52
            /// and POSIX / Win32 native filesystem file handle semantics. Employs RAII for deterministic
            /// file descriptor / handle closure on destruction or disposal.
            class FileStream : public Stream {
            public:
                /// \brief Initializes a new instance of the FileStream class with the specified path and creation mode.
                /// \param sPath A relative or absolute path for the file that the current FileStream object will encapsulate.
                /// \param iMode A constant that determines how to open or create the file.
                /// \throws ArgumentException If the mode parameter is invalid.
                /// \throws IOException If an I/O error occurs or the file already exists when creating new.
                DOTNETDUPE_API FileStream(const String& sPath, int iMode);

                /// \brief Destructor releasing encapsulated native stream resources.
                DOTNETDUPE_API ~FileStream() override;

                /// \brief Gets a value indicating whether the current stream supports reading.
                /// \return true if the stream supports reading; otherwise, false.
                DOTNETDUPE_API bool CanRead() const override;

                /// \brief Gets a value indicating whether the current stream supports seeking.
                /// \return true if the stream supports seeking; otherwise, false.
                DOTNETDUPE_API bool CanSeek() const override;

                /// \brief Gets a value indicating whether the current stream supports writing.
                /// \return true if the stream supports writing; otherwise, false.
                DOTNETDUPE_API bool CanWrite() const override;

                /// \brief Gets the length in bytes of the stream.
                /// \return A long value representing the length of the stream in bytes.
                /// \throws IOException If seeking is not supported or file handle is invalid.
                DOTNETDUPE_API long GetLength() const override;

                /// \brief Gets the current position within the stream.
                /// \return The current position within the stream.
                /// \throws IOException If seeking is not supported or file handle is invalid.
                DOTNETDUPE_API long GetPosition() const override;

                /// \brief Sets the current position within the stream.
                /// \param llValue The new position within the stream.
                /// \throws IOException If seeking is not supported or file handle is invalid.
                DOTNETDUPE_API void SetPosition(long llValue) override;

                /// \brief Clears buffers for this stream and causes any unwritten data to be written to the file.
                /// \throws IOException If an I/O error occurs during flushing.
                DOTNETDUPE_API void Flush() override;

                /// \brief Reads a block of bytes from the stream and writes the data in a given buffer.
                /// \param pBuffer When this method returns, contains the specified byte array with the values between offset and (offset + count - 1).
                /// \param iOffset The byte offset in array at which the read bytes will be placed.
                /// \param nCount The maximum number of bytes to read.
                /// \return The total number of bytes read into the buffer.
                /// \throws IOException If an I/O error occurs or stream is not readable.
                DOTNETDUPE_API int Read(char* pBuffer, int iOffset, int nCount) override;

                /// \brief Sets the current position of this stream to the given value.
                /// \param llOffset The point relative to origin from which to begin seeking.
                /// \param iOrigin A SeekOrigin value indicating the reference point from which the new position is to be obtained.
                /// \return The new position in the stream.
                /// \throws ArgumentException If origin is invalid.
                /// \throws IOException If an I/O error occurs or seeking is not supported.
                DOTNETDUPE_API long Seek(long llOffset, int iOrigin) override;

                /// \brief Sets the length of this stream to the given value.
                /// \param llValue The new length of the stream.
                /// \throws IOException Operation is not supported on FileStream.
                DOTNETDUPE_API void SetLength(long llValue) override;

                /// \brief Writes a block of bytes to the file stream.
                /// \param pBuffer The buffer containing data to write to the stream.
                /// \param iOffset The zero-based byte offset in array from which to begin copying bytes.
                /// \param nCount The number of bytes to be written.
                /// \throws IOException If an I/O error occurs or stream is not writable.
                DOTNETDUPE_API void Write(const char* pBuffer, int iOffset, int nCount) override;

                /// \brief Releases all unmanaged resources used by the FileStream.
                DOTNETDUPE_API void Dispose() override;

            private:
                struct Impl;
                Impl* m_pImpl;
                String m_sPath;
                int m_iMode;
                bool m_bCanRead;
                bool m_bCanWrite;
                bool m_bCanSeek;
            };
        }
    }
}

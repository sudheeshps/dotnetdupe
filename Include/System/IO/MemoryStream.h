#pragma once
#include "Common.h"
#include "System/IO/Stream.h"
#include "System/Array.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class MemoryStream
            /// \brief Creates a stream whose backing store is memory.
            ///
            /// \details Implements an expandable, in-memory byte buffer compliant with
            /// ECMA-335 Partition IV Section 5.53 specifications. Provides high-throughput
            /// random-access stream semantics without file-system overhead.
            class MemoryStream : public Stream {
            public:
                /// \brief Initializes a new non-resizable instance of the MemoryStream class with an expandable capacity initialized to zero.
                DOTNETDUPE_API MemoryStream();

                /// \brief Initializes a new non-resizable instance of the MemoryStream class based on the specified byte array.
                /// \param buffer The array of unsigned bytes from which to create the current stream.
                DOTNETDUPE_API explicit MemoryStream(const Array<char>& buffer);

                /// \brief Initializes a new non-resizable instance of the MemoryStream class based on the specified byte array, with the CanWrite property set as specified.
                /// \param buffer The array of unsigned bytes from which to create this stream.
                /// \param writable The setting of the CanWrite property, which determines whether the stream supports writing.
                DOTNETDUPE_API MemoryStream(const Array<char>& buffer, bool writable);

                /// \brief Destructor releasing encapsulated memory resources.
                DOTNETDUPE_API ~MemoryStream();

                /// \brief Gets a value indicating whether the current stream supports reading.
                /// \return true if the stream is open; otherwise, false.
                DOTNETDUPE_API bool CanRead() const override;

                /// \brief Gets a value indicating whether the current stream supports seeking.
                /// \return true if the stream is open; otherwise, false.
                DOTNETDUPE_API bool CanSeek() const override;

                /// \brief Gets a value indicating whether the current stream supports writing.
                /// \return true if the stream supports writing; otherwise, false.
                DOTNETDUPE_API bool CanWrite() const override;

                /// \brief Returns the length of the stream in bytes.
                /// \return The length of the stream in bytes.
                /// \throws IOException If the stream is closed.
                DOTNETDUPE_API long GetLength() const override;

                /// \brief Gets the current position within the stream.
                /// \return The current position within the stream.
                /// \throws IOException If the stream is closed.
                DOTNETDUPE_API long GetPosition() const override;

                /// \brief Sets the current position within the stream.
                /// \param value The position within the stream.
                /// \throws IOException If the stream is closed.
                /// \throws ArgumentOutOfRangeException If value is negative.
                DOTNETDUPE_API void SetPosition(long value) override;

                /// \brief Overrides the Flush method so that no action is performed.
                DOTNETDUPE_API void Flush() override;

                /// \brief Reads a block of bytes from the current stream and writes the data to a buffer.
                /// \param buffer When this method returns, the buffer contains the specified byte array with the values between offset and (offset + count - 1).
                /// \param offset The zero-based byte offset in buffer at which to begin storing the data read from the current stream.
                /// \param count The maximum number of bytes to be read.
                /// \return The total number of bytes read into the buffer.
                /// \throws IOException If the stream is closed.
                /// \throws ArgumentNullException If buffer is null.
                /// \throws ArgumentOutOfRangeException If offset or count is negative.
                DOTNETDUPE_API int Read(char* buffer, int offset, int count) override;

                /// \brief Sets the position within the current stream to the specified value.
                /// \param offset The new position within the stream.
                /// \param origin A value of type SeekOrigin indicating the reference point used to obtain the new position.
                /// \return The new position within the stream.
                /// \throws IOException If the stream is closed or offset seeks before beginning.
                /// \throws ArgumentException If origin is invalid.
                DOTNETDUPE_API long Seek(long offset, int origin) override;

                /// \brief Sets the length of the current stream to the specified value.
                /// \param value The desired length of the current stream in bytes.
                /// \throws IOException If the stream is closed or not writable.
                /// \throws ArgumentOutOfRangeException If value is negative.
                DOTNETDUPE_API void SetLength(long value) override;

                /// \brief Writes a block of bytes to the current stream using data read from a buffer.
                /// \param buffer The buffer to write data from.
                /// \param offset The zero-based byte offset in buffer from which to begin copying bytes.
                /// \param count The maximum number of bytes to write.
                /// \throws IOException If the stream is closed or not writable.
                /// \throws ArgumentNullException If buffer is null.
                /// \throws ArgumentOutOfRangeException If offset or count is negative.
                DOTNETDUPE_API void Write(const char* buffer, int offset, int count) override;

                /// \brief Releases the unmanaged resources used by the MemoryStream.
                DOTNETDUPE_API void Dispose() override;

                /// \brief Writes the stream contents to a byte array, regardless of the Position property.
                /// \return A new byte array.
                DOTNETDUPE_API Array<char> ToArray() const;

            private:
                struct MemoryStreamImpl;
                MemoryStreamImpl* m_pImpl;
            };
        }
    }
}

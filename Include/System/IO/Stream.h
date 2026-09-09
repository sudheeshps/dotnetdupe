#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/IO/IDisposable.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            /// \class Stream
            /// \brief Provides a generic view of a sequence of bytes.
            ///
            /// \details Abstract base class for all data streams, compliant with
            /// ECMA-335 Partition IV Section 5.50 specifications. Streams encapsulate
            /// reading, writing, seeking, and lifecycle management for arbitrary underlying media.
            class Stream : public Object, public IDisposable {
            public:
                /// \brief Virtual destructor ensuring proper polymorphic destruction.
                virtual ~Stream() = default;

                /// \brief Gets a value indicating whether the current stream supports reading.
                /// \return true if the stream supports reading; otherwise, false.
                virtual bool CanRead() const = 0;

                /// \brief Gets a value indicating whether the current stream supports seeking.
                /// \return true if the stream supports seeking; otherwise, false.
                virtual bool CanSeek() const = 0;

                /// \brief Gets a value indicating whether the current stream supports writing.
                /// \return true if the stream supports writing; otherwise, false.
                virtual bool CanWrite() const = 0;

                /// \brief Gets the length in bytes of the stream.
                /// \return A long value representing the length of the stream in bytes.
                /// \throws IOException If seeking is not supported.
                virtual long GetLength() const = 0;

                /// \brief Gets the position within the current stream.
                /// \return The current position within the stream.
                /// \throws IOException If seeking is not supported.
                virtual long GetPosition() const = 0;

                /// \brief Sets the position within the current stream.
                /// \param value The new position within the stream.
                /// \throws IOException If seeking is not supported or value is negative.
                virtual void SetPosition(long value) = 0;

                /// \brief Clears all buffers for this stream and causes any buffered data to be written to the underlying device.
                virtual void Flush() = 0;

                /// \brief Reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.
                /// \param buffer An array of bytes to store the read data.
                /// \param offset The zero-based byte offset in buffer at which to begin storing the data.
                /// \param count The maximum number of bytes to be read from the current stream.
                /// \return The total number of bytes read into the buffer.
                virtual int Read(char* buffer, int offset, int count) = 0;

                /// \brief Sets the position within the current stream.
                /// \param offset A byte offset relative to the origin parameter.
                /// \param origin A value of type SeekOrigin indicating the reference point used to obtain the new position.
                /// \return The new position within the current stream.
                virtual long Seek(long offset, int origin) = 0;

                /// \brief Sets the length of the current stream.
                /// \param value The desired length of the current stream in bytes.
                virtual void SetLength(long value) = 0;

                /// \brief Writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
                /// \param buffer An array of bytes containing data to write.
                /// \param offset The zero-based byte offset in buffer at which to begin copying bytes.
                /// \param count The number of bytes to be written.
                virtual void Write(const char* buffer, int offset, int count) = 0;

                /// \brief Releases all resources used by the Stream.
                virtual void Dispose() = 0;
            };
        }
    }
}

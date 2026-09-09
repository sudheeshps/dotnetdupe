#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/Net/Sockets/Socket.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                /// \class NetworkStream
                /// \brief Provides the underlying stream of data for network access.
                ///
                /// \details Bridges the IO::Stream abstraction with a connected Socket, compliant with
                /// RFC 793 and ECMA-335 Partition IV Section 5.50. Does not support seeking, but provides
                /// synchronous read, write, flush, and ownership management for underlying network sockets.
                class NetworkStream : public IO::Stream {
                public:
                    /// \brief Creates a new instance of the NetworkStream class for the specified Socket.
                    /// \param socket The Socket that the NetworkStream will use to send and receive data.
                    /// \throws ArgumentNullException If socket is null.
                    DOTNETDUPE_API explicit NetworkStream(const SmartPointer<Socket>& socket);

                    /// \brief Initializes a new instance of the NetworkStream class for the specified Socket with the specified Socket ownership.
                    /// \param socket The Socket that the NetworkStream will use to send and receive data.
                    /// \param bOwnsSocket true to indicate that the NetworkStream will take ownership of the Socket; otherwise, false.
                    /// \throws ArgumentNullException If socket is null.
                    DOTNETDUPE_API NetworkStream(const SmartPointer<Socket>& socket, bool bOwnsSocket);

                    /// \brief Destructor closing stream and owned socket.
                    DOTNETDUPE_API ~NetworkStream();

                    /// \brief Gets a value that indicates whether the NetworkStream supports reading.
                    /// \return true if data can be read from the stream; otherwise, false.
                    DOTNETDUPE_API bool CanRead() const override;

                    /// \brief Gets a value that indicates whether the stream supports seeking. Always returns false.
                    /// \return false in all cases.
                    DOTNETDUPE_API bool CanSeek() const override;

                    /// \brief Gets a value that indicates whether the NetworkStream supports writing.
                    /// \return true if data can be written to the NetworkStream; otherwise, false.
                    DOTNETDUPE_API bool CanWrite() const override;

                    /// \brief Gets the length of the data available on the stream. Not supported.
                    /// \throws IOException In all cases.
                    DOTNETDUPE_API long GetLength() const override;

                    /// \brief Gets the current position within the stream. Not supported.
                    /// \throws IOException In all cases.
                    DOTNETDUPE_API long GetPosition() const override;

                    /// \brief Sets the current position of the stream. Not supported.
                    /// \throws IOException In all cases.
                    DOTNETDUPE_API void SetPosition(long value) override;

                    /// \brief Flushes data from the stream.
                    DOTNETDUPE_API void Flush() override;

                    /// \brief Reads data from the NetworkStream and stores it in the buffer.
                    /// \param buffer The location in memory to store data read from the NetworkStream.
                    /// \param offset The location in buffer to begin storing the data to.
                    /// \param count The number of bytes to read from the NetworkStream.
                    /// \return The number of bytes read from the NetworkStream.
                    /// \throws IOException If the stream is disposed or receive fails.
                    DOTNETDUPE_API int Read(char* buffer, int offset, int count) override;

                    /// \brief Sets the current position of the stream to the given value. Not supported.
                    /// \throws IOException In all cases.
                    DOTNETDUPE_API long Seek(long offset, int origin) override;

                    /// \brief Sets the length of the stream. Not supported.
                    /// \throws IOException In all cases.
                    DOTNETDUPE_API void SetLength(long value) override;

                    /// \brief Writes data to the NetworkStream.
                    /// \param buffer The data to write to the NetworkStream.
                    /// \param offset The location in buffer from which to start writing data.
                    /// \param count The number of bytes to write to the NetworkStream.
                    /// \throws IOException If the stream is disposed or send fails.
                    DOTNETDUPE_API void Write(const char* buffer, int offset, int count) override;

                    /// \brief Releases the unmanaged resources used by the NetworkStream.
                    DOTNETDUPE_API void Dispose() override;

                    /// \brief Closes the NetworkStream and releases all resources.
                    DOTNETDUPE_API void Close();

                private:
                    SmartPointer<Socket> m_pSocket;
                    bool m_bOwnsSocket;
                    bool m_bDisposed;
                };

            }
        }
    }
}

/// \file MockStream.h
/// \brief Mock stream implementation for unit testing and diagnostic validation.

#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {

            /// \class MockStream
            /// \brief Controllable stream implementation for unit testing IO consumers.
            ///
            /// MockStream provides configurable capabilities (CanRead, CanWrite, CanSeek)
            /// and simulated failure modes (SetThrowOnRead, SetThrowOnWrite) to facilitate
            /// deterministic testing of stream readers, writers, and pipelines.
            class MockStream : public Stream {
            private:
                struct Impl;
                Impl* m_pImpl;

            public:
                /// \brief Initializes a new instance of MockStream with configurable capabilities.
                /// \param bCanRead Whether read operations are permitted.
                /// \param bCanWrite Whether write operations are permitted.
                /// \param bCanSeek Whether seek operations are permitted.
                DOTNETDUPE_API MockStream(bool bCanRead = true, bool bCanWrite = true, bool bCanSeek = true);

                /// \brief Releases resources used by the MockStream.
                DOTNETDUPE_API virtual ~MockStream() override;

                /// \brief Configures whether future read operations throw an IOException.
                /// \param bThrow True to throw IOException on Read; false to operate normally.
                DOTNETDUPE_API void SetThrowOnRead(bool bThrow);

                /// \brief Configures whether future write operations throw an IOException.
                /// \param bThrow True to throw IOException on Write; false to operate normally.
                DOTNETDUPE_API void SetThrowOnWrite(bool bThrow);

                /// \brief Sets whether read operations are supported.
                /// \param bCanRead True if reading is supported; otherwise false.
                DOTNETDUPE_API void SetCanRead(bool bCanRead);

                /// \brief Sets whether write operations are supported.
                /// \param bCanWrite True if writing is supported; otherwise false.
                DOTNETDUPE_API void SetCanWrite(bool bCanWrite);

                /// \brief Sets whether seeking is supported.
                /// \param bCanSeek True if seeking is supported; otherwise false.
                DOTNETDUPE_API void SetCanSeek(bool bCanSeek);

                /// \brief Checks whether the stream has been disposed.
                /// \return True if disposed; otherwise false.
                DOTNETDUPE_API bool IsDisposed() const;

                /// \brief Gets a value indicating whether the current stream supports reading.
                /// \return True if reading is supported; otherwise false.
                DOTNETDUPE_API bool CanRead() const override;

                /// \brief Gets a value indicating whether the current stream supports seeking.
                /// \return True if seeking is supported; otherwise false.
                DOTNETDUPE_API bool CanSeek() const override;

                /// \brief Gets a value indicating whether the current stream supports writing.
                /// \return True if writing is supported; otherwise false.
                DOTNETDUPE_API bool CanWrite() const override;

                /// \brief Gets the length in bytes of the stream.
                /// \return Stream length in bytes.
                DOTNETDUPE_API long GetLength() const override;

                /// \brief Gets the current position within the stream.
                /// \return Current byte position.
                DOTNETDUPE_API long GetPosition() const override;

                /// \brief Sets the current position within the stream.
                /// \param lValue Target position offset.
                DOTNETDUPE_API void SetPosition(long lValue) override;

                /// \brief Clears all buffers for this stream.
                DOTNETDUPE_API void Flush() override;

                /// \brief Reads a sequence of bytes from the current stream.
                /// \param pBuffer Output buffer to receive bytes.
                /// \param iOffset Offset within buffer.
                /// \param iCount Maximum number of bytes to read.
                /// \return Total number of bytes read.
                DOTNETDUPE_API int Read(char* pBuffer, int iOffset, int iCount) override;

                /// \brief Sets the position within the current stream.
                /// \param lOffset Byte offset relative to origin.
                /// \param iOrigin Seek origin (0=Begin, 1=Current, 2=End).
                /// \return The new position within the stream.
                DOTNETDUPE_API long Seek(long lOffset, int iOrigin) override;

                /// \brief Sets the length of the current stream.
                /// \param lValue Desired stream length in bytes.
                DOTNETDUPE_API void SetLength(long lValue) override;

                /// \brief Writes a sequence of bytes to the current stream.
                /// \param pBuffer Buffer containing bytes to write.
                /// \param iOffset Offset within buffer.
                /// \param iCount Number of bytes to write.
                DOTNETDUPE_API void Write(const char* pBuffer, int iOffset, int iCount) override;

                /// \brief Closes and releases all resources associated with the stream.
                DOTNETDUPE_API void Dispose() override;
            };

        }
    }
}

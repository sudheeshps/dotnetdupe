#include "pch.h"
#include "System/IO/MockStream.h"
#include "System/String.h"
#include "System/IOException.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            struct MockStream::Impl {
                std::vector<char> m_buffer;
                long m_lPosition;
                bool m_bCanRead;
                bool m_bCanWrite;
                bool m_bCanSeek;
                bool m_bThrowOnRead;
                bool m_bThrowOnWrite;
                bool m_bIsDisposed;
            };

            MockStream::MockStream(bool bCanRead, bool bCanWrite, bool bCanSeek)
                : m_pImpl(new Impl()) {
                /// Step: Initialize mock stream state and capabilities.
                m_pImpl->m_lPosition = 0;
                m_pImpl->m_bCanRead = bCanRead;
                m_pImpl->m_bCanWrite = bCanWrite;
                m_pImpl->m_bCanSeek = bCanSeek;
                m_pImpl->m_bThrowOnRead = false;
                m_pImpl->m_bThrowOnWrite = false;
                m_pImpl->m_bIsDisposed = false;
            }

            MockStream::~MockStream() {
                /// Step: Release allocated implementation memory.
                delete m_pImpl;
            }

            void MockStream::SetThrowOnRead(bool bThrow) {
                /// Step: Configure read fault injection.
                m_pImpl->m_bThrowOnRead = bThrow;
            }

            void MockStream::SetThrowOnWrite(bool bThrow) {
                /// Step: Configure write fault injection.
                m_pImpl->m_bThrowOnWrite = bThrow;
            }

            void MockStream::SetCanRead(bool bCanRead) {
                /// Step: Set read capability flag.
                m_pImpl->m_bCanRead = bCanRead;
            }

            void MockStream::SetCanWrite(bool bCanWrite) {
                /// Step: Set write capability flag.
                m_pImpl->m_bCanWrite = bCanWrite;
            }

            void MockStream::SetCanSeek(bool bCanSeek) {
                /// Step: Set seek capability flag.
                m_pImpl->m_bCanSeek = bCanSeek;
            }

            bool MockStream::IsDisposed() const {
                /// Return: Stream disposal state.
                return m_pImpl->m_bIsDisposed;
            }

            bool MockStream::CanRead() const {
                /// Return: True if readable and not disposed.
                return m_pImpl->m_bCanRead && !m_pImpl->m_bIsDisposed;
            }

            bool MockStream::CanSeek() const {
                /// Return: True if seekable and not disposed.
                return m_pImpl->m_bCanSeek && !m_pImpl->m_bIsDisposed;
            }

            bool MockStream::CanWrite() const {
                /// Return: True if writable and not disposed.
                return m_pImpl->m_bCanWrite && !m_pImpl->m_bIsDisposed;
            }

            long MockStream::GetLength() const {
                /// Guard: Check if disposed.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                /// Return: Total buffer length.
                return static_cast<long>(m_pImpl->m_buffer.size());
            }

            long MockStream::GetPosition() const {
                /// Guard: Check if disposed.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                /// Return: Current stream position.
                return m_pImpl->m_lPosition;
            }

            void MockStream::SetPosition(long lValue) {
                /// Guard: Check if disposed or seek not supported.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek) throw IOException("Stream does not support seeking.");
                /// Step: Update stream position.
                m_pImpl->m_lPosition = lValue;
            }

            void MockStream::Flush() {
                /// Guard: Check if disposed.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
            }

            int MockStream::Read(char* pBuffer, int iOffset, int iCount) {
                /// Guard: Validate state, capabilities, and parameters.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanRead) throw IOException("Stream does not support reading.");
                if (m_pImpl->m_bThrowOnRead) throw IOException("Simulated read fault.");
                if (!pBuffer || iOffset < 0 || iCount < 0) return 0;

                /// Step: Calculate available bytes.
                long lAvailable = static_cast<long>(m_pImpl->m_buffer.size()) - m_pImpl->m_lPosition;
                if (lAvailable <= 0) return 0;

                /// Step: Copy bytes into destination buffer.
                int iToRead = static_cast<int>(lAvailable < iCount ? lAvailable : iCount);
                for (int i = 0; i < iToRead; ++i) {
                    pBuffer[iOffset + i] = m_pImpl->m_buffer[m_pImpl->m_lPosition + i];
                }
                m_pImpl->m_lPosition += iToRead;
                return iToRead;
            }

            long MockStream::Seek(long lOffset, int iOrigin) {
                /// Guard: Validate state and seek capability.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek) throw IOException("Stream does not support seeking.");

                /// Step: Calculate target position relative to origin.
                long lNewPos = m_pImpl->m_lPosition;
                if (iOrigin == 0) lNewPos = lOffset;
                else if (iOrigin == 1) lNewPos += lOffset;
                else if (iOrigin == 2) lNewPos = static_cast<long>(m_pImpl->m_buffer.size()) + lOffset;

                /// Step: Clamp and assign new position.
                if (lNewPos < 0) lNewPos = 0;
                m_pImpl->m_lPosition = lNewPos;
                return m_pImpl->m_lPosition;
            }

            void MockStream::SetLength(long lValue) {
                /// Guard: Validate state and seek/write capabilities.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek || !m_pImpl->m_bCanWrite) throw IOException("Stream does not support seeking or writing.");
                /// Step: Resize underlying storage buffer.
                m_pImpl->m_buffer.resize(static_cast<size_t>(lValue), 0);
            }

            void MockStream::Write(const char* pBuffer, int iOffset, int iCount) {
                /// Guard: Validate state, capabilities, and parameters.
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanWrite) throw IOException("Stream does not support writing.");
                if (m_pImpl->m_bThrowOnWrite) throw IOException("Simulated write fault.");
                if (!pBuffer || iOffset < 0 || iCount <= 0) return;

                /// Step: Expand buffer if needed to accommodate data.
                size_t uRequiredSize = static_cast<size_t>(m_pImpl->m_lPosition + iCount);
                if (m_pImpl->m_buffer.size() < uRequiredSize) {
                    m_pImpl->m_buffer.resize(uRequiredSize, 0);
                }

                /// Step: Copy bytes into internal storage buffer.
                for (int i = 0; i < iCount; ++i) {
                    m_pImpl->m_buffer[m_pImpl->m_lPosition + i] = pBuffer[iOffset + i];
                }
                m_pImpl->m_lPosition += iCount;
            }

            void MockStream::Dispose() {
                /// Step: Mark stream instance as disposed.
                m_pImpl->m_bIsDisposed = true;
            }

        }
    }
}

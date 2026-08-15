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
                m_pImpl->m_lPosition = 0;
                m_pImpl->m_bCanRead = bCanRead;
                m_pImpl->m_bCanWrite = bCanWrite;
                m_pImpl->m_bCanSeek = bCanSeek;
                m_pImpl->m_bThrowOnRead = false;
                m_pImpl->m_bThrowOnWrite = false;
                m_pImpl->m_bIsDisposed = false;
            }

            MockStream::~MockStream() {
                delete m_pImpl;
            }

            void MockStream::SetThrowOnRead(bool bThrow) { m_pImpl->m_bThrowOnRead = bThrow; }
            void MockStream::SetThrowOnWrite(bool bThrow) { m_pImpl->m_bThrowOnWrite = bThrow; }
            void MockStream::SetCanRead(bool bCanRead) { m_pImpl->m_bCanRead = bCanRead; }
            void MockStream::SetCanWrite(bool bCanWrite) { m_pImpl->m_bCanWrite = bCanWrite; }
            void MockStream::SetCanSeek(bool bCanSeek) { m_pImpl->m_bCanSeek = bCanSeek; }
            bool MockStream::IsDisposed() const { return m_pImpl->m_bIsDisposed; }

            bool MockStream::CanRead() const { return m_pImpl->m_bCanRead && !m_pImpl->m_bIsDisposed; }
            bool MockStream::CanSeek() const { return m_pImpl->m_bCanSeek && !m_pImpl->m_bIsDisposed; }
            bool MockStream::CanWrite() const { return m_pImpl->m_bCanWrite && !m_pImpl->m_bIsDisposed; }

            long MockStream::GetLength() const {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                return static_cast<long>(m_pImpl->m_buffer.size());
            }

            long MockStream::GetPosition() const {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                return m_pImpl->m_lPosition;
            }

            void MockStream::SetPosition(long lValue) {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek) throw IOException("Stream does not support seeking.");
                m_pImpl->m_lPosition = lValue;
            }

            void MockStream::Flush() {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
            }

            int MockStream::Read(char* pBuffer, int iOffset, int iCount) {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanRead) throw IOException("Stream does not support reading.");
                if (m_pImpl->m_bThrowOnRead) throw IOException("Simulated read fault.");
                if (!pBuffer || iOffset < 0 || iCount < 0) return 0;

                long lAvailable = static_cast<long>(m_pImpl->m_buffer.size()) - m_pImpl->m_lPosition;
                if (lAvailable <= 0) return 0;

                int iToRead = static_cast<int>(lAvailable < iCount ? lAvailable : iCount);
                for (int i = 0; i < iToRead; ++i) {
                    pBuffer[iOffset + i] = m_pImpl->m_buffer[m_pImpl->m_lPosition + i];
                }
                m_pImpl->m_lPosition += iToRead;
                return iToRead;
            }

            long MockStream::Seek(long lOffset, int iOrigin) {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek) throw IOException("Stream does not support seeking.");

                long lNewPos = m_pImpl->m_lPosition;
                if (iOrigin == 0) lNewPos = lOffset;
                else if (iOrigin == 1) lNewPos += lOffset;
                else if (iOrigin == 2) lNewPos = static_cast<long>(m_pImpl->m_buffer.size()) + lOffset;

                if (lNewPos < 0) lNewPos = 0;
                m_pImpl->m_lPosition = lNewPos;
                return m_pImpl->m_lPosition;
            }

            void MockStream::SetLength(long lValue) {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanSeek || !m_pImpl->m_bCanWrite) throw IOException("Stream does not support seeking or writing.");
                m_pImpl->m_buffer.resize(static_cast<size_t>(lValue), 0);
            }

            void MockStream::Write(const char* pBuffer, int iOffset, int iCount) {
                if (m_pImpl->m_bIsDisposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_bCanWrite) throw IOException("Stream does not support writing.");
                if (m_pImpl->m_bThrowOnWrite) throw IOException("Simulated write fault.");
                if (!pBuffer || iOffset < 0 || iCount <= 0) return;

                size_t uRequiredSize = static_cast<size_t>(m_pImpl->m_lPosition + iCount);
                if (m_pImpl->m_buffer.size() < uRequiredSize) {
                    m_pImpl->m_buffer.resize(uRequiredSize, 0);
                }

                for (int i = 0; i < iCount; ++i) {
                    m_pImpl->m_buffer[m_pImpl->m_lPosition + i] = pBuffer[iOffset + i];
                }
                m_pImpl->m_lPosition += iCount;
            }

            void MockStream::Dispose() {
                m_pImpl->m_bIsDisposed = true;
            }

        }
    }
}

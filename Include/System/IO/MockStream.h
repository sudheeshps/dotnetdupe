#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/IOException.h"
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            class MockStream : public Stream {
            private:
                std::vector<char> m_buffer;
                long m_lPosition;
                bool m_bCanRead;
                bool m_bCanWrite;
                bool m_bCanSeek;
                bool m_bThrowOnRead;
                bool m_bThrowOnWrite;
                bool m_bIsDisposed;

            public:
                MockStream(bool bCanRead = true, bool bCanWrite = true, bool bCanSeek = true)
                    : m_lPosition(0),
                      m_bCanRead(bCanRead),
                      m_bCanWrite(bCanWrite),
                      m_bCanSeek(bCanSeek),
                      m_bThrowOnRead(false),
                      m_bThrowOnWrite(false),
                      m_bIsDisposed(false) {}

                virtual ~MockStream() override = default;

                void SetThrowOnRead(bool bThrow) { m_bThrowOnRead = bThrow; }
                void SetThrowOnWrite(bool bThrow) { m_bThrowOnWrite = bThrow; }
                void SetCanRead(bool bCanRead) { m_bCanRead = bCanRead; }
                void SetCanWrite(bool bCanWrite) { m_bCanWrite = bCanWrite; }
                void SetCanSeek(bool bCanSeek) { m_bCanSeek = bCanSeek; }
                bool IsDisposed() const { return m_bIsDisposed; }

                bool CanRead() const override { return m_bCanRead && !m_bIsDisposed; }
                bool CanSeek() const override { return m_bCanSeek && !m_bIsDisposed; }
                bool CanWrite() const override { return m_bCanWrite && !m_bIsDisposed; }

                long GetLength() const override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    return static_cast<long>(m_buffer.size());
                }

                long GetPosition() const override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    return m_lPosition;
                }

                void SetPosition(long lValue) override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    if (!m_bCanSeek) throw IOException("Stream does not support seeking.");
                    m_lPosition = lValue;
                }

                void Flush() override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                }

                int Read(char* pBuffer, int iOffset, int iCount) override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    if (!m_bCanRead) throw IOException("Stream does not support reading.");
                    if (m_bThrowOnRead) throw IOException("Simulated read fault.");
                    if (!pBuffer || iOffset < 0 || iCount < 0) return 0;

                    long lAvailable = static_cast<long>(m_buffer.size()) - m_lPosition;
                    if (lAvailable <= 0) return 0;

                    int iToRead = static_cast<int>(lAvailable < iCount ? lAvailable : iCount);
                    for (int i = 0; i < iToRead; ++i) {
                        pBuffer[iOffset + i] = m_buffer[m_lPosition + i];
                    }
                    m_lPosition += iToRead;
                    return iToRead;
                }

                long Seek(long lOffset, int iOrigin) override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    if (!m_bCanSeek) throw IOException("Stream does not support seeking.");

                    long lNewPos = m_lPosition;
                    if (iOrigin == 0) lNewPos = lOffset;
                    else if (iOrigin == 1) lNewPos += lOffset;
                    else if (iOrigin == 2) lNewPos = static_cast<long>(m_buffer.size()) + lOffset;

                    if (lNewPos < 0) lNewPos = 0;
                    m_lPosition = lNewPos;
                    return m_lPosition;
                }

                void SetLength(long lValue) override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    if (!m_bCanSeek || !m_bCanWrite) throw IOException("Stream does not support seeking or writing.");
                    m_buffer.resize(static_cast<size_t>(lValue), 0);
                }

                void Write(const char* pBuffer, int iOffset, int iCount) override {
                    if (m_bIsDisposed) throw IOException("Stream is closed.");
                    if (!m_bCanWrite) throw IOException("Stream does not support writing.");
                    if (m_bThrowOnWrite) throw IOException("Simulated write fault.");
                    if (!pBuffer || iOffset < 0 || iCount <= 0) return;

                    size_t uRequiredSize = static_cast<size_t>(m_lPosition + iCount);
                    if (m_buffer.size() < uRequiredSize) {
                        m_buffer.resize(uRequiredSize, 0);
                    }

                    for (int i = 0; i < iCount; ++i) {
                        m_buffer[m_lPosition + i] = pBuffer[iOffset + i];
                    }
                    m_lPosition += iCount;
                }

                void Dispose() override {
                    m_bIsDisposed = true;
                }
            };

        }
    }
}

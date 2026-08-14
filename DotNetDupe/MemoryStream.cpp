#include "pch.h"
#include "System/IO/MemoryStream.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/IOException.h"
#include <vector>
#include <algorithm>
#include <cstring>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            struct MemoryStream::MemoryStreamImpl {
                std::vector<char> m_buffer;
                long m_position;
                bool m_writable;
                bool m_disposed;
                
                MemoryStreamImpl() : m_position(0), m_writable(true), m_disposed(false) {}
                MemoryStreamImpl(bool writable) : m_position(0), m_writable(writable), m_disposed(false) {}
            };

            MemoryStream::MemoryStream() : m_pImpl(new MemoryStreamImpl()) {}

            MemoryStream::MemoryStream(const Array<char>& buffer) : m_pImpl(new MemoryStreamImpl()) {
                m_pImpl->m_buffer.assign(buffer.GetData(), buffer.GetData() + buffer.GetLength());
            }

            MemoryStream::MemoryStream(const Array<char>& buffer, bool writable) : m_pImpl(new MemoryStreamImpl(writable)) {
                m_pImpl->m_buffer.assign(buffer.GetData(), buffer.GetData() + buffer.GetLength());
            }

            MemoryStream::~MemoryStream() {
                Dispose();
                delete m_pImpl;
            }

            bool MemoryStream::CanRead() const {
                return !m_pImpl->m_disposed;
            }

            bool MemoryStream::CanSeek() const {
                return !m_pImpl->m_disposed;
            }

            bool MemoryStream::CanWrite() const {
                return m_pImpl->m_writable && !m_pImpl->m_disposed;
            }

            long MemoryStream::GetLength() const {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                return static_cast<long>(m_pImpl->m_buffer.size());
            }

            long MemoryStream::GetPosition() const {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                return m_pImpl->m_position;
            }

            void MemoryStream::SetPosition(long value) {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                if (value < 0) throw ArgumentOutOfRangeException("position");
                m_pImpl->m_position = value;
            }

            void MemoryStream::Flush() {
                // No-op
            }

            int MemoryStream::Read(char* buffer, int offset, int count) {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                if (buffer == nullptr) throw ArgumentNullException("buffer");
                if (offset < 0 || count < 0) throw ArgumentOutOfRangeException("offset or count");

                long remaining = GetLength() - m_pImpl->m_position;
                if (remaining <= 0) return 0;

                int toRead = static_cast<int>((std::min)(static_cast<long>(count), remaining));
                std::memcpy(buffer + offset, m_pImpl->m_buffer.data() + m_pImpl->m_position, toRead);
                m_pImpl->m_position += toRead;
                return toRead;
            }

            static long CalculateSeekPosition(long currentPos, long length, long offset, int origin) {
                if (origin == 0) return offset;
                if (origin == 1) return currentPos + offset;
                if (origin == 2) return length + offset;
                throw ArgumentException("Invalid seek origin.");
            }

            long MemoryStream::Seek(long offset, int origin) {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                long newPosition = CalculateSeekPosition(m_pImpl->m_position, GetLength(), offset, origin);
                if (newPosition < 0) throw IOException("An attempt was made to move the file pointer before the beginning of the file.");
                m_pImpl->m_position = newPosition;
                return m_pImpl->m_position;
            }

            void MemoryStream::SetLength(long value) {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_writable) throw IOException("Stream is not writable.");
                if (value < 0) throw ArgumentOutOfRangeException("value");

                m_pImpl->m_buffer.resize(static_cast<size_t>(value), 0);
                if (m_pImpl->m_position > value) {
                    m_pImpl->m_position = value;
                }
            }

            void MemoryStream::Write(const char* buffer, int offset, int count) {
                if (m_pImpl->m_disposed) throw IOException("Stream is closed.");
                if (!m_pImpl->m_writable) throw IOException("Stream is not writable.");
                if (buffer == nullptr) throw ArgumentNullException("buffer");
                if (offset < 0 || count < 0) throw ArgumentOutOfRangeException("offset or count");

                long requiredSize = m_pImpl->m_position + count;
                if (requiredSize > GetLength()) {
                    m_pImpl->m_buffer.resize(static_cast<size_t>(requiredSize));
                }

                std::memcpy(m_pImpl->m_buffer.data() + m_pImpl->m_position, buffer + offset, count);
                m_pImpl->m_position += count;
            }

            void MemoryStream::Dispose() {
                m_pImpl->m_disposed = true;
            }

            Array<char> MemoryStream::ToArray() const {
                int size = static_cast<int>(m_pImpl->m_buffer.size());
                Array<char> arr(size);
                if (size > 0) {
                    std::memcpy(arr.GetData(), m_pImpl->m_buffer.data(), size);
                }
                return arr;
            }
        }
    }
}

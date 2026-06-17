#include "pch.h"
#include "System/IO/MemoryStream.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/IOException.h"
#include <algorithm>
#include <cstring>

namespace DotNetDupe {
    namespace System {
        namespace IO {

            MemoryStream::MemoryStream()
                : m_position(0), m_writable(true), m_disposed(false) {}

            MemoryStream::MemoryStream(const Array<char>& buffer)
                : m_position(0), m_writable(true), m_disposed(false) {
                m_buffer.assign(buffer.GetData(), buffer.GetData() + buffer.GetLength());
            }

            MemoryStream::MemoryStream(const Array<char>& buffer, bool writable)
                : m_position(0), m_writable(writable), m_disposed(false) {
                m_buffer.assign(buffer.GetData(), buffer.GetData() + buffer.GetLength());
            }

            MemoryStream::~MemoryStream() {
                Dispose();
            }

            bool MemoryStream::CanRead() const {
                return !m_disposed;
            }

            bool MemoryStream::CanSeek() const {
                return !m_disposed;
            }

            bool MemoryStream::CanWrite() const {
                return m_writable && !m_disposed;
            }

            long MemoryStream::GetLength() const {
                if (m_disposed) throw IOException("Stream is closed.");
                return static_cast<long>(m_buffer.size());
            }

            long MemoryStream::GetPosition() const {
                if (m_disposed) throw IOException("Stream is closed.");
                return m_position;
            }

            void MemoryStream::SetPosition(long value) {
                if (m_disposed) throw IOException("Stream is closed.");
                if (value < 0) throw ArgumentOutOfRangeException("position");
                m_position = value;
            }

            void MemoryStream::Flush() {
                // No-op
            }

            int MemoryStream::Read(char* buffer, int offset, int count) {
                if (m_disposed) throw IOException("Stream is closed.");
                if (buffer == nullptr) throw ArgumentNullException("buffer");
                if (offset < 0 || count < 0) throw ArgumentOutOfRangeException("offset or count");

                long remaining = GetLength() - m_position;
                if (remaining <= 0) return 0;

                int toRead = static_cast<int>((std::min)(static_cast<long>(count), remaining));
                std::memcpy(buffer + offset, m_buffer.data() + m_position, toRead);
                m_position += toRead;
                return toRead;
            }

            long MemoryStream::Seek(long offset, int origin) {
                if (m_disposed) throw IOException("Stream is closed.");

                long newPosition = m_position;
                switch (origin) {
                    case 0: // Begin
                        newPosition = offset;
                        break;
                    case 1: // Current
                        newPosition += offset;
                        break;
                    case 2: // End
                        newPosition = GetLength() + offset;
                        break;
                    default:
                        throw ArgumentException("Invalid seek origin.");
                }

                if (newPosition < 0) {
                    throw IOException("An attempt was made to move the file pointer before the beginning of the file.");
                }

                m_position = newPosition;
                return m_position;
            }

            void MemoryStream::SetLength(long value) {
                if (m_disposed) throw IOException("Stream is closed.");
                if (!m_writable) throw IOException("Stream is not writable.");
                if (value < 0) throw ArgumentOutOfRangeException("value");

                m_buffer.resize(static_cast<size_t>(value), 0);
                if (m_position > value) {
                    m_position = value;
                }
            }

            void MemoryStream::Write(const char* buffer, int offset, int count) {
                if (m_disposed) throw IOException("Stream is closed.");
                if (!m_writable) throw IOException("Stream is not writable.");
                if (buffer == nullptr) throw ArgumentNullException("buffer");
                if (offset < 0 || count < 0) throw ArgumentOutOfRangeException("offset or count");

                long requiredSize = m_position + count;
                if (requiredSize > GetLength()) {
                    m_buffer.resize(static_cast<size_t>(requiredSize));
                }

                std::memcpy(m_buffer.data() + m_position, buffer + offset, count);
                m_position += count;
            }

            void MemoryStream::Dispose() {
                m_disposed = true;
            }

            Array<char> MemoryStream::ToArray() const {
                int size = static_cast<int>(m_buffer.size());
                Array<char> arr(size);
                if (size > 0) {
                    std::memcpy(arr.GetData(), m_buffer.data(), size);
                }
                return arr;
            }
        }
    }
}

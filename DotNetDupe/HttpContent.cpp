#include "pch.h"
#include "System/Net/Http/HttpContent.h"
#include "System/IO/MemoryStream.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include <cstring>
#include <vector>

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                void HttpContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    /// Guard: Ensure target destination stream is valid.
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    /// Read entity body as byte array and copy to output stream.
                    Array<char> data = ReadAsByteArray();
                    if (data.GetLength() > 0) {
                        stream->Write(data.GetData(), 0, data.GetLength());
                    }
                }

                long HttpContent::GetLength() const {
                    /// Default length is indeterminate (-1).
                    return -1;
                }

                // --- StringContent ---

                StringContent::StringContent(const String& content)
                    : m_sContent(content) {
                    /// Initialize with default UTF-8 plain text Content-Type header.
                    GetHeaders().Add("Content-Type", "text/plain; charset=utf-8");
                }

                StringContent::StringContent(const String& content, const String& mediaType)
                    : m_sContent(content) {
                    /// Normalize content-type header with charset suffix.
                    String contentType = mediaType;
                    if (!contentType.Contains("charset")) {
                        contentType = contentType + "; charset=utf-8";
                    }
                    GetHeaders().Add("Content-Type", contentType);
                }

                String StringContent::ReadAsString() {
                    /// Return underlying string payload.
                    return m_sContent;
                }

                Array<char> StringContent::ReadAsByteArray() {
                    /// Convert UTF-8 characters to byte array.
                    int len = m_sContent.GetLength();
                    if (len <= 0) return Array<char>(0);
                    return Array<char>(m_sContent.GetRawString(), len);
                }

                SmartPointer<IO::Stream> StringContent::ReadAsStream() {
                    /// Wrap byte array in MemoryStream.
                    return SmartPointer<IO::Stream>(new IO::MemoryStream(ReadAsByteArray()), true);
                }

                void StringContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    /// Guard: Validate destination stream.
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    /// Write raw UTF-8 string bytes into target stream.
                    int len = m_sContent.GetLength();
                    if (len > 0) {
                        stream->Write(m_sContent.GetRawString(), 0, len);
                    }
                }

                long StringContent::GetLength() const {
                    /// Return string byte length.
                    return m_sContent.GetLength();
                }

                // --- ByteArrayContent ---

                ByteArrayContent::ByteArrayContent(const Array<char>& content)
                    : m_arrContent(content), m_iOffset(0), m_iCount(content.GetLength()) {
                    /// Initialize with default octet-stream Content-Type header.
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                ByteArrayContent::ByteArrayContent(const Array<char>& content, int offset, int count)
                    : m_arrContent(content), m_iOffset(offset), m_iCount(count) {
                    /// Guard: Validate buffer slice boundaries.
                    if (offset < 0 || count < 0 || offset + count > content.GetLength()) {
                        throw ArgumentOutOfRangeException("Offset or count is out of range.");
                    }
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                String ByteArrayContent::ReadAsString() {
                    /// Decode buffer slice into String.
                    String s;
                    if (m_iCount > 0) {
                        std::string temp(m_arrContent.GetData() + m_iOffset, m_iCount);
                        s = String(temp.c_str());
                    }
                    return s;
                }

                Array<char> ByteArrayContent::ReadAsByteArray() {
                    /// Return direct buffer or copy slice.
                    if (m_iOffset == 0 && m_iCount == m_arrContent.GetLength()) {
                        return m_arrContent;
                    }
                    Array<char> arr(m_iCount);
                    if (m_iCount > 0) {
                        std::memcpy(arr.GetData(), m_arrContent.GetData() + m_iOffset, m_iCount);
                    }
                    return arr;
                }

                SmartPointer<IO::Stream> ByteArrayContent::ReadAsStream() {
                    /// Return MemoryStream wrapping byte buffer slice.
                    return SmartPointer<IO::Stream>(new IO::MemoryStream(ReadAsByteArray()), true);
                }

                void ByteArrayContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    /// Guard: Validate destination stream.
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    /// Stream byte slice into destination.
                    if (m_iCount > 0) {
                        stream->Write(m_arrContent.GetData(), m_iOffset, m_iCount);
                    }
                }

                long ByteArrayContent::GetLength() const {
                    /// Return slice count.
                    return m_iCount;
                }

                // --- StreamContent ---

                StreamContent::StreamContent(const SmartPointer<IO::Stream>& stream)
                    : m_pStream(stream) {
                    /// Guard: Ensure source stream is non-null.
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                String StreamContent::ReadAsString() {
                    /// Read byte array and convert to string.
                    Array<char> bytes = ReadAsByteArray();
                    String s;
                    if (bytes.GetLength() > 0) {
                        std::string temp(bytes.GetData(), bytes.GetLength());
                        s = String(temp.c_str());
                    }
                    return s;
                }

                Array<char> StreamContent::ReadAsByteArray() {
                    /// Guard: Check null stream.
                    if (m_pStream.IsNull()) return Array<char>(0);
                    /// Buffer read all available bytes from source stream.
                    std::vector<char> vecData;
                    char buf[4096];
                    int iRead = 0;
                    while ((iRead = m_pStream->Read(buf, 0, sizeof(buf))) > 0) {
                        vecData.insert(vecData.end(), buf, buf + iRead);
                    }
                    Array<char> arr(static_cast<int>(vecData.size()));
                    if (!vecData.empty()) {
                        std::memcpy(arr.GetData(), vecData.data(), vecData.size());
                    }
                    return arr;
                }

                SmartPointer<IO::Stream> StreamContent::ReadAsStream() {
                    /// Return encapsulated stream.
                    return m_pStream;
                }

                void StreamContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    /// Guard: Ensure target stream is valid.
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    if (m_pStream.IsNull()) return;
                    /// Pump buffer chunks from source to target.
                    char buf[4096];
                    int iRead = 0;
                    while ((iRead = m_pStream->Read(buf, 0, sizeof(buf))) > 0) {
                        stream->Write(buf, 0, iRead);
                    }
                }

                long StreamContent::GetLength() const {
                    /// Query length if stream supports seeking.
                    if (!m_pStream.IsNull() && m_pStream->CanSeek()) {
                        try {
                            return m_pStream->GetLength();
                        } catch (const Exception&) {
                            /// Return -1 on stream query failure.
                            return -1;
                        }
                    }
                    return -1;
                }

            }
        }
    }
}

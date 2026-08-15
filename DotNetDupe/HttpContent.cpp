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
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    Array<char> data = ReadAsByteArray();
                    if (data.GetLength() > 0) {
                        stream->Write(data.GetData(), 0, data.GetLength());
                    }
                }

                long HttpContent::GetLength() const {
                    return -1;
                }

                // --- StringContent ---

                StringContent::StringContent(const String& content)
                    : m_sContent(content) {
                    GetHeaders().Add("Content-Type", "text/plain; charset=utf-8");
                }

                StringContent::StringContent(const String& content, const String& mediaType)
                    : m_sContent(content) {
                    String contentType = mediaType;
                    if (!contentType.Contains("charset")) {
                        contentType = contentType + "; charset=utf-8";
                    }
                    GetHeaders().Add("Content-Type", contentType);
                }

                String StringContent::ReadAsString() {
                    return m_sContent;
                }

                Array<char> StringContent::ReadAsByteArray() {
                    int len = m_sContent.GetLength();
                    if (len <= 0) return Array<char>(0);
                    return Array<char>(m_sContent.GetRawString(), len);
                }

                SmartPointer<IO::Stream> StringContent::ReadAsStream() {
                    return SmartPointer<IO::Stream>(new IO::MemoryStream(ReadAsByteArray()), true);
                }

                void StringContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    int len = m_sContent.GetLength();
                    if (len > 0) {
                        stream->Write(m_sContent.GetRawString(), 0, len);
                    }
                }

                long StringContent::GetLength() const {
                    return m_sContent.GetLength();
                }

                // --- ByteArrayContent ---

                ByteArrayContent::ByteArrayContent(const Array<char>& content)
                    : m_arrContent(content), m_iOffset(0), m_iCount(content.GetLength()) {
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                ByteArrayContent::ByteArrayContent(const Array<char>& content, int offset, int count)
                    : m_arrContent(content), m_iOffset(offset), m_iCount(count) {
                    if (offset < 0 || count < 0 || offset + count > content.GetLength()) {
                        throw ArgumentOutOfRangeException("Offset or count is out of range.");
                    }
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                String ByteArrayContent::ReadAsString() {
                    String s;
                    if (m_iCount > 0) {
                        std::string temp(m_arrContent.GetData() + m_iOffset, m_iCount);
                        s = String(temp.c_str());
                    }
                    return s;
                }

                Array<char> ByteArrayContent::ReadAsByteArray() {
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
                    return SmartPointer<IO::Stream>(new IO::MemoryStream(ReadAsByteArray()), true);
                }

                void ByteArrayContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    if (m_iCount > 0) {
                        stream->Write(m_arrContent.GetData(), m_iOffset, m_iCount);
                    }
                }

                long ByteArrayContent::GetLength() const {
                    return m_iCount;
                }

                // --- StreamContent ---

                StreamContent::StreamContent(const SmartPointer<IO::Stream>& stream)
                    : m_pStream(stream) {
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    GetHeaders().Add("Content-Type", "application/octet-stream");
                }

                String StreamContent::ReadAsString() {
                    Array<char> bytes = ReadAsByteArray();
                    String s;
                    if (bytes.GetLength() > 0) {
                        std::string temp(bytes.GetData(), bytes.GetLength());
                        s = String(temp.c_str());
                    }
                    return s;
                }

                Array<char> StreamContent::ReadAsByteArray() {
                    if (m_pStream.IsNull()) return Array<char>(0);
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
                    return m_pStream;
                }

                void StreamContent::CopyTo(const SmartPointer<IO::Stream>& stream) {
                    if (stream.IsNull()) {
                        throw ArgumentNullException("stream");
                    }
                    if (m_pStream.IsNull()) return;
                    char buf[4096];
                    int iRead = 0;
                    while ((iRead = m_pStream->Read(buf, 0, sizeof(buf))) > 0) {
                        stream->Write(buf, 0, iRead);
                    }
                }

                long StreamContent::GetLength() const {
                    if (!m_pStream.IsNull() && m_pStream->CanSeek()) {
                        try {
                            return m_pStream->GetLength();
                        } catch (const Exception&) {
                            return -1;
                        } catch (const std::exception&) {
                            return -1;
                        }
                    }
                    return -1;
                }

            }
        }
    }
}

/// \file HttpContent.h
/// \brief Represents an HTTP entity body and associated content headers.

#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/Array.h"
#include "System/SmartPointer.h"
#include "System/IO/Stream.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Http {

                /// \class HttpContent
                /// \brief A base class representing an HTTP entity body and content headers.
                ///
                /// Provides polymorphic stream, byte array, and string reading abstractions.
                /// Standards Conformance: RFC 9110 (HTTP Semantics), RFC 9112.
                class HttpContent : public Object {
                public:
                    /// \brief Virtual destructor for polymorphic cleanup.
                    DOTNETDUPE_API virtual ~HttpContent() = default;

                    /// \brief Serializes the HTTP content to a string as an operation.
                    /// \return The content string representation.
                    DOTNETDUPE_API virtual String ReadAsString() = 0;

                    /// \brief Serializes the HTTP content to a byte array.
                    /// \return The content as an Array<char>.
                    DOTNETDUPE_API virtual Array<char> ReadAsByteArray() = 0;

                    /// \brief Serializes the HTTP content and returns a stream that represents the content.
                    /// \return A SmartPointer to an IO::Stream reading the content.
                    DOTNETDUPE_API virtual SmartPointer<IO::Stream> ReadAsStream() = 0;

                    /// \brief Serializes the HTTP content to a stream.
                    /// \param stream The target stream to copy the content to.
                    /// \throws ArgumentNullException When stream is null.
                    DOTNETDUPE_API virtual void CopyTo(const SmartPointer<IO::Stream>& stream);

                    /// \brief Gets the length in bytes of the HHTP content, or -1 if indeterminate.
                    /// \return The byte length or -1.
                    DOTNETDUPE_API virtual long GetLength() const;

                    /// \brief Gets the HTTP content headers defined in RFC 9110.
                    /// \return Mutable reference to headers Dictionary.
                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders() { return m_headers; }

                    /// \brief Gets the HTTP content headers as read-only.
                    /// \return Const reference to headers Dictionary.
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const { return m_headers; }

                protected:
                    /// \brief Protected default constructor for derived content types.
                    DOTNETDUPE_API HttpContent() = default;

                private:
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

                /// \brief Type alias for reference-counted SmartPointer to HttpContent.
                using HttpContentPtr = SmartPointer<HttpContent>;

                /// \class StringContent
                /// \brief Provides HTTP content based on a string with charset and media type.
                class StringContent : public HttpContent {
                public:
                    /// \brief Creates a new instance of StringContent with text/plain default media type.
                    /// \param content The string used to initialize the StringContent.
                    DOTNETDUPE_API explicit StringContent(const String& content);

                    /// \brief Creates a new instance of StringContent with specified media type.
                    /// \param content The string used to initialize the StringContent.
                    /// \param mediaType The media type for the content (e.g. "application/json").
                    DOTNETDUPE_API StringContent(const String& content, const String& mediaType);

                    /// \brief Reads the content payload as a string.
                    /// \return The underlying content string.
                    DOTNETDUPE_API String ReadAsString() override;

                    /// \brief Reads the content payload as a byte array.
                    /// \return Array of bytes representing the UTF-8 content.
                    DOTNETDUPE_API Array<char> ReadAsByteArray() override;

                    /// \brief Reads the content payload as a readable MemoryStream.
                    /// \return SmartPointer to MemoryStream wrapping the content.
                    DOTNETDUPE_API SmartPointer<IO::Stream> ReadAsStream() override;

                    /// \brief Writes the content string directly to the destination stream.
                    /// \param stream The target stream to copy into.
                    DOTNETDUPE_API void CopyTo(const SmartPointer<IO::Stream>& stream) override;

                    /// \brief Gets the length in bytes of the UTF-8 content string.
                    /// \return Number of bytes in content.
                    DOTNETDUPE_API long GetLength() const override;

                private:
                    String m_sContent;
                };

                /// \class ByteArrayContent
                /// \brief Provides HTTP content based on a byte array buffer.
                class ByteArrayContent : public HttpContent {
                public:
                    /// \brief Initializes a new instance of ByteArrayContent with full byte buffer.
                    /// \param content The byte buffer used to initialize the ByteArrayContent.
                    DOTNETDUPE_API explicit ByteArrayContent(const Array<char>& content);

                    /// \brief Initializes a new instance of ByteArrayContent with bounded range.
                    /// \param content The byte buffer.
                    /// \param offset The starting byte offset.
                    /// \param count The number of bytes to include.
                    DOTNETDUPE_API ByteArrayContent(const Array<char>& content, int offset, int count);

                    /// \brief Reads the byte buffer decoded as a string.
                    /// \return String representation of the bytes.
                    DOTNETDUPE_API String ReadAsString() override;

                    /// \brief Reads the content byte buffer.
                    /// \return Sub-array of bytes.
                    DOTNETDUPE_API Array<char> ReadAsByteArray() override;

                    /// \brief Reads the content as a MemoryStream.
                    /// \return SmartPointer to Stream.
                    DOTNETDUPE_API SmartPointer<IO::Stream> ReadAsStream() override;

                    /// \brief Copies the byte buffer into the target stream.
                    /// \param stream The target stream.
                    DOTNETDUPE_API void CopyTo(const SmartPointer<IO::Stream>& stream) override;

                    /// \brief Returns the byte count of the buffer slice.
                    /// \return Number of bytes.
                    DOTNETDUPE_API long GetLength() const override;

                private:
                    Array<char> m_arrContent;
                    int m_iOffset;
                    int m_iCount;
                };

                /// \class StreamContent
                /// \brief Provides HTTP content based on an underlying stream.
                class StreamContent : public HttpContent {
                public:
                    /// \brief Initializes a new instance of StreamContent wrapping an existing Stream.
                    /// \param stream The stream used to initialize the StreamContent.
                    DOTNETDUPE_API explicit StreamContent(const SmartPointer<IO::Stream>& stream);

                    /// \brief Reads the remaining bytes in the stream into a string.
                    /// \return Decoded string.
                    DOTNETDUPE_API String ReadAsString() override;

                    /// \brief Reads the remaining bytes in the stream into a byte array.
                    /// \return Array of raw bytes.
                    DOTNETDUPE_API Array<char> ReadAsByteArray() override;

                    /// \brief Returns the underlying stream.
                    /// \return SmartPointer to Stream.
                    DOTNETDUPE_API SmartPointer<IO::Stream> ReadAsStream() override;

                    /// \brief Copies the underlying stream into the destination stream.
                    /// \param stream The destination stream.
                    DOTNETDUPE_API void CopyTo(const SmartPointer<IO::Stream>& stream) override;

                    /// \brief Gets the length of the underlying stream if seekable, or -1.
                    /// \return Length in bytes or -1.
                    DOTNETDUPE_API long GetLength() const override;

                private:
                    SmartPointer<IO::Stream> m_pStream;
                };

            }
        }
    }
}

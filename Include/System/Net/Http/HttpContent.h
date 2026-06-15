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

                class HttpContent : public Object {
                public:
                    DOTNETDUPE_API virtual ~HttpContent() = default;

                    DOTNETDUPE_API virtual String ReadAsString() = 0;
                    DOTNETDUPE_API virtual Array<char> ReadAsByteArray() = 0;
                    DOTNETDUPE_API virtual SmartPointer<IO::Stream> ReadAsStream() = 0;

                    DOTNETDUPE_API virtual void CopyTo(const SmartPointer<IO::Stream>& stream);
                    DOTNETDUPE_API virtual long GetLength() const;

                    DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeaders() { return m_headers; }
                    DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeaders() const { return m_headers; }

                protected:
                    DOTNETDUPE_API HttpContent() = default;

                private:
                    Collections::Generic::Dictionary<String, String> m_headers;
                };

                class StringContent : public HttpContent {
                public:
                    DOTNETDUPE_API explicit StringContent(const String& content);
                    DOTNETDUPE_API StringContent(const String& content, const String& mediaType);

                    DOTNETDUPE_API String ReadAsString() override;
                    DOTNETDUPE_API Array<char> ReadAsByteArray() override;
                    DOTNETDUPE_API SmartPointer<IO::Stream> ReadAsStream() override;

                    DOTNETDUPE_API void CopyTo(const SmartPointer<IO::Stream>& stream) override;
                    DOTNETDUPE_API long GetLength() const override;

                private:
                    String m_sContent;
                };

                class ByteArrayContent : public HttpContent {
                public:
                    DOTNETDUPE_API explicit ByteArrayContent(const Array<char>& content);
                    DOTNETDUPE_API ByteArrayContent(const Array<char>& content, int offset, int count);

                    DOTNETDUPE_API String ReadAsString() override;
                    DOTNETDUPE_API Array<char> ReadAsByteArray() override;
                    DOTNETDUPE_API SmartPointer<IO::Stream> ReadAsStream() override;

                    DOTNETDUPE_API void CopyTo(const SmartPointer<IO::Stream>& stream) override;
                    DOTNETDUPE_API long GetLength() const override;

                private:
                    Array<char> m_arrContent;
                    int m_iOffset;
                    int m_iCount;
                };

            }
        }
    }
}

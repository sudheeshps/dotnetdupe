#pragma once

#include "Common.h"
#include "System/IO/Stream.h"
#include "System/SmartPointer.h"
#include "System/String.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Security {

                class SslStream : public IO::Stream {
                public:
                    DOTNETDUPE_API SslStream(const SmartPointer<IO::Stream>& innerStream);
                    DOTNETDUPE_API SslStream(const SmartPointer<IO::Stream>& innerStream, bool leaveInnerStreamOpen);
                    DOTNETDUPE_API ~SslStream() override;

                    DOTNETDUPE_API void AuthenticateAsClient(const String& targetHost);
                    DOTNETDUPE_API void AuthenticateAsServer(const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate);

                    DOTNETDUPE_API bool CanRead() const override;
                    DOTNETDUPE_API bool CanSeek() const override;
                    DOTNETDUPE_API bool CanWrite() const override;
                    DOTNETDUPE_API long GetLength() const override;
                    DOTNETDUPE_API long GetPosition() const override;
                    DOTNETDUPE_API void SetPosition(long value) override;

                    DOTNETDUPE_API void Flush() override;
                    DOTNETDUPE_API int Read(char* buffer, int offset, int count) override;
                    DOTNETDUPE_API long Seek(long offset, int origin) override;
                    DOTNETDUPE_API void SetLength(long value) override;
                    DOTNETDUPE_API void Write(const char* buffer, int offset, int count) override;

                    DOTNETDUPE_API void Dispose() override;

                private:
                    SmartPointer<IO::Stream> m_spInnerStream;
                    bool m_bLeaveInnerStreamOpen;
                    bool m_bDisposed;
                    
                    void* m_pSslCtx; // SSL_CTX*
                    void* m_pSsl;    // SSL*
                    void* m_pBioIn;  // BIO*
                    void* m_pBioOut; // BIO*

                    void InitializeOpenSSL();
                    void ProcessHandshake();
                    void FlushOutboundBio();
                };

            }
        }
    }
}

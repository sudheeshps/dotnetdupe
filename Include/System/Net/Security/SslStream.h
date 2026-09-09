/// \file SslStream.h
/// \brief Provides a stream that uses the Transport Layer Security (TLS) protocol to secure network communications.

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

                /// \class SslStream
                /// \brief Provides a stream that uses the Transport Layer Security (TLS/SSL) protocol backed by OpenSSL.
                ///
                /// Supports client authentication, server certificate configuration, and encrypted network I/O.
                /// Standards Conformance: RFC 8446 (TLS 1.3), RFC 5246 (TLS 1.2), RFC 6066 (Server Name Indication - SNI).
                class SslStream : public IO::Stream {
                public:
                    /// \brief Initializes a new instance of the SslStream class using the specified inner stream.
                    /// \param innerStream The Stream object used by the SslStream for sending and receiving data.
                    /// \throws ArgumentNullException When innerStream is null.
                    DOTNETDUPE_API SslStream(const SmartPointer<IO::Stream>& innerStream);

                    /// \brief Initializes a new instance of the SslStream class with stream closure behavior option.
                    /// \param innerStream The Stream object used for encrypted transport.
                    /// \param leaveInnerStreamOpen true to leave inner stream open upon disposing SslStream; otherwise, false.
                    /// \throws ArgumentNullException When innerStream is null.
                    DOTNETDUPE_API SslStream(const SmartPointer<IO::Stream>& innerStream, bool leaveInnerStreamOpen);

                    /// \brief Releases all unmanaged resources and closes OpenSSL structures.
                    DOTNETDUPE_API ~SslStream() override;

                    /// \brief Called by clients to authenticate the server and optionally the client in a client-server connection.
                    /// \param targetHost The target host name used for SNI and certificate validation.
                    /// \throws IOException If the TLS handshake fails.
                    DOTNETDUPE_API void AuthenticateAsClient(const String& targetHost);

                    /// \brief Called by servers to authenticate the server and optionally the client in a client-server connection.
                    /// \param certificate The X509Certificate2 certificate used to authenticate the server.
                    /// \throws IOException If the TLS handshake fails.
                    DOTNETDUPE_API void AuthenticateAsServer(const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate);

                    /// \brief Gets a value indicating whether the current stream supports reading.
                    /// \return true if inner stream supports reading and SslStream is not disposed.
                    DOTNETDUPE_API bool CanRead() const override;

                    /// \brief Gets a value indicating whether the current stream supports seeking (always false for TLS).
                    /// \return false.
                    DOTNETDUPE_API bool CanSeek() const override;

                    /// \brief Gets a value indicating whether the current stream supports writing.
                    /// \return true if inner stream supports writing and SslStream is not disposed.
                    DOTNETDUPE_API bool CanWrite() const override;

                    /// \brief Gets the length of the data in the stream (unsupported for TLS stream).
                    /// \return -1.
                    DOTNETDUPE_API long GetLength() const override;

                    /// \brief Gets the position within the current stream (unsupported for TLS stream).
                    /// \return 0.
                    DOTNETDUPE_API long GetPosition() const override;

                    /// \brief Sets the position within the current stream (unsupported for TLS stream).
                    /// \param value Target position.
                    DOTNETDUPE_API void SetPosition(long value) override;

                    /// \brief Flushes data written to the stream to the underlying transport.
                    DOTNETDUPE_API void Flush() override;

                    /// \brief Reads data from this stream into the specified byte buffer.
                    /// \param buffer Pointer to destination buffer.
                    /// \param offset Starting byte offset in destination buffer.
                    /// \param count Maximum number of bytes to read.
                    /// \return Number of decrypted bytes read into the buffer.
                    DOTNETDUPE_API int Read(char* buffer, int offset, int count) override;

                    /// \brief Sets the current position of this stream to the given value (unsupported).
                    /// \param offset Point relative to origin.
                    /// \param origin Origin seek point.
                    /// \return 0.
                    DOTNETDUPE_API long Seek(long offset, int origin) override;

                    /// \brief Sets the length of this stream (unsupported for TLS stream).
                    /// \param value Length to set.
                    DOTNETDUPE_API void SetLength(long value) override;

                    /// \brief Encrypts and writes the specified number of bytes to the underlying stream.
                    /// \param buffer Pointer to source buffer.
                    /// \param offset Starting byte offset in buffer.
                    /// \param count Number of bytes to encrypt and write.
                    DOTNETDUPE_API void Write(const char* buffer, int offset, int count) override;

                    /// \brief Disposes TLS context, BIO buffers, and optionally inner transport.
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
                    void* CreateSslContext(bool isServer);
                    void ConfigureServerCert(void* ctx, const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate);
                };

            }
        }
    }
}

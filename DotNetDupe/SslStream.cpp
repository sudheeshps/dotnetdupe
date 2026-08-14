#include "pch.h"
#include "System/Net/Security/SslStream.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include <mutex>

#if defined(_WIN32)
    #include <openssl/ssl.h>
    #include <openssl/err.h>
    #include <openssl/bio.h>
#else
    #include <openssl/ssl.h>
    #include <openssl/err.h>
    #include <openssl/bio.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Security {

                static std::once_flag s_sslInitOnce;

                void SslStream::InitializeOpenSSL() {
                    std::call_once(s_sslInitOnce, []() {
                        SSL_library_init();
                        SSL_load_error_strings();
                        OpenSSL_add_all_algorithms();
                    });
                }

                SslStream::SslStream(const SmartPointer<IO::Stream>& innerStream)
                    : m_spInnerStream(innerStream),
                      m_bLeaveInnerStreamOpen(false),
                      m_bDisposed(false),
                      m_pSslCtx(nullptr),
                      m_pSsl(nullptr),
                      m_pBioIn(nullptr),
                      m_pBioOut(nullptr) {
                    if (innerStream.IsNull()) {
                        throw ArgumentNullException("innerStream cannot be null.");
                    }
                    InitializeOpenSSL();
                }

                SslStream::SslStream(const SmartPointer<IO::Stream>& innerStream, bool leaveInnerStreamOpen)
                    : m_spInnerStream(innerStream),
                      m_bLeaveInnerStreamOpen(leaveInnerStreamOpen),
                      m_bDisposed(false),
                      m_pSslCtx(nullptr),
                      m_pSsl(nullptr),
                      m_pBioIn(nullptr),
                      m_pBioOut(nullptr) {
                    if (innerStream.IsNull()) {
                        throw ArgumentNullException("innerStream cannot be null.");
                    }
                    InitializeOpenSSL();
                }

                SslStream::~SslStream() {
                    Dispose();
                }

                void* SslStream::CreateSslContext(bool isServer) {
                    SSL_CTX* ctx = SSL_CTX_new(isServer ? TLS_server_method() : TLS_client_method());
                    if (!ctx) {
                        throw IO::IOException("Failed to create SSL context.");
                    }

                    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
                    return ctx;
                }

                void SslStream::ConfigureServerCert(void* rawCtx, const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate) {
                    SSL_CTX* ctx = static_cast<SSL_CTX*>(rawCtx);
                    X509* cert = static_cast<X509*>(certificate->GetInternalCert());
                    EVP_PKEY* pkey = static_cast<EVP_PKEY*>(certificate->GetInternalKey());

                    if (SSL_CTX_use_certificate(ctx, cert) <= 0) {
                        SSL_CTX_free(ctx);
                        m_pSslCtx = nullptr;
                        throw ArgumentException("Failed to configure certificate in SSL context.");
                    }

                    if (SSL_CTX_use_PrivateKey(ctx, pkey) <= 0) {
                        SSL_CTX_free(ctx);
                        m_pSslCtx = nullptr;
                        throw ArgumentException("Failed to configure private key in SSL context.");
                    }
                }

                static SSL* CreateAndBindSsl(SSL_CTX* ctx, void*& pBioIn, void*& pBioOut) {
                    SSL* ssl = SSL_new(ctx);
                    if (!ssl) {
                        SSL_CTX_free(ctx);
                        throw IO::IOException("Failed to create SSL handle.");
                    }
                    BIO* bioIn = BIO_new(BIO_s_mem());
                    BIO* bioOut = BIO_new(BIO_s_mem());
                    pBioIn = bioIn;
                    pBioOut = bioOut;
                    SSL_set_bio(ssl, bioIn, bioOut);
                    return ssl;
                }

                void SslStream::AuthenticateAsClient(const String& targetHost) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (m_pSsl) throw IO::IOException("Already authenticated.");

                    SSL_CTX* ctx = static_cast<SSL_CTX*>(CreateSslContext(false));
                    m_pSslCtx = ctx;
                    SSL* ssl = CreateAndBindSsl(ctx, m_pBioIn, m_pBioOut);
                    m_pSsl = ssl;
                    SSL_set_tlsext_host_name(ssl, targetHost.GetRawString());
                    SSL_set_connect_state(ssl);
                    ProcessHandshake();
                }

                void SslStream::AuthenticateAsServer(const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (m_pSsl) throw IO::IOException("Already authenticated.");
                    if (certificate.IsNull()) throw ArgumentNullException("certificate cannot be null.");

                    SSL_CTX* ctx = static_cast<SSL_CTX*>(CreateSslContext(true));
                    m_pSslCtx = ctx;
                    ConfigureServerCert(ctx, certificate);
                    SSL* ssl = CreateAndBindSsl(ctx, m_pBioIn, m_pBioOut);
                    m_pSsl = ssl;
                    SSL_set_accept_state(ssl);
                    ProcessHandshake();
                }

                static void PumpNetworkToBio(IO::Stream* pStream, void* pBioIn, const char* pErrorContext) {
                    char buffer[4096];
                    int read = pStream->Read(buffer, 0, sizeof(buffer));
                    if (read <= 0) throw IO::IOException(pErrorContext);
                    BIO_write(static_cast<BIO*>(pBioIn), buffer, read);
                }

                static void FlushBioOutbound(void* pBioOut, const SmartPointer<IO::Stream>& spInnerStream) {
                    if (!pBioOut) return;
                    char buffer[4096];
                    while (true) {
                        int read = BIO_read(static_cast<BIO*>(pBioOut), buffer, sizeof(buffer));
                        if (read <= 0) break;
                        spInnerStream->Write(buffer, 0, read);
                    }
                }

                static void HandleHandshakeError(int err, void* pBioIn, void* pBioOut, const SmartPointer<IO::Stream>& spStream) {
                    FlushBioOutbound(pBioOut, spStream);
                    if (err == SSL_ERROR_WANT_READ) {
                        PumpNetworkToBio(spStream.Get(), pBioIn, "Connection closed during SSL handshake.");
                    } else if (err != SSL_ERROR_WANT_WRITE) {
                        char errBuf[256];
                        ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                        throw IO::IOException(String("SSL handshake failed: ") + errBuf);
                    }
                }

                void SslStream::ProcessHandshake() {
                    SSL* ssl = static_cast<SSL*>(m_pSsl);
                    while (!SSL_is_init_finished(ssl)) {
                        int ret = SSL_do_handshake(ssl);
                        if (ret == 1) break;
                        HandleHandshakeError(SSL_get_error(ssl, ret), m_pBioIn, m_pBioOut, m_spInnerStream);
                    }
                    FlushOutboundBio();
                }

                void SslStream::FlushOutboundBio() {
                    FlushBioOutbound(m_pBioOut, m_spInnerStream);
                }

                bool SslStream::CanRead() const { return !m_bDisposed; }
                bool SslStream::CanSeek() const { return false; }
                bool SslStream::CanWrite() const { return !m_bDisposed; }
                long SslStream::GetLength() const { throw IO::IOException("SslStream does not support seeking."); }
                long SslStream::GetPosition() const { throw IO::IOException("SslStream does not support seeking."); }
                void SslStream::SetPosition(long value) { throw IO::IOException("SslStream does not support seeking."); }

                void SslStream::Flush() {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    m_spInnerStream->Flush();
                }

                long SslStream::Seek(long offset, int origin) { throw IO::IOException("SslStream does not support seeking."); }
                void SslStream::SetLength(long value) { throw IO::IOException("SslStream does not support seeking."); }

                static int HandleReadError(int err, const SmartPointer<IO::Stream>& spStream, void* pBioIn, void* pBioOut) {
                    if (err == SSL_ERROR_WANT_READ) {
                        FlushBioOutbound(pBioOut, spStream);
                        char rawBuf[4096];
                        int read = spStream->Read(rawBuf, 0, sizeof(rawBuf));
                        if (read <= 0) return 0;
                        BIO_write(static_cast<BIO*>(pBioIn), rawBuf, read);
                        return -1;
                    }
                    if (err == SSL_ERROR_ZERO_RETURN) return 0;
                    if (err == SSL_ERROR_WANT_WRITE) {
                        FlushBioOutbound(pBioOut, spStream);
                        return -1;
                    }
                    char errBuf[256];
                    ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                    throw IO::IOException(String("SSL read failed: ") + errBuf);
                }

                int SslStream::Read(char* buffer, int offset, int count) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (!m_pSsl) throw IO::IOException("SslStream is not authenticated.");
                    SSL* ssl = static_cast<SSL*>(m_pSsl);
                    while (true) {
                        int ret = SSL_read(ssl, buffer + offset, count);
                        if (ret > 0) return ret;
                        int r = HandleReadError(SSL_get_error(ssl, ret), m_spInnerStream, m_pBioIn, m_pBioOut);
                        if (r >= 0) return r;
                    }
                }

                static void HandleWriteError(int err, const SmartPointer<IO::Stream>& spStream, void* pBioIn, void* pBioOut) {
                    if (err == SSL_ERROR_WANT_WRITE) {
                        FlushBioOutbound(pBioOut, spStream);
                    } else if (err == SSL_ERROR_WANT_READ) {
                        FlushBioOutbound(pBioOut, spStream);
                        PumpNetworkToBio(spStream.Get(), pBioIn, "Connection closed during SSL write.");
                    } else {
                        char errBuf[256];
                        ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                        throw IO::IOException(String("SSL write failed: ") + errBuf);
                    }
                }

                void SslStream::Write(const char* buffer, int offset, int count) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (!m_pSsl) throw IO::IOException("SslStream is not authenticated.");
                    SSL* ssl = static_cast<SSL*>(m_pSsl);
                    int written = 0;
                    while (written < count) {
                        int ret = SSL_write(ssl, buffer + offset + written, count - written);
                        if (ret <= 0) {
                            HandleWriteError(SSL_get_error(ssl, ret), m_spInnerStream, m_pBioIn, m_pBioOut);
                        } else {
                            written += ret;
                            FlushOutboundBio();
                        }
                    }
                }

                void SslStream::Dispose() {
                    if (!m_bDisposed) {
                        m_bDisposed = true;
                        
                        if (m_pSsl) {
                            SSL_free(static_cast<SSL*>(m_pSsl));
                            m_pSsl = nullptr;
                        }
                        
                        if (m_pSslCtx) {
                            SSL_CTX_free(static_cast<SSL_CTX*>(m_pSslCtx));
                            m_pSslCtx = nullptr;
                        }
                        
                        m_pBioIn = nullptr;
                        m_pBioOut = nullptr;
                        
                        if (!m_bLeaveInnerStreamOpen && !m_spInnerStream.IsNull()) {
                            m_spInnerStream->Dispose();
                        }
                        m_spInnerStream = nullptr;
                    }
                }

            }
        }
    }
}

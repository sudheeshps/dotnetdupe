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

                void SslStream::AuthenticateAsClient(const String& targetHost) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (m_pSsl) throw IO::IOException("Already authenticated.");

                    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
                    if (!ctx) {
                        throw IO::IOException("Failed to create SSL context.");
                    }
                    m_pSslCtx = ctx;

                    // Support TLS 1.2 and 1.3 by default
                    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

                    SSL* ssl = SSL_new(ctx);
                    if (!ssl) {
                        SSL_CTX_free(ctx);
                        m_pSslCtx = nullptr;
                        throw IO::IOException("Failed to create SSL handle.");
                    }
                    m_pSsl = ssl;

                    // Set SNI
                    SSL_set_tlsext_host_name(ssl, targetHost.GetRawString());

                    BIO* bioIn = BIO_new(BIO_s_mem());
                    BIO* bioOut = BIO_new(BIO_s_mem());
                    m_pBioIn = bioIn;
                    m_pBioOut = bioOut;

                    SSL_set_bio(ssl, bioIn, bioOut);
                    SSL_set_connect_state(ssl);

                    ProcessHandshake();
                }

                void SslStream::AuthenticateAsServer(const SmartPointer<::DotNetDupe::System::Security::Cryptography::X509Certificates::X509Certificate2>& certificate) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (m_pSsl) throw IO::IOException("Already authenticated.");
                    if (certificate.IsNull()) {
                        throw ArgumentNullException("certificate cannot be null.");
                    }

                    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
                    if (!ctx) {
                        throw IO::IOException("Failed to create SSL context.");
                    }
                    m_pSslCtx = ctx;

                    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

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

                    SSL* ssl = SSL_new(ctx);
                    if (!ssl) {
                        SSL_CTX_free(ctx);
                        m_pSslCtx = nullptr;
                        throw IO::IOException("Failed to create SSL handle.");
                    }
                    m_pSsl = ssl;

                    BIO* bioIn = BIO_new(BIO_s_mem());
                    BIO* bioOut = BIO_new(BIO_s_mem());
                    m_pBioIn = bioIn;
                    m_pBioOut = bioOut;

                    SSL_set_bio(ssl, bioIn, bioOut);
                    SSL_set_accept_state(ssl);

                    ProcessHandshake();
                }

                void SslStream::ProcessHandshake() {
                    SSL* ssl = static_cast<SSL*>(m_pSsl);
                    
                    while (!SSL_is_init_finished(ssl)) {
                        int ret = SSL_do_handshake(ssl);
                        if (ret == 1) {
                            break; // Handshake complete!
                        }
                        
                        int err = SSL_get_error(ssl, ret);
                        if (err == SSL_ERROR_WANT_READ) {
                            FlushOutboundBio();
                            
                            char buffer[4096];
                            int read = m_spInnerStream->Read(buffer, 0, sizeof(buffer));
                            if (read <= 0) {
                                throw IO::IOException("Connection closed during SSL handshake.");
                            }
                            
                            BIO_write(static_cast<BIO*>(m_pBioIn), buffer, read);
                        } else if (err == SSL_ERROR_WANT_WRITE) {
                            FlushOutboundBio();
                        } else {
                            char errBuf[256];
                            ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                            throw IO::IOException(String("SSL handshake failed: ") + errBuf);
                        }
                    }
                    
                    FlushOutboundBio();
                }

                void SslStream::FlushOutboundBio() {
                    if (!m_pBioOut) return;
                    
                    char buffer[4096];
                    while (true) {
                        int read = BIO_read(static_cast<BIO*>(m_pBioOut), buffer, sizeof(buffer));
                        if (read <= 0) break;
                        m_spInnerStream->Write(buffer, 0, read);
                    }
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

                long SslStream::Seek(long offset, int origin) {
                    throw IO::IOException("SslStream does not support seeking.");
                }

                void SslStream::SetLength(long value) {
                    throw IO::IOException("SslStream does not support seeking.");
                }

                int SslStream::Read(char* buffer, int offset, int count) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    if (!m_pSsl) throw IO::IOException("SslStream is not authenticated.");
                    
                    SSL* ssl = static_cast<SSL*>(m_pSsl);
                    
                    while (true) {
                        int ret = SSL_read(ssl, buffer + offset, count);
                        if (ret > 0) {
                            return ret;
                        }
                        
                        int err = SSL_get_error(ssl, ret);
                        if (err == SSL_ERROR_WANT_READ) {
                            FlushOutboundBio();
                            
                            char rawBuf[4096];
                            int read = m_spInnerStream->Read(rawBuf, 0, sizeof(rawBuf));
                            if (read <= 0) {
                                return 0; // EOF
                            }
                            
                            BIO_write(static_cast<BIO*>(m_pBioIn), rawBuf, read);
                        } else if (err == SSL_ERROR_ZERO_RETURN) {
                            return 0; // SSL closed cleanly
                        } else if (err == SSL_ERROR_WANT_WRITE) {
                            FlushOutboundBio();
                        } else {
                            char errBuf[256];
                            ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                            throw IO::IOException(String("SSL read failed: ") + errBuf);
                        }
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
                            int err = SSL_get_error(ssl, ret);
                            if (err == SSL_ERROR_WANT_WRITE) {
                                FlushOutboundBio();
                            } else if (err == SSL_ERROR_WANT_READ) {
                                FlushOutboundBio();
                                char rawBuf[4096];
                                int read = m_spInnerStream->Read(rawBuf, 0, sizeof(rawBuf));
                                if (read <= 0) {
                                    throw IO::IOException("Connection closed during SSL write.");
                                }
                                BIO_write(static_cast<BIO*>(m_pBioIn), rawBuf, read);
                            } else {
                                char errBuf[256];
                                ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
                                throw IO::IOException(String("SSL write failed: ") + errBuf);
                            }
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

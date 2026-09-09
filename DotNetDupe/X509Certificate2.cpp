#include "pch.h"
#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"
#include "System/ArgumentException.h"
#include "System/IOException.h"
#include <cstdio>

#include <mutex>

#if defined(_WIN32)
    #include <openssl/ssl.h>
    #include <openssl/pem.h>
    #include <openssl/x509.h>
    #include <openssl/err.h>
#else
    #include <openssl/ssl.h>
    #include <openssl/pem.h>
    #include <openssl/x509.h>
    #include <openssl/err.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {
                namespace X509Certificates {

                    static std::once_flag s_cryptoInitOnce;

                    static void InitializeCrypto() {
                        /// Step: Register OpenSSL algorithms and error strings once per process.
                        std::call_once(s_cryptoInitOnce, []() {
                            OpenSSL_add_all_algorithms();
                            ERR_load_crypto_strings();
                        });
                    }

                    static X509* LoadPemCert(const String& certPath) {
                        /// Guard: Open file BIO for certificate path.
                        BIO* pCertBio = BIO_new_file(certPath.GetRawString(), "r");
                        if (!pCertBio) throw IO::IOException("Could not open certificate file.");

                        /// Step: Parse PEM X.509 certificate and release BIO handle.
                        X509* pCert = PEM_read_bio_X509(pCertBio, nullptr, nullptr, nullptr);
                        BIO_free(pCertBio);
                        if (!pCert) throw ArgumentException("Invalid PEM certificate.");
                        return pCert;
                    }

                    static EVP_PKEY* LoadPemKey(const String& keyPath) {
                        /// Guard: Open file BIO for private key path.
                        BIO* pKeyBio = BIO_new_file(keyPath.GetRawString(), "r");
                        if (!pKeyBio) throw IO::IOException("Could not open private key file.");

                        /// Step: Parse PEM private key and release BIO handle.
                        EVP_PKEY* pKey = PEM_read_bio_PrivateKey(pKeyBio, nullptr, nullptr, nullptr);
                        BIO_free(pKeyBio);
                        if (!pKey) throw ArgumentException("Invalid PEM private key.");
                        return pKey;
                    }

                    X509Certificate2::X509Certificate2(const String& certPath, const String& keyPath)
                        : m_pCert(nullptr), m_pKey(nullptr) {
                        /// Step: Initialize cryptographic subsystem.
                        InitializeCrypto();

                        /// Step: Load certificate and key with exception cleanup.
                        X509* pCert = LoadPemCert(certPath);
                        try {
                            m_pKey = LoadPemKey(keyPath);
                            m_pCert = pCert;
                        } catch (...) {
                            /// Cleanup: Free allocated certificate on private key load failure.
                            X509_free(pCert);
                            throw;
                        }
                    }

                    X509Certificate2::~X509Certificate2() {
                        /// Step: Release OpenSSL X509 certificate structure.
                        if (m_pCert) {
                            X509_free(static_cast<X509*>(m_pCert));
                        }
                        /// Step: Release OpenSSL EVP_PKEY private key structure.
                        if (m_pKey) {
                            EVP_PKEY_free(static_cast<EVP_PKEY*>(m_pKey));
                        }
                    }

                    void* X509Certificate2::GetInternalCert() const {
                        /// Return: Internal OpenSSL X509* pointer.
                        return m_pCert;
                    }

                    void* X509Certificate2::GetInternalKey() const {
                        /// Return: Internal OpenSSL EVP_PKEY* pointer.
                        return m_pKey;
                    }

                }
            }
        }
    }
}

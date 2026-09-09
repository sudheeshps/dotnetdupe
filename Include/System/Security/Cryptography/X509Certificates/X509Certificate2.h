/// \file X509Certificate2.h
/// \brief Represents an X.509 certificate and private key per RFC 5280.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {
                namespace X509Certificates {

                    /// \class X509Certificate2
                    /// \brief Represents an X.509 v3 certificate containing public and private keys conforming to RFC 5280.
                    ///
                    /// Encapsulates native OpenSSL X509 and EVP_PKEY pointers to provide certificate parsing,
                    /// validation, and cryptographic handshake support for TLS and HTTPS services.
                    class X509Certificate2 : public Object {
                    public:
                        /// \brief Initializes a new instance of X509Certificate2 from PEM certificate and private key files.
                        /// \param certPath File path to the PEM-encoded X.509 certificate.
                        /// \param keyPath File path to the PEM-encoded private key file.
                        DOTNETDUPE_API X509Certificate2(const String& certPath, const String& keyPath);

                        /// \brief Releases OpenSSL certificate and private key resources.
                        DOTNETDUPE_API ~X509Certificate2();

                        /// \brief Gets the internal OpenSSL X509* pointer.
                        /// \return Pointer to native OpenSSL X509 structure.
                        DOTNETDUPE_API void* GetInternalCert() const;

                        /// \brief Gets the internal OpenSSL EVP_PKEY* pointer.
                        /// \return Pointer to native OpenSSL EVP_PKEY structure.
                        DOTNETDUPE_API void* GetInternalKey() const;

                    private:
                        void* m_pCert; // X509*
                        void* m_pKey;  // EVP_PKEY*
                    };

                }
            }
        }
    }
}

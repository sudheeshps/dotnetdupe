#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {
                namespace X509Certificates {

                    class X509Certificate2 : public Object {
                    public:
                        DOTNETDUPE_API X509Certificate2(const String& certPath, const String& keyPath);
                        DOTNETDUPE_API ~X509Certificate2();

                        DOTNETDUPE_API void* GetInternalCert() const;
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

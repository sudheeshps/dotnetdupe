#pragma once
#include "Common.h"
#include "System/Object.h"
#include "System/String.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/Dictionary.h"

namespace DotNetDupe {
    namespace System {
        namespace IdentityModel {
            namespace Tokens {
                namespace Jwt {

                    class JWTToken : public Object {
                    public:
                        DOTNETDUPE_API JWTToken();
                        DOTNETDUPE_API ~JWTToken() = default;

                        DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetHeader();
                        DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetHeader() const;

                        DOTNETDUPE_API Collections::Generic::Dictionary<String, String>& GetPayload();
                        DOTNETDUPE_API const Collections::Generic::Dictionary<String, String>& GetPayload() const;

                        DOTNETDUPE_API String GetSignature() const;

                        DOTNETDUPE_API String CreateToken(const String& secretKey);

                        DOTNETDUPE_API static SmartPointer<JWTToken> Parse(const String& tokenStr);
                        DOTNETDUPE_API bool Verify(const String& secretKey) const;

                    private:
                        Collections::Generic::Dictionary<String, String> m_header;
                        Collections::Generic::Dictionary<String, String> m_payload;
                        String m_signature;
                        String m_rawTokenWithoutSignature;
                    };

                }
            }
        }
    }
}

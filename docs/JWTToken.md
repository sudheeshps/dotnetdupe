# JWTToken, HMACSHA256 &amp; X509Certificate2

**Namespace:** `DotNetDupe::System::IdentityModel::Tokens::Jwt` & `DotNetDupe::System::Security::Cryptography`  
**Header:** `#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"`, `#include "System/Security/Cryptography/HMACSHA256.h"`, `#include "System/Security/Cryptography/X509Certificates/X509Certificate2.h"`

Provides token generation, signature validation, HMAC-SHA256 hashing, and X.509 certificate loading for SSL/TLS authentication.

---

## `HMACSHA256` Class

Computes a Hash-based Message Authentication Code (HMAC) using the SHA256 hash function.

### Methods
- `Array<char> ComputeHash(const Array<char>& buffer)`: Computes the hash for the given byte buffer.
- `static Array<char> ComputeHash(const Array<char>& buffer, const Array<char>& key)`: Static helper to compute HMAC-SHA256.

---

## `X509Certificate2` Class

Represents an X.509 certificate and private key pair for TLS servers.

### Constructors
- `X509Certificate2(const String& certPath, const String& keyPath)`: Loads PEM certificate and private key from files.

### Member Functions
- `void* GetInternalCert() const`: Returns the internal OpenSSL `X509*` handle.
- `void* GetInternalKey() const`: Returns the internal OpenSSL `EVP_PKEY*` handle.

---

## `JWTToken` Class

Encapsulates JSON Web Token (JWT) encoding, header/claims inspection, and HMAC-SHA256 signature verification.

### Methods
- `Dictionary<String, String>& GetHeader()` / `const Dictionary<String, String>& GetHeader() const`: Gets token headers (e.g. `alg`, `typ`).
- `Dictionary<String, String>& GetPayload()` / `const Dictionary<String, String>& GetPayload() const`: Gets token claims (e.g. `sub`, `name`, `role`, `exp`).
- `String GetSignature() const`: Returns raw signature string.
- `String CreateToken(const String& secretKey)`: Signs and encodes the JWT token into a compact `header.payload.signature` string.
- `static SmartPointer<JWTToken> Parse(const String& tokenStr)`: Parses an encoded JWT string into a `JWTToken` object.
- `bool Verify(const String& secretKey) const`: Validates token signature against a secret key.

---

## Example

```cpp
#include "System/Console.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/String.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;

int main() {
    String sSecret = "SuperSecretSigningKey123456789!";

    // Create Token
    JWTToken jwtToken;
    jwtToken.GetPayload().Add("sub", "user_1029");
    jwtToken.GetPayload().Add("role", "Administrator");
    jwtToken.GetPayload().Add("iss", "DotNetDupeAuth");

    String sJwtString = jwtToken.CreateToken(sSecret);
    Console::WriteLine("Generated JWT:\n{0}", sJwtString);

    // Parse and Verify Token
    auto spParsedToken = JWTToken::Parse(sJwtString);
    bool bIsValid = spParsedToken->Verify(sSecret);

    Console::WriteLine("Signature Valid: {0}", bIsValid);
    Console::WriteLine("Subject Claim:   {0}", spParsedToken->GetPayload()["sub"]);
    Console::WriteLine("Role Claim:      {0}", spParsedToken->GetPayload()["role"]);

    return 0;
}
```

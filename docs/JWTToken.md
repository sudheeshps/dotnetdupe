### System.IdentityModel.Tokens.Jwt Namespace

Provides a programming interface for JSON Web Token (JWT) creation, parsing, and verification.

---

### class `JWTToken`

Represents a JSON Web Token containing a header block, a claims payload, and a cryptographic signature.

#### Methods

##### `JWTToken()`
Initializes a new instance of the `JWTToken` class with default header values (`alg: HS256`, `typ: JWT`).

##### `Dictionary<String, String>& GetHeader()`
Gets the dictionary representing the token header properties.

##### `Dictionary<String, String>& GetPayload()`
Gets the dictionary representing the token payload claims.

##### `String GetSignature() const`
Gets the parsed Base64Url-encoded signature string (only set for parsed tokens).

##### `String CreateToken(const String& secretKey)`
Serializes, Base64Url-encodes, and signs the token using HMAC-SHA256 with the specified secret key. Returns the complete token string.

##### `static SmartPointer<JWTToken> Parse(const String& tokenStr)`
Parses a standard dot-separated JWT token string into a `JWTToken` instance. Throws `ArgumentException` if the format is invalid.

##### `bool Verify(const String& secretKey) const`
Verifies the token's cryptographic signature against the specified secret key. Returns `true` if valid, otherwise `false`.

---

### class `Convert` (Extensions)

Provides Base64 helpers.

##### `static String ToBase64String(const Array<char>& inArray)`
Conves a byte array to its equivalent string representation encoded with base-64 digits.

##### `static Array<char> FromBase64String(const String& s)`
Conves the specified string, which encodes binary data as base-64 digits, to an equivalent byte array.

---

## Code Example

The following example demonstrates how to create, sign, parse, and verify a JWT token using `JWTToken`.

```cpp
#include "System/Console.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;

int main() {
    try {
        // 1. Create a new JWT Token and add claims
        JWTToken token;
        token.GetPayload().Add("iss", "auth-service");
        token.GetPayload().Add("sub", "john_doe_12");
        token.GetPayload().Add("role", "admin");

        String secretKey = "super-secret-key-phrase";

        // 2. Serialize and sign the token
        String jwtStr = token.CreateToken(secretKey);
        Console::WriteLine("Generated JWT:");
        Console::WriteLine(jwtStr);

        // 3. Parse the token back
        Console::WriteLine("\nParsing JWT...");
        auto parsedToken = JWTToken::Parse(jwtStr);

        Console::Write("Issuer (iss): ");
        Console::WriteLine(parsedToken->GetPayload()["iss"]);

        Console::Write("Subject (sub): ");
        Console::WriteLine(parsedToken->GetPayload()["sub"]);

        // 4. Verify token signatures
        Console::Write("Verifying with correct key: ");
        Console::WriteLine(parsedToken->Verify(secretKey) ? "SUCCESS" : "FAILED");

        Console::Write("Verifying with incorrect key: ");
        Console::WriteLine(parsedToken->Verify("wrong-key") ? "SUCCESS" : "FAILED");

    } catch (const BasicException<char>& ex) {
        Console::Write("Error: ");
        Console::WriteLine(ex.What());
    }

    return 0;
}
```

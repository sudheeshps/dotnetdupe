#include "System/String.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"

#include "System/Console.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>

using namespace DotNetDupe::System;

#include "Demos.h"

void DemonstrateJwt() {
    Console::WriteLine("\n=== Demonstrate JWT Token ===");
    try {
        using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;

        // 1. Create token
        JWTToken token;
        token.GetPayload().Add("iss", "dot-net-dupe-auth");
        token.GetPayload().Add("sub", "user_98765");
        token.GetPayload().Add("scope", "read write");

        String secret = "secure-signature-secret-key";

        Console::WriteLine("Generating signed JWT token...");
        String tokenStr = token.CreateToken(secret);
        Console::Write("JWT Token string: ");
        Console::WriteLine(tokenStr);

        // 2. Parse token
        Console::WriteLine("Parsing token...");
        auto parsedToken = JWTToken::Parse(tokenStr);
        
        Console::Write("  Subject claim: ");
        Console::WriteLine(parsedToken->GetPayload()["sub"]);
        Console::Write("  Scope claim:   ");
        Console::WriteLine(parsedToken->GetPayload()["scope"]);

        // 3. Verify signature
        Console::Write("Verifying signature with correct key:   ");
        Console::WriteLine(parsedToken->Verify(secret) ? "VALID" : "INVALID");

        Console::Write("Verifying signature with incorrect key: ");
        Console::WriteLine(parsedToken->Verify("some-other-secret-key") ? "VALID" : "INVALID");

    } catch (const BasicException<char>& ex) {
        Console::Write("Error during JWT demonstration: ");
        Console::WriteLine(ex.What());
    }
}

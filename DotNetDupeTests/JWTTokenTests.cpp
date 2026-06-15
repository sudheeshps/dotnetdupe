#include "pch.h"
#include "gtest/gtest.h"
#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
#include "System/Convert.h"
#include "System/Security/Cryptography/HMACSHA256.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IdentityModel::Tokens::Jwt;
using namespace DotNetDupe::System::Security::Cryptography;

namespace DotNetDupeTests {

    TEST(JWTTokenTests, GivenBytes_WhenBase64Encoded_ThenSucceedsAndDecodesCorrectly) {
        // Given
        Array<char> bytes({'H', 'e', 'l', 'l', 'o'});

        // When
        String encoded = Convert::ToBase64String(bytes);
        Array<char> decoded = Convert::FromBase64String(encoded);

        // Then
        EXPECT_EQ(encoded, "SGVsbG8=");
        EXPECT_EQ(decoded.GetLength(), 5);
        EXPECT_EQ(decoded[0], 'H');
        EXPECT_EQ(decoded[4], 'o');
    }

    TEST(JWTTokenTests, GivenKeyAndBuffer_WhenHMACSHA256Computed_ThenResultMatchesStandard) {
        // Given
        Array<char> key({'k', 'e', 'y'});
        Array<char> data({'t', 'e', 's', 't'});

        // When
        Array<char> hash = HMACSHA256::ComputeHash(data, key);
        String base64 = Convert::ToBase64String(hash);

        // Then
        EXPECT_EQ(base64, "Aq+1YwSQLGVvy3N83QPeYgW7bUAdooEu/ZstNqCK8Vk=");
    }

    TEST(JWTTokenTests, GivenJWTToken_WhenCreatedAndVerified_ThenSecurityIsMaintained) {
        // Given
        JWTToken token;
        token.GetPayload().Add("sub", "user123");
        token.GetPayload().Add("name", "Alice");

        String secret = "my-secret-key-12345";

        // When
        String tokenStr = token.CreateToken(secret);
        auto parsedToken = JWTToken::Parse(tokenStr);

        // Then
        EXPECT_TRUE(parsedToken->Verify(secret));
        EXPECT_FALSE(parsedToken->Verify("wrong-secret-key"));
        EXPECT_EQ(parsedToken->GetPayload()["sub"], "user123");
        EXPECT_EQ(parsedToken->GetPayload()["name"], "Alice");
    }
}

#include "pch.h"
#include "gtest/gtest.h"
#include "System/Security/Cryptography/SHA256.h"
#include "System/IO/MemoryStream.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Security::Cryptography;

TEST(SHA256Tests, GivenEmptyInput_WhenComputeHash_ThenMatchesKnownStandardHash) {
    Array<byte> empty(0);
    Array<byte> hash = SHA256::HashData(empty);
    EXPECT_EQ(hash.GetLength(), 32);
    String hex = SHA256::ToHexString(hash);
    EXPECT_STREQ(hex.GetRawString(), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(SHA256Tests, GivenAsciiText_WhenComputeHash_ThenMatchesFipsVector) {
    const char* pMsg = "hello world";
    Array<byte> buffer(11);
    for (int i = 0; i < 11; i++) buffer[i] = static_cast<byte>(pMsg[i]);
    Array<byte> hash = SHA256::HashData(buffer);
    String hex = SHA256::ToHexString(hash);
    EXPECT_STREQ(hex.GetRawString(), "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9");
}

TEST(SHA256Tests, GivenMemoryStream_WhenHashDataCalled_ThenComputesStreamHash) {
    const char* pMsg = "hello world";
    MemoryStream memStream;
    memStream.Write(pMsg, 0, 11);
    memStream.SetPosition(0);
    Array<byte> hash = SHA256::HashData(&memStream);
    String hex = SHA256::ToHexString(hash);
    EXPECT_STREQ(hex.GetRawString(), "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9");
}

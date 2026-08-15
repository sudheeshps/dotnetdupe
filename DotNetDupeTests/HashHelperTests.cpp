#include "pch.h"
#include <gtest/gtest.h>
#include "System/HashHelper.h"
#include "System/String.h"
#include "System/SmartPointer.h"

using namespace DotNetDupe::System;

class HashTestObject : public Object {
public:
    int Val;
    HashTestObject(int v) : Val(v) {}
    int GetHashCode() const override {
        return Val * 42;
    }
};

TEST(HashHelperTests, PrimitiveHashes) {
    EXPECT_EQ(HashHelper<int>::GetHashCode(5), 5);
    EXPECT_EQ(HashHelper<bool>::GetHashCode(true), 1);
    EXPECT_EQ(HashHelper<bool>::GetHashCode(false), 0);
    EXPECT_EQ(HashHelper<char>::GetHashCode('A'), 'A');
}

TEST(HashHelperTests, StringHash) {
    String s1("TestString");
    String s2("TestString");
    String s3("DifferentString");
    
    EXPECT_EQ(HashHelper<String>::GetHashCode(s1), HashHelper<String>::GetHashCode(s2));
    EXPECT_NE(HashHelper<String>::GetHashCode(s1), HashHelper<String>::GetHashCode(s3));
}

TEST(HashHelperTests, SmartPointerHash) {
    auto p1 = SmartPointer<HashTestObject>::NewShared(10);
    auto p2 = SmartPointer<HashTestObject>::NewShared(10);
    
    // Hash of SmartPointer should be the object's hash (if properly implemented, though our HashHelper delegates to GetHashCode)
    EXPECT_EQ(HashHelper<SmartPointer<HashTestObject>>::GetHashCode(p1), 420);
    EXPECT_EQ(HashHelper<SmartPointer<HashTestObject>>::GetHashCode(p2), 420);
    
    SmartPointer<HashTestObject> nullPtr;
    EXPECT_EQ(HashHelper<SmartPointer<HashTestObject>>::GetHashCode(nullPtr), 0);
}

TEST(HashHelperTests, RawPointerHash) {
    int val = 5;
    int* pVal = &val;
    EXPECT_NE(HashHelper<int*>::GetHashCode(pVal), 0);
    EXPECT_EQ(HashHelper<int*>::GetHashCode(nullptr), 0);
}

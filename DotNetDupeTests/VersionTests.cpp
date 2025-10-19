#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/Version.h"
#include "../DotNetDupe/String.h"

using namespace DotNetDupe::System;

namespace SystemTests {
    namespace VersionTests {
        // Given: Major, minor, build, and revision numbers
        // When: A Version object is constructed with all arguments
        // Then: The properties of the Version object should be correctly set
        TEST(VersionTest, Constructor_With_All_Arguments) {
            // Given
            int major = 1, minor = 2, build = 3, revision = 4;

            // When
            Version v(major, minor, build, revision);

            // Then
            EXPECT_EQ(v.GetMajor(), major);
            EXPECT_EQ(v.GetMinor(), minor);
            EXPECT_EQ(v.GetBuild(), build);
            EXPECT_EQ(v.GetRevision(), revision);
        }

        // Given: Major, minor, and build numbers
        // When: A Version object is constructed with three arguments
        // Then: The revision number should be 0
        TEST(VersionTest, Constructor_With_Three_Arguments) {
            // Given
            int major = 1, minor = 2, build = 3;

            // When
            Version v(major, minor, build);

            // Then
            EXPECT_EQ(v.GetMajor(), major);
            EXPECT_EQ(v.GetMinor(), minor);
            EXPECT_EQ(v.GetBuild(), build);
            EXPECT_EQ(v.GetRevision(), 0);
        }

        // Given: Major and minor numbers
        // When: A Version object is constructed with two arguments
        // Then: The build and revision numbers should be 0
        TEST(VersionTest, Constructor_With_Two_Arguments) {
            // Given
            int major = 1, minor = 2;

            // When
            Version v(major, minor);

            // Then
            EXPECT_EQ(v.GetMajor(), major);
            EXPECT_EQ(v.GetMinor(), minor);
            EXPECT_EQ(v.GetBuild(), 0);
            EXPECT_EQ(v.GetRevision(), 0);
        }

        // Given: No arguments
        // When: A Version object is constructed with the default constructor
        // Then: All properties should be 0
        TEST(VersionTest, Default_Constructor) {
            // Given, When
            Version v;

            // Then
            EXPECT_EQ(v.GetMajor(), 0);
            EXPECT_EQ(v.GetMinor(), 0);
            EXPECT_EQ(v.GetBuild(), 0);
            EXPECT_EQ(v.GetRevision(), 0);
        }

        // Given: A Version object
        // When: The ToString method is called
        // Then: The method should return the correct string representation
        TEST(VersionTest, ToString_Returns_Correct_String) {
            // Given
            Version v(1, 2, 3, 4);

            // When
            String s = v.ToString();

            // Then
            EXPECT_EQ(s, _T("1.2.3.4"));
        }

        // Given: Two Version objects with the same version numbers
        // When: The objects are compared using the equality operator
        // Then: The result should be true
        TEST(VersionTest, Operator_Equals_Returns_True_For_Same_Versions) {
            // Given
            Version v1(1, 2, 3, 4);
            Version v2(1, 2, 3, 4);

            // When, Then
            EXPECT_TRUE(v1 == v2);
        }

        // Given: Two Version objects with different version numbers
        // When: The objects are compared using the equality operator
        // Then: The result should be false
        TEST(VersionTest, Operator_Equals_Returns_False_For_Different_Versions) {
            // Given
            Version v1(1, 2, 3, 4);
            Version v2(1, 2, 3, 5);

            // When, Then
            EXPECT_FALSE(v1 == v2);
        }

        // Given: Two Version objects with the same version numbers
        // When: The objects are compared using the inequality operator
        // Then: The result should be false
        TEST(VersionTest, Operator_Not_Equals_Returns_False_For_Same_Versions) {
            // Given
            Version v1(1, 2, 3, 4);
            Version v2(1, 2, 3, 4);

            // When, Then
            EXPECT_FALSE(v1 != v2);
        }

        // Given: Two Version objects with different version numbers
        // When: The objects are compared using the inequality operator
        // Then: The result should be true
        TEST(VersionTest, Operator_Not_Equals_Returns_True_For_Different_Versions) {
            // Given
            Version v1(1, 2, 3, 4);
            Version v2(1, 2, 3, 5);

            // When, Then
            EXPECT_TRUE(v1 != v2);
        }
    }
}

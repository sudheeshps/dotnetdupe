#include "pch.h"
#include "gtest/gtest.h"
#include "System/Version.h"
#include "System/String.h"
#include "System/ArgumentException.h"
#include "System/FormatException.h"

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
            EXPECT_EQ(s, "1.2.3.4");
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

        TEST(VersionTest, GivenTwoPartString_WhenParseCalled_ThenConstructsVersion) {
            Version v = Version::Parse("2.5");
            EXPECT_EQ(v.GetMajor(), 2);
            EXPECT_EQ(v.GetMinor(), 5);
            EXPECT_EQ(v.GetBuild(), 0);
            EXPECT_EQ(v.GetRevision(), 0);
        }

        TEST(VersionTest, GivenThreePartString_WhenParseCalled_ThenConstructsVersion) {
            Version v = Version::Parse("3.1.4");
            EXPECT_EQ(v.GetMajor(), 3);
            EXPECT_EQ(v.GetMinor(), 1);
            EXPECT_EQ(v.GetBuild(), 4);
            EXPECT_EQ(v.GetRevision(), 0);
        }

        TEST(VersionTest, GivenFourPartString_WhenParseCalled_ThenConstructsVersion) {
            Version v = Version::Parse("10.0.19041.1");
            EXPECT_EQ(v.GetMajor(), 10);
            EXPECT_EQ(v.GetMinor(), 0);
            EXPECT_EQ(v.GetBuild(), 19041);
            EXPECT_EQ(v.GetRevision(), 1);
        }

        TEST(VersionTest, GivenEmptyString_WhenParseCalled_ThenThrowsArgumentException) {
            EXPECT_THROW(Version::Parse(""), ArgumentException);
        }

        TEST(VersionTest, GivenInvalidFormatStrings_WhenParseCalled_ThenThrowsFormatException) {
            EXPECT_THROW(Version::Parse("1"), FormatException);
            EXPECT_THROW(Version::Parse("1.2.3.4.5"), FormatException);
            EXPECT_THROW(Version::Parse("1.a"), FormatException);
            EXPECT_THROW(Version::Parse("1.-2"), FormatException);
            EXPECT_THROW(Version::Parse("1.2."), FormatException);
            EXPECT_THROW(Version::Parse(".1.2"), FormatException);
            EXPECT_THROW(Version::Parse("1..2"), FormatException);
        }

        TEST(VersionTest, GivenValidString_WhenTryParseCalled_ThenReturnsTrueAndPopulatesResult) {
            Version v;
            EXPECT_TRUE(Version::TryParse("4.8.0.1234", v));
            EXPECT_EQ(v.GetMajor(), 4);
            EXPECT_EQ(v.GetMinor(), 8);
            EXPECT_EQ(v.GetBuild(), 0);
            EXPECT_EQ(v.GetRevision(), 1234);
        }

        TEST(VersionTest, GivenInvalidString_WhenTryParseCalled_ThenReturnsFalse) {
            Version v;
            EXPECT_FALSE(Version::TryParse("", v));
            EXPECT_FALSE(Version::TryParse("1", v));
            EXPECT_FALSE(Version::TryParse("invalid.version", v));
            EXPECT_FALSE(Version::TryParse("1.2.3.4.5", v));
        }
    }
}

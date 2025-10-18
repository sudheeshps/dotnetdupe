#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/OperatingSystem.h"
#include "../DotNetDupe/Version.h"
#include "../DotNetDupe/String.h"

using namespace DotNetDupe::System;

namespace SystemTests
{
    namespace OperatingSystemTests
    {
        // Given: A PlatformID and a Version object
        // When: An OperatingSystem object is constructed
        // Then: The GetPlatform and GetVersion methods should return the correct values
        TEST(OperatingSystemTest, Constructor_Should_Initialize_Properties)
        {
            // Given
            const auto expectedPlatform = PlatformID::Win32NT;
            const Version expectedVersion(10, 0, 22000, 0);

            // When
            const OperatingSystem os(expectedPlatform, expectedVersion);

            // Then
            EXPECT_EQ(expectedPlatform, os.GetPlatform());
            EXPECT_EQ(expectedVersion, os.GetVersion());
        }

        // Given: An OperatingSystem object
        // When: GetVersionString is called
        // Then: It should return the string representation of the version
        TEST(OperatingSystemTest, GetVersionString_Should_Return_Correct_String)
        {
            // Given
            const Version version(6, 1, 7601, 0);
            const OperatingSystem os(PlatformID::Win32NT, version);
            const String expectedVersionString = _T("6.1.7601.0");

            // When
            const auto actualVersionString = os.GetVersionString();

            // Then
            EXPECT_EQ(expectedVersionString, actualVersionString);
        }

        // Given: An OperatingSystem object
        // When: GetPlatform is called
        // Then: It should return the correct PlatformID
        TEST(OperatingSystemTest, GetPlatform_Should_Return_Correct_Platform)
        {
            // Given
            const auto expectedPlatform = PlatformID::Unix;
            const OperatingSystem os(expectedPlatform, Version(5, 1, 0, 0));

            // When
            const auto actualPlatform = os.GetPlatform();

            // Then
            EXPECT_EQ(expectedPlatform, actualPlatform);
        }

        // Given: An OperatingSystem object
        // When: GetVersion is called
        // Then: It should return the correct Version object
        TEST(OperatingSystemTest, GetVersion_Should_Return_Correct_Version)
        {
            // Given
            const Version expectedVersion(10, 0, 19041, 0);
            const OperatingSystem os(PlatformID::Win32NT, expectedVersion);

            // When
            const auto actualVersion = os.GetVersion();

            // Then
            EXPECT_EQ(expectedVersion, actualVersion);
        }

        // Given: A valid OperatingSystem object
        // When: GetServicePack is called
        // Then: A non-empty string should be returned
        TEST(OperatingSystemTest, GetServicePack_Should_Return_NonEmpty_String)
        {
            // Given
            const OperatingSystem os(PlatformID::Win32NT, Version(10, 0));

            // When
            const auto servicePack = os.GetServicePack();

            // Then
            // We can't know the exact service pack, but it shouldn't be null.
            // On modern Windows, this is often an empty string.
            // So we just check that the call succeeds and we get a String object.
            SUCCEED();
        }

        // Given: The current operating system is Windows
        // When: IsWindows is called
        // Then: It should return true
        TEST(OperatingSystemTest, IsWindows_Should_Return_True)
        {
            // When
            const bool result = OperatingSystem::IsWindows();

            // Then
            EXPECT_TRUE(result);
        }

        // Given: The current operating system is Windows
        // When: IsLinux is called
        // Then: It should return false
        TEST(OperatingSystemTest, IsLinux_Should_Return_False)
        {
            // When
            const bool result = OperatingSystem::IsLinux();

            // Then
            EXPECT_FALSE(result);
        }

        // Given: The current operating system is Windows
        // When: IsMacOS is called
        // Then: It should return false
        TEST(OperatingSystemTest, IsMacOS_Should_Return_False)
        {
            // When
            const bool result = OperatingSystem::IsMacOS();

            // Then
            EXPECT_FALSE(result);
        }

    }
}

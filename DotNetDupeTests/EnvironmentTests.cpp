#include "pch.h"
#include "System/Environment.h"

using namespace DotNetDupe::System;
namespace SystemTests {
    namespace EnvironmentTests {
        TEST(Environment, GetCurrentDirectory_WhenCalled_ReturnsCurrentDirectory) {
            // Given

            // When
            auto currentDirectory = Environment::GetCurrentDirectory();

            // Then
            ASSERT_FALSE(currentDirectory.IsEmpty());
        }

        TEST(Environment, GetSystemDirectory_WhenCalled_ReturnsSystemDirectory) {
            // Given

            // When
            auto systemDirectory = Environment::GetSystemDirectory();

            // Then
            ASSERT_FALSE(systemDirectory.IsEmpty());
        }

        TEST(Environment, GetOSVersion_WhenCalled_ReturnsOSVersion) {
            // Given

            // When
            auto osVersion = Environment::GetOSVersion();

            // Then
            ASSERT_FALSE(osVersion.IsEmpty());
        }

        TEST(Environment, GetUserDomainName_WhenCalled_ReturnsUserDomainName) {
            // Given

            // When
            auto userDomainName = Environment::GetUserDomainName();

            // Then
            ASSERT_FALSE(userDomainName.IsEmpty());
        }

        TEST(Environment, GetVersion_WhenCalled_ReturnsVersion) {
            // Given

            // When
            auto version = Environment::GetVersion();

            // Then
            ASSERT_FALSE(version.IsEmpty());
        }

        TEST(Environment, GetWorkingSet_WhenCalled_ReturnsWorkingSet) {
            // Given

            // When
            auto workingSet = Environment::GetWorkingSet();

            // Then
            ASSERT_GT(workingSet, 0);
        }

        TEST(Environment, ExpandEnvironmentVariables_WhenCalled_ExpandsVariables) {
            // Given
#if defined(_WIN32)
            auto variable = "PATH";
            auto pattern = "%PATH%";
#else
            auto variable = "HOME";
            auto pattern = "%HOME%";
#endif
            auto expected = Environment::GetEnvironmentVariable(variable);

            // When
            auto expanded = Environment::ExpandEnvironmentVariables(pattern);

            // Then
            ASSERT_EQ(expected, expanded);
        }

        TEST(Environment, GetCommandLineArgs_WhenCalled_ReturnsCommandLineArgs) {
            // Given

            // When
            auto commandLineArgs = Environment::GetCommandLineArgs();

            // Then
            ASSERT_FALSE(commandLineArgs.IsNull());
            ASSERT_GT(commandLineArgs.GetLength(), 0);
        }

        TEST(Environment, GetEnvironmentVariable_WhenCalled_ReturnsEnvironmentVariable) {
            // Given
            auto variable = "DotNetDupe_TestVar";
            auto value = "TestValue";
            Environment::SetEnvironmentVariable(variable, value);

            // When
            auto result = Environment::GetEnvironmentVariable(variable);

            // Then
            ASSERT_EQ(result, value);
        }

        TEST(Environment, GetEnvironmentVariables_WhenCalled_ReturnsEnvironmentVariables) {
            // Given

            // When
            auto envVars = Environment::GetEnvironmentVariables();

            // Then
            ASSERT_GT(envVars.GetCount(), 0);
        }

        TEST(Environment, GetFolderPath_WhenCalled_ReturnsFolderPath) {
            // Given
#if defined(_WIN32)
            auto folder = Environment::SpecialFolder::System;
#else
            auto folder = Environment::SpecialFolder::UserProfile;
#endif

            // When
            auto folderPath = Environment::GetFolderPath(folder);

            // Then
            ASSERT_FALSE(folderPath.IsEmpty());
        }

        TEST(Environment, GetLogicalDrives_WhenCalled_ReturnsLogicalDrives) {
            // Given

            // When
            auto logicalDrives = Environment::GetLogicalDrives();

            // Then
            ASSERT_FALSE(logicalDrives.IsNull());
        }

        TEST(Environment, SetEnvironmentVariable_WhenCalled_SetsEnvironmentVariable) {
            // Given
            auto variable = "TestVar";
            auto value = "TestValue";

            // When
            Environment::SetEnvironmentVariable(variable, value);

            // Then
            auto result = Environment::GetEnvironmentVariable(variable);
            ASSERT_EQ(result, value);
        }

        TEST(Environment, GetOperatingSystem_WhenCalled_ReturnsOperatingSystem) {
            // Given

            // When
            auto os = Environment::GetOperatingSystem();

            // Then
#if defined(_WIN32)
            ASSERT_EQ(os.GetPlatform(), PlatformID::Win32NT);
#else
            ASSERT_EQ(os.GetPlatform(), PlatformID::Unix);
#endif
            ASSERT_TRUE(os.GetVersion().GetMajor() >= 0);
        }
    }
}
#include "pch.h"
#include "../DotNetDupe/Environment.h"

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
            auto path = Environment::GetEnvironmentVariable(_T("PATH"));

            // When
            auto expanded = Environment::ExpandEnvironmentVariables(_T("%PATH%"));

            // Then
            ASSERT_EQ(path, expanded);
        }

        TEST(Environment, GetCommandLineArgs_WhenCalled_ReturnsCommandLineArgs) {
            // Given

            // When
            auto commandLineArgs = Environment::GetCommandLineArgs();

            // Then
            ASSERT_FALSE(commandLineArgs.empty());
        }

        TEST(Environment, GetEnvironmentVariable_WhenCalled_ReturnsEnvironmentVariable) {
            // Given
            auto variable = _T("TestVar");
            auto value = _T("TestValue");
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
            ASSERT_FALSE(envVars.empty());
        }

        TEST(Environment, GetFolderPath_WhenCalled_ReturnsFolderPath) {
            // Given

            // When
            auto folderPath = Environment::GetFolderPath(Environment::SpecialFolder::System);

            // Then
            ASSERT_FALSE(folderPath.IsEmpty());
        }

        TEST(Environment, GetLogicalDrives_WhenCalled_ReturnsLogicalDrives) {
            // Given

            // When
            auto logicalDrives = Environment::GetLogicalDrives();

            // Then
            ASSERT_FALSE(logicalDrives.empty());
        }

        TEST(Environment, SetEnvironmentVariable_WhenCalled_SetsEnvironmentVariable) {
            // Given
            auto variable = _T("TestVar");
            auto value = _T("TestValue");

            // When
            Environment::SetEnvironmentVariable(variable, value);

            // Then
            auto result = Environment::GetEnvironmentVariable(variable);
            ASSERT_EQ(result, value);
        }
    }
}
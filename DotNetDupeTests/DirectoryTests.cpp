#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/Directory.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

TEST(DirectoryTests, GivenValidDirectoryPath_WhenCreateAndExists_ThenDirectoryCreatedAndExistsReturnsTrue) {
    // Given
    String sTestDir = Path::Combine({ Directory::GetCurrentDirectory(), "TestDir_CreateExists" });
    if (Directory::Exists(sTestDir)) {
        Directory::Delete(sTestDir, true);
    }

    // When
    Directory::CreateDirectory(sTestDir);

    // Then
    EXPECT_TRUE(Directory::Exists(sTestDir));

    // Clean up
    Directory::Delete(sTestDir, true);
    EXPECT_FALSE(Directory::Exists(sTestDir));
}

TEST(DirectoryTests, GivenDirectoryWithFiles_WhenGetFiles_ThenReturnsMatchingFileList) {
    // Given
    String sTestDir = Path::Combine({ Directory::GetCurrentDirectory(), "TestDir_GetFiles" });
    Directory::CreateDirectory(sTestDir);

    String sFile1 = Path::Combine({ sTestDir, "test1.txt" });
    String sFile2 = Path::Combine({ sTestDir, "test2.log" });
    File::WriteAllText(sFile1, "Content 1");
    File::WriteAllText(sFile2, "Content 2");

    // When
    Array<String> allFiles = Directory::GetFiles(sTestDir);
    Array<String> txtFiles = Directory::GetFiles(sTestDir, "*.txt");

    // Then
    EXPECT_EQ(allFiles.GetLength(), 2);
    EXPECT_EQ(txtFiles.GetLength(), 1);

    // Clean up
    Directory::Delete(sTestDir, true);
}

TEST(DirectoryTests, GivenNestedDirectories_WhenGetDirectories_ThenReturnsSubdirectories) {
    // Given
    String sTestDir = Path::Combine({ Directory::GetCurrentDirectory(), "TestDir_Subdirs" });
    String sSubDir1 = Path::Combine({ sTestDir, "Sub1" });
    String sSubDir2 = Path::Combine({ sTestDir, "Sub2" });
    Directory::CreateDirectory(sSubDir1);
    Directory::CreateDirectory(sSubDir2);

    // When
    Array<String> subdirs = Directory::GetDirectories(sTestDir);

    // Then
    EXPECT_EQ(subdirs.GetLength(), 2);

    // Clean up
    Directory::Delete(sTestDir, true);
}

TEST(DirectoryTests, GivenExistingDirectory_WhenMove_ThenDirectoryRenamed) {
    // Given
    String sSourceDir = Path::Combine({ Directory::GetCurrentDirectory(), "TestDir_SourceMove" });
    String sTargetDir = Path::Combine({ Directory::GetCurrentDirectory(), "TestDir_TargetMove" });
    if (Directory::Exists(sSourceDir)) Directory::Delete(sSourceDir, true);
    if (Directory::Exists(sTargetDir)) Directory::Delete(sTargetDir, true);

    Directory::CreateDirectory(sSourceDir);

    // When
    Directory::Move(sSourceDir, sTargetDir);

    // Then
    EXPECT_FALSE(Directory::Exists(sSourceDir));
    EXPECT_TRUE(Directory::Exists(sTargetDir));

    // Clean up
    Directory::Delete(sTargetDir, true);
}

TEST(DirectoryTests, GivenNonExistentDirectory_WhenDelete_ThenThrowsIOException) {
    // Given
    String sFakeDir = Path::Combine({ Directory::GetCurrentDirectory(), "NonExistentDir_12345" });

    // When / Then
    EXPECT_THROW(Directory::Delete(sFakeDir), IOException);
}

TEST(DirectoryTests, GivenEmptyPath_WhenCreateDirectory_ThenThrowsArgumentException) {
    // Given
    String sEmptyPath = "";

    // When / Then
    EXPECT_THROW(Directory::CreateDirectory(sEmptyPath), ArgumentException);
}

TEST(DirectoryTests, GivenNestedPath_WhenCreateDirectoryRecursiveTrue_ThenAllDirectoriesCreated) {
    // Given
    String sNestedDir = Path::Combine({ Directory::GetCurrentDirectory(), "RecursiveParent", "RecursiveChild", "DeepSub" });
    String sTopDir = Path::Combine({ Directory::GetCurrentDirectory(), "RecursiveParent" });
    if (Directory::Exists(sTopDir)) Directory::Delete(sTopDir, true);

    // When
    Directory::CreateDirectory(sNestedDir, true);

    // Then
    EXPECT_TRUE(Directory::Exists(sNestedDir));

    // Cleanup
    Directory::Delete(sTopDir, true);
}

TEST(DirectoryTests, GivenNonExistentParent_WhenCreateDirectoryRecursiveFalse_ThenThrowsIOException) {
    // Given
    String sNonExistentParent = Path::Combine({ Directory::GetCurrentDirectory(), "MissingParent_999" });
    String sNestedDir = Path::Combine({ sNonExistentParent, "Child" });
    if (Directory::Exists(sNonExistentParent)) Directory::Delete(sNonExistentParent, true);

    // When / Then
    EXPECT_THROW(Directory::CreateDirectory(sNestedDir, false), IOException);
}

#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/Directory.h"
#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/IO/SearchOption.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

TEST(SearchOptionTests, GivenNestedDirectories_WhenGetFilesRecursive_ThenReturnsAllFiles) {
    String sBaseDir = Path::Combine({ Directory::GetCurrentDirectory(), "Test_SearchOption_Dir" });
    String sSubDir = Path::Combine({ sBaseDir, "SubDir" });
    if (Directory::Exists(sBaseDir)) Directory::Delete(sBaseDir, true);
    Directory::CreateDirectory(sSubDir);

    File::WriteAllText(Path::Combine({ sBaseDir, "file1.txt" }), "content1");
    File::WriteAllText(Path::Combine({ sSubDir, "file2.txt" }), "content2");

    Array<String> topFiles = Directory::GetFiles(sBaseDir, "*.txt", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(topFiles.GetLength(), 1);

    Array<String> allFiles = Directory::GetFiles(sBaseDir, "*.txt", SearchOption::AllDirectories);
    EXPECT_EQ(allFiles.GetLength(), 2);

    Array<String> allEntries = Directory::GetFileSystemEntries(sBaseDir, "*", SearchOption::AllDirectories);
    EXPECT_GE(allEntries.GetLength(), 3);

    Directory::Delete(sBaseDir, true);
}

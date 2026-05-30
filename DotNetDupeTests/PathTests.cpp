#include "pch.h"
#include "gtest/gtest.h"
#include "System/String.h"
#include "System/Path.h"
#include <filesystem>
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
namespace fs = std::filesystem;
namespace SystemTests {
    namespace PathTests {
        TEST(PathTest, ChangeExtension_Should_Return_Path_With_Changed_Extension) {
            String path = "test.txt";
            path = Path::ChangeExtension(path, "csv");
            EXPECT_TRUE(path == "test.csv");
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_FileName_Passed) {
            ASSERT_THROW(Path::ChangeExtension("", "txt"), ArgumentException);
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_Extension_Passed) {
            ASSERT_THROW(Path::ChangeExtension("file", ""), ArgumentException);
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_FileNameOrExtension_Passed) {
            ASSERT_THROW(Path::ChangeExtension("fil", "txt"), ArgumentException);
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path) {
#if defined(_WIN32)
            auto combined = Path::Combine({ "C:\\Test","file.txt" });
            EXPECT_TRUE(combined == "C:\\Test\\file.txt");
#else
            auto combined = Path::Combine({ "/tmp/Test","file.txt" });
            EXPECT_TRUE(combined == "/tmp/Test/file.txt");
#endif
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components) {
#if defined(_WIN32)
            auto combined = Path::Combine({ "C:\\Test","folder", "file.txt" });
            EXPECT_TRUE(combined == "C:\\Test\\folder\\file.txt");
#else
            auto combined = Path::Combine({ "/tmp/Test","folder", "file.txt" });
            EXPECT_TRUE(combined == "/tmp/Test/folder/file.txt");
#endif
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components2) {
#if defined(_WIN32)
            auto combined = Path::Combine({ "C:\\Test","folder", "folder2" });
            EXPECT_TRUE(combined == "C:\\Test\\folder\\folder2");
#else
            auto combined = Path::Combine({ "/tmp/Test","folder", "folder2" });
            EXPECT_TRUE(combined == "/tmp/Test/folder/folder2");
#endif
        }
        TEST(PathTest, Combine_Should_Return_First_Path_When_First_Path_Is_Not_Rooted) {
#if defined(_WIN32)
            auto combined = Path::Combine({ "Test","file.txt" });
#else
            auto combined = Path::Combine({ "Test","file.txt" });
#endif
            EXPECT_TRUE(combined == "Test");
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_True) {
#if defined(_WIN32)
            EXPECT_TRUE(true == Path::EndsInDirectorySeparator("C:\\Test\\"));
#else
            EXPECT_TRUE(true == Path::EndsInDirectorySeparator("/tmp/Test/"));
#endif
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator("Test"));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False_When_Path_Invalid) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator("Test"));
        }
        TEST(PathTest, Exists_Should_Return_True_When_Folder_Path_Exists) {
#if defined(_WIN32)
            EXPECT_TRUE(true == Path::Exists("C:\\Windows"));
#else
            EXPECT_TRUE(true == Path::Exists("/etc"));
#endif
        }
        TEST(PathTest, Exists_Should_Return_True_When_File_Path_Exists) {
#if defined(_WIN32)
            EXPECT_TRUE(true == Path::Exists("C:\\Windows\\System32\\notepad.exe"));
#else
            EXPECT_TRUE(true == Path::Exists("/etc/passwd"));
#endif
        }
        TEST(PathTest, Exists_Should_Return_False_When_Path_Not_Exists) {
#if defined(_WIN32)
            EXPECT_TRUE(false == Path::Exists("C:\\dsfdfd"));
#else
            EXPECT_TRUE(false == Path::Exists("/tmp/dsfdfd"));
#endif
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_File_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test\\file.txt") == "C:\\Test");
#else
            EXPECT_TRUE(Path::GetDirectoryName("/tmp/Test/file.txt") == "/tmp/Test");
#endif
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_A_Sub_Directory) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test\\TestSub") == "C:\\Test");
#else
            EXPECT_TRUE(Path::GetDirectoryName("/tmp/Test/TestSub") == "/tmp/Test");
#endif
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Drive_Letter_For_A_Root_Folder) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test") == "C:\\");
#else
            EXPECT_TRUE(Path::GetDirectoryName("/tmp/Test") == "/tmp");
#endif
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetDirectoryName("") == "");
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Root_Drive_When_No_Parent) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetDirectoryName("C:\\") == "C:\\");
#else
            EXPECT_TRUE(Path::GetDirectoryName("/") == "/");
#endif
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_File_Path_Passed) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileName("C:\\Test\\file.txt") == "file.txt");
#else
            EXPECT_TRUE(Path::GetFileName("/tmp/Test/file.txt") == "file.txt");
#endif
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_Valid_Path_Passed) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileName("C:\\Test") == "Test");
#else
            EXPECT_TRUE(Path::GetFileName("/tmp/Test") == "Test");
#endif
        }
        TEST(PathTest, GetFileName_Should_Return_FileName_Part_When_File_Name_With_Only_Extension) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileName("C:\\Test\\.txt") == ".txt");
#else
            EXPECT_TRUE(Path::GetFileName("/tmp/Test/.txt") == ".txt");
#endif
        }
        TEST(PathTest, GetFileName_Should_Return_Single_Dot_When_Single_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName(".") == ".");
        }
        TEST(PathTest, GetFileName_Should_Return_Double_Dot_When_Double_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName("..") == "..");
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Root_Passed) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileName("C:\\") == "");
#else
            EXPECT_TRUE(Path::GetFileName("/") == "");
#endif
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileName("") == "");
        }
        TEST(PathTest, GetExtension_Should_Return_Extension_When_File_Path_Passed) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetExtension("C:\\Test\\file.txt") == "txt");
#else
            EXPECT_TRUE(Path::GetExtension("/tmp/Test/file.txt") == "txt");
#endif
        }
        TEST(PathTest, GetExtension_Should_Return_Same_String_When_Folder_Path_Passed) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetExtension("C:\\Test") == "");
#else
            EXPECT_TRUE(Path::GetExtension("/tmp/Test") == "");
#endif
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("C:\\file.txt") == "file");
#else
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("/tmp/file.txt") == "file");
#endif
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension2) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("C:\\file") == "file");
#else
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("/tmp/file") == "file");
#endif
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("") == "");
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_File_Name_Passed) {
#if defined(_WIN32)
            fs::current_path("C:\\Windows\\System32");
            EXPECT_TRUE(Path::GetFullPath("notepad.exe") == "C:\\Windows\\System32\\notepad.exe");
#else
            fs::current_path("/usr/bin");
            EXPECT_TRUE(Path::GetFullPath("ls") == "/usr/bin/ls");
#endif
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_Folder_Name_Passed) {
#if defined(_WIN32)
            fs::current_path("C:\\Windows");
            EXPECT_TRUE(Path::GetFullPath("System32") == "C:\\Windows\\System32");
#else
            fs::current_path("/usr");
            EXPECT_TRUE(Path::GetFullPath("bin") == "/usr/bin");
#endif
        }
        TEST(PathTest, GetInvalidFileNameChars_Should_Return_Vector_Of_Correct_Size) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            EXPECT_EQ(41, invalidChars.GetLength());
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Contain_Invalid_Special_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            std::vector<char> specialChars = { '<', '>', ':', '"', '/', '\\', '|', '?', '*' };
            for (auto specialChar : specialChars) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), specialChar), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Contain_Invalid_Control_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            for (int i = 0; i <= 31; ++i) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), static_cast<char>(i)), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Not_Contain_Valid_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            std::vector<char> validChars = { 'a', 'b', 'c', '1', '2', '3', '.' };
            for (auto validChar : validChars) {
                EXPECT_EQ(std::find(invalidChars.begin(), invalidChars.end(), validChar), invalidChars.end());
            }
        }
        TEST(PathTest, GetInvalidPathChars_Should_Return_Vector_Of_Correct_Size) {
            auto invalidChars = Path::GetInvalidPathChars();
            EXPECT_EQ(33, invalidChars.GetLength());
        }

        TEST(PathTest, GetInvalidPathChars_Should_Contain_Pipe_Char) {
            auto invalidChars = Path::GetInvalidPathChars();
            EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), '|'), invalidChars.end());
        }

        TEST(PathTest, GetInvalidPathChars_Should_Contain_Invalid_Control_Chars) {
            auto invalidChars = Path::GetInvalidPathChars();
            for (int i = 0; i <= 31; ++i) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), static_cast<char>(i)), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidPathChars_Should_Not_Contain_Valid_Chars) {
            auto invalidChars = Path::GetInvalidPathChars();
            std::vector<char> validChars = { 'a', 'b', 'c', '1', '2', '3', '.', '\\', '/' };
            for (auto validChar : validChars) {
                EXPECT_EQ(std::find(invalidChars.begin(), invalidChars.end(), validChar), invalidChars.end());
            }
        }

        TEST(PathTest, GetPathRoot_Should_Return_Drive_Root) {
#if defined(_WIN32)
            String path = "C:\\folder\\file.txt";
            String expectedRoot = "C:\\";
#else
            String path = "/tmp/folder/file.txt";
            String expectedRoot = "/";
#endif
            String actualRoot = Path::GetPathRoot(path);
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root) {
#if defined(_WIN32)
            String path = "\\Server\\Share\\folder\\file.txt";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "\\Server\\Share";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
#else
            // UNC is Windows-specific concept in this library's current implementation
            SUCCEED();
#endif
        }

        TEST(PathTest, GetPathRoot_Should_Return_Volume_Separator_Root) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetPathRoot("C:\\") == "C:\\");
#else
            EXPECT_TRUE(Path::GetPathRoot("/") == "/");
#endif
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Relative_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::GetPathRoot("folder\\file.txt") == "");
#else
            EXPECT_TRUE(Path::GetPathRoot("folder/file.txt") == "");
#endif
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Empty_Path) {
            EXPECT_TRUE(Path::GetPathRoot("") == "");
        }

        TEST(PathTest, GetPathRoot_Should_Return_Drive_Root_With_Trailing_Slash) {
#if defined(_WIN32)
            String path = "D:\\";
            String expectedRoot = "D:\\";
#else
            String path = "/";
            String expectedRoot = "/";
#endif
            String actualRoot = Path::GetPathRoot(path);
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root_With_Trailing_Slash) {
#if defined(_WIN32)
            String path = "\\Server\\Share\\";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "\\Server\\Share";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
#else
            SUCCEED();
#endif
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_File_Name_Only) {
            EXPECT_TRUE(Path::GetPathRoot("file.txt") == "");
        }

        // Given: A request for a random file name
        // When: GetRandomFileName is called
        // Then: The returned string should not be empty
        TEST(PathTest, GetRandomFileName_WhenCalled_ReturnsNonEmptyString) {
            // Act
            String randomFileName = Path::GetRandomFileName();

            // Assert
            EXPECT_FALSE(randomFileName.IsEmpty());
        }

        // Given: A request for a random file name
        // When: GetRandomFileName is called
        // Then: The returned string should have a length of 12
        TEST(PathTest, GetRandomFileName_WhenCalled_ReturnsStringOfLength12) {
            // Act
            String randomFileName = Path::GetRandomFileName();

            // Assert
            EXPECT_EQ(12, randomFileName.GetLength());
        }

        // Given: A request for a random file name
        // When: GetRandomFileName is called
        // Then: The returned string should have a dot at index 8
        TEST(PathTest, GetRandomFileName_WhenCalled_ReturnsStringWithDotAtIndex8) {
            // Act
            String randomFileName = Path::GetRandomFileName();

            // Assert
            EXPECT_EQ('.', randomFileName [8]);
        }

        // Given: A request for a random file name
        // When: GetRandomFileName is called
        // Then: The returned string should contain only valid characters
        TEST(PathTest, GetRandomFileName_WhenCalled_ReturnsStringWithValidChars) {
            // Arrange
            const String validChars = "abcdefghijklmnopqrstuvwxyz0123456789.";

            // Act
            String randomFileName = Path::GetRandomFileName();

            // Assert
            for (int i = 0; i < randomFileName.GetLength(); ++i) {
                EXPECT_TRUE(validChars.Contains(randomFileName [i]));
            }
        }

        // Negative tests for GetRandomFileName are not applicable as the method does not take any input
        // and is designed to always return a valid 8.3 format file name.
        TEST(PathTest, GetRelativePath_should_return_the_relative_path) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\", "C:\\Users\\Test");
            EXPECT_TRUE(result == "Users\\Test");
#else
            auto result = Path::GetRelativePath("/", "/Users/Test");
            EXPECT_TRUE(result == "Users/Test");
#endif
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_paths_are_the_same) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\Users\\Test", "C:\\Users\\Test");
#else
            auto result = Path::GetRelativePath("/Users/Test", "/Users/Test");
#endif
            EXPECT_TRUE(result == ".");
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_a_subdirectory) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\Users", "C:\\Users\\Test\\file.txt");
            EXPECT_TRUE(result == "Test\\file.txt");
#else
            auto result = Path::GetRelativePath("/Users", "/Users/Test/file.txt");
            EXPECT_TRUE(result == "Test/file.txt");
#endif
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_in_a_different_directory) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\Users\\Test1", "C:\\Users\\Test2");
            EXPECT_TRUE(result == "..\\Test2");
#else
            auto result = Path::GetRelativePath("/Users/Test1", "/Users/Test2");
            EXPECT_TRUE(result == "../Test2");
#endif
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_in_a_parent_directory) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\Users\\Test\\Sub", "C:\\Users\\Test");
#else
            auto result = Path::GetRelativePath("/Users/Test/Sub", "/Users/Test");
#endif
            EXPECT_TRUE(result == "..");
        }

        TEST(PathTest, GetRelativePath_should_return_the_path_when_drives_are_different) {
#if defined(_WIN32)
            auto result = Path::GetRelativePath("C:\\Users", "D:\\Users");
            EXPECT_TRUE(result == "D:\\Users");
#else
            // No drives on Linux
            SUCCEED();
#endif
        }

        TEST(PathTest, GetTempFileName_ReturnsNonEmptyPath) {
            String tempFile = Path::GetTempFileName();
            EXPECT_FALSE(tempFile.IsEmpty());
            fs::remove((const char*)tempFile);
        }

        TEST(PathTest, GetTempFileName_CreatesFile) {
            String tempFile = Path::GetTempFileName();
            EXPECT_TRUE(fs::exists((const char*)tempFile));
            fs::remove((const char*)tempFile);
        }

        TEST(PathTest, GetTempFileName_HasTmpExtension) {
            String tempFile = Path::GetTempFileName();
            EXPECT_TRUE(tempFile.EndsWith(".tmp", false));
            fs::remove((const char*)tempFile);
        }

        TEST(PathTest, GetTempFileName_IsInTempDirectory) {
            String tempFile = Path::GetTempFileName();
            String tempDir = Path::GetTempPath();
            EXPECT_TRUE(tempFile.StartsWith(tempDir, false));
            fs::remove((const char*)tempFile);
        }

        TEST(PathTest, GetTempFileName_ReturnsUniqueNames) {
            String tempFile1 = Path::GetTempFileName();
            String tempFile2 = Path::GetTempFileName();
            EXPECT_NE(tempFile1, tempFile2);
            fs::remove((const char*)tempFile1);
            fs::remove((const char*)tempFile2);
        }

        TEST(PathTest, GetTempPath_ReturnsNonEmptyPath) {
            String tempPath = Path::GetTempPath();
            EXPECT_FALSE(tempPath.IsEmpty());
        }

        TEST(PathTest, GetTempPath_ReturnsExistingDirectory) {
            String tempPath = Path::GetTempPath();
            EXPECT_TRUE(fs::exists((const char*)tempPath));
            EXPECT_TRUE(fs::is_directory((const char*)tempPath));
        }

        TEST(PathTest, GetTempPath_EndsWithDirectorySeparator) {
            String tempPath = Path::GetTempPath();
            EXPECT_TRUE(Path::EndsInDirectorySeparator(tempPath));
        }

        TEST(PathTest, HasExtension_Should_Return_True_When_Path_Has_Extension) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::HasExtension("C:\\file.txt"));
#else
            EXPECT_TRUE(Path::HasExtension("/tmp/file.txt"));
#endif
        }

        TEST(PathTest, HasExtension_Should_Return_False_When_Path_Has_No_Extension) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::HasExtension("C:\\file"));
#else
            EXPECT_FALSE(Path::HasExtension("/tmp/file"));
#endif
        }

        TEST(PathTest, HasExtension_Should_Return_False_When_Path_Ends_With_Dot) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::HasExtension("C:\\file."));
#else
            EXPECT_FALSE(Path::HasExtension("/tmp/file."));
#endif
        }

        TEST(PathTest, HasExtension_Should_Return_True_When_FileName_Starts_With_Dot) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::HasExtension("C:\\.gitignore"));
#else
            EXPECT_TRUE(Path::HasExtension("/tmp/.gitignore"));
#endif
        }

        TEST(PathTest, HasExtension_Should_Return_False_For_Directory_Path) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::HasExtension("C:\\directory\\"));
#else
            EXPECT_FALSE(Path::HasExtension("/tmp/directory/"));
#endif
        }

        TEST(PathTest, HasExtension_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::HasExtension(""));
        }

        TEST(PathTest, HasExtension_Should_Return_True_For_Path_With_Multiple_Dots) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::HasExtension("C:\\file.name.txt"));
#else
            EXPECT_TRUE(Path::HasExtension("/tmp/file.name.txt"));
#endif
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True_For_Absolute_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathFullyQualified("C:\\folder\\file.txt"));
#else
            EXPECT_TRUE(Path::IsPathFullyQualified("/folder/file.txt"));
#endif
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True_For_UNC_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathFullyQualified("\\\\server\\share\\file.txt"));
#else
            // UNC is Windows-specific
            SUCCEED();
#endif
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Relative_Path) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::IsPathFullyQualified("folder\\file.txt"));
#else
            EXPECT_FALSE(Path::IsPathFullyQualified("folder/file.txt"));
#endif
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_FileName_Only) {
            EXPECT_FALSE(Path::IsPathFullyQualified("file.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::IsPathFullyQualified(""));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Drive_Letter_Only) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::IsPathFullyQualified("C:"));
#else
            SUCCEED();
#endif
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True__For_Root_Directory) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathFullyQualified("C:\\"));
#else
            EXPECT_TRUE(Path::IsPathFullyQualified("/"));
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Absolute_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathRooted("C:\\folder\\file.txt"));
#else
            EXPECT_TRUE(Path::IsPathRooted("/folder/file.txt"));
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_UNC_Path) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathRooted("\\\\server\\share\\file.txt"));
#else
            SUCCEED();
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Path_Starting_With_Separator) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathRooted("\\folder\\file.txt"));
#else
            EXPECT_TRUE(Path::IsPathRooted("/folder/file.txt"));
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Relative_Path) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::IsPathRooted("folder\\file.txt"));
#else
            EXPECT_FALSE(Path::IsPathRooted("folder/file.txt"));
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_FileName_Only) {
            EXPECT_FALSE(Path::IsPathRooted("file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::IsPathRooted(""));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Drive_Letter_Only) {
#if defined(_WIN32)
            EXPECT_FALSE(Path::IsPathRooted("C:"));
#else
            SUCCEED();
#endif
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Root_Directory) {
#if defined(_WIN32)
            EXPECT_TRUE(Path::IsPathRooted("C:\\"));
#else
            EXPECT_TRUE(Path::IsPathRooted("/"));
#endif
        }

        TEST(PathTest, Join_Should_Return_Joined_Path) {
#if defined(_WIN32)
            auto joined = Path::Join({ "C:\\Test", "file.txt" });
            EXPECT_TRUE(joined == "C:\\Test\\file.txt");
#else
            auto joined = Path::Join({ "/tmp/Test", "file.txt" });
            EXPECT_TRUE(joined == "/tmp/Test/file.txt");
#endif
        }

        TEST(PathTest, Join_Should_Return_Joined_Path_For_Multiple_Components) {
#if defined(_WIN32)
            auto joined = Path::Join({ "C:\\Test", "folder", "file.txt" });
            EXPECT_TRUE(joined == "C:\\Test\\folder\\file.txt");
#else
            auto joined = Path::Join({ "/tmp/Test", "folder", "file.txt" });
            EXPECT_TRUE(joined == "/tmp/Test/folder/file.txt");
#endif
        }

        TEST(PathTest, Join_Should_Throw_Exception_For_Invalid_Chars) {
#if defined(_WIN32)
            ASSERT_THROW(Path::Join({ "C:\\Test", "file|name.txt" }), ArgumentException);
#else
            // | is valid on Linux, but let's see what GetInvalidFileNameChars says
            SUCCEED();
#endif
        }

        TEST(PathTest, TryJoin_Should_Return_False_For_Invalid_Chars) {
#if defined(_WIN32)
            String result("");
            EXPECT_FALSE(Path::TryJoin({ "C:\\Test", "file|name.txt" }, result));
#else
            SUCCEED();
#endif
        }

        TEST(PathTest, TryJoin_Should_Return_True_For_Valid_Path) {
            String result("");
#if defined(_WIN32)
            EXPECT_TRUE(Path::TryJoin({ "C:\\Test", "file.txt" }, result));
            EXPECT_TRUE(result == "C:\\Test\\file.txt");
#else
            EXPECT_TRUE(Path::TryJoin({ "/tmp/Test", "file.txt" }, result));
            EXPECT_TRUE(result == "/tmp/Test/file.txt");
#endif
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Remove_Trailing_Separator) {
#if defined(_WIN32)
            String path = "C:\\Test\\";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "C:\\Test");
#else
            String path = "/tmp/Test/";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "/tmp/Test");
#endif
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Do_Nothing_If_No_Trailing_Separator) {
#if defined(_WIN32)
            String path = "C:\\Test";
#else
            String path = "/tmp/Test";
#endif
            path = Path::TrimEndingDirectorySeparator(path);
#if defined(_WIN32)
            EXPECT_TRUE(path == "C:\\Test");
#else
            EXPECT_TRUE(path == "/tmp/Test");
#endif
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Not_Trim_Root_Path) {
#if defined(_WIN32)
            String path = "C:\\";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "C:\\");
#else
            String path = "/";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "/");
#endif
        }
    }
}
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
            auto combined = Path::Combine({ "C:\\Test","file.txt" });
            EXPECT_TRUE(combined == "C:\\Test\\file.txt");
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components) {
            auto combined = Path::Combine({ "C:\\Test","folder", "file.txt" });
            std::cout << "Combined: " << combined.GetRawString();
            EXPECT_TRUE(combined == "C:\\Test\\folder\\file.txt");
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components2) {
            auto combined = Path::Combine({ "C:\\Test","folder", "folder2" });
            EXPECT_TRUE(combined == "C:\\Test\\folder\\folder2");
        }
        TEST(PathTest, Combine_Should_Return_First_Path_When_First_Path_Is_Not_Rooted) {
            auto combined = Path::Combine({ "Test","file.txt" });
            EXPECT_TRUE(combined == "Test");
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_True) {
            EXPECT_TRUE(true == Path::EndsInDirectorySeparator("C:\\Test\\"));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator("Test"));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False_When_Path_Invalid) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator("Test"));
        }
        TEST(PathTest, Exists_Should_Return_True_When_Folder_Path_Exists) {
            EXPECT_TRUE(true == Path::Exists("C:\\Windows"));
        }
        TEST(PathTest, Exists_Should_Return_True_When_File_Path_Exists) {
            EXPECT_TRUE(true == Path::Exists("C:\\Windows\\System32\\notepad.exe"));
        }
        TEST(PathTest, Exists_Should_Return_False_When_Path_Not_Exists) {
            EXPECT_TRUE(false == Path::Exists("C:\\dsfdfd"));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_File_Path) {
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test\\file.txt") == "C:\\Test");
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_A_Sub_Directory) {
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test\\TestSub") == "C:\\Test");
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Drive_Letter_For_A_Root_Folder) {
            EXPECT_TRUE(Path::GetDirectoryName("C:\\Test") == "C:\\");
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetDirectoryName("") == "");
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Root_Drive_When_No_Parent) {
            EXPECT_TRUE(Path::GetDirectoryName("C:\\") == "C:\\");
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_File_Path_Passed) {
            EXPECT_TRUE(Path::GetFileName("C:\\Test\\file.txt") == "file.txt");
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_Valid_Path_Passed) {
            EXPECT_TRUE(Path::GetFileName("C:\\Test") == "Test");
        }
        TEST(PathTest, GetFileName_Should_Return_FileName_Part_When_File_Name_With_Only_Extension) {
            EXPECT_TRUE(Path::GetFileName("C:\\Test\\.txt") == ".txt");
        }
        TEST(PathTest, GetFileName_Should_Return_Single_Dot_When_Single_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName(".") == ".");
        }
        TEST(PathTest, GetFileName_Should_Return_Double_Dot_When_Double_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName("..") == "..");
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Root_Passed) {
            EXPECT_TRUE(Path::GetFileName("C:\\") == "");
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileName("") == "");
        }
        TEST(PathTest, GetExtension_Should_Return_Extension_When_File_Path_Passed) {
            EXPECT_TRUE(Path::GetExtension("C:\\Test\\file.txt") == "txt");
        }
        TEST(PathTest, GetExtension_Should_Return_Same_String_When_Folder_Path_Passed) {
            EXPECT_TRUE(Path::GetExtension("C:\\Test") == "");
        }
        TEST(PathTest, GetExtension_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetExtension("") == "");
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("C:\\file.txt") == "file");
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension2) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("C:\\file") == "file");
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension("") == "");
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_File_Name_Passed) {
            std::filesystem::current_path("C:\\Windows\\System32");
            EXPECT_TRUE(Path::GetFullPath("notepad.exe") == "C:\\Windows\\System32\\notepad.exe");
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_Folder_Name_Passed) {
            std::filesystem::current_path("C:\\Windows");
            EXPECT_TRUE(Path::GetFullPath("System32") == "C:\\Windows\\System32");
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
            String path = "C:\\folder\\file.txt";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "C:\\";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root) {
            String path = "\\Server\\Share\\folder\\file.txt";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "\\Server\\Share";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
        }

        TEST(PathTest, GetPathRoot_Should_Return_Volume_Separator_Root) {
            EXPECT_TRUE(Path::GetPathRoot("C:\\") == "C:\\");
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Relative_Path) {
            EXPECT_TRUE(Path::GetPathRoot("folder\\file.txt") == "");
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Empty_Path) {
            EXPECT_TRUE(Path::GetPathRoot("") == "");
        }

        TEST(PathTest, GetPathRoot_Should_Return_Drive_Root_With_Trailing_Slash) {
            String path = "D:\\";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "D:\\";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root_With_Trailing_Slash) {
            String path = "\\Server\\Share\\";
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = "\\Server\\Share";
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot;
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
            auto result = Path::GetRelativePath("C:\\", "C:\\Users\\Test");
            EXPECT_TRUE(result == "Users\\Test");
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_paths_are_the_same) {
            auto result = Path::GetRelativePath("C:\\Users\\Test", "C:\\Users\\Test");
            EXPECT_TRUE(result == ".");
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_a_subdirectory) {
            auto result = Path::GetRelativePath("C:\\Users", "C:\\Users\\Test\\file.txt");
            EXPECT_TRUE(result == "Test\\file.txt");
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_in_a_different_directory) {
            auto result = Path::GetRelativePath("C:\\Users\\Test1", "C:\\Users\\Test2");
            EXPECT_TRUE(result == "..\\Test2");
        }

        TEST(PathTest, GetRelativePath_should_return_the_relative_path_when_path_is_in_a_parent_directory) {
            auto result = Path::GetRelativePath("C:\\Users\\Test\\Sub", "C:\\Users\\Test");
            EXPECT_TRUE(result == "..");
        }

        TEST(PathTest, GetRelativePath_should_return_the_path_when_drives_are_different) {
            auto result = Path::GetRelativePath("C:\\Users", "D:\\Users");
            EXPECT_TRUE(result == "D:\\Users");
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
            EXPECT_TRUE(Path::HasExtension("C:\\file.txt"));
        }

        TEST(PathTest, HasExtension_Should_Return_False_When_Path_Has_No_Extension) {
            EXPECT_FALSE(Path::HasExtension("C:\\file"));
        }

        TEST(PathTest, HasExtension_Should_Return_False_When_Path_Ends_With_Dot) {
            EXPECT_FALSE(Path::HasExtension("C:\\file."));
        }

        TEST(PathTest, HasExtension_Should_Return_True_When_FileName_Starts_With_Dot) {
            EXPECT_TRUE(Path::HasExtension("C:\\.gitignore"));
        }

        TEST(PathTest, HasExtension_Should_Return_False_For_Directory_Path) {
            EXPECT_FALSE(Path::HasExtension("C:\\directory\\"));
        }

        TEST(PathTest, HasExtension_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::HasExtension(""));
        }

        TEST(PathTest, HasExtension_Should_Return_True_For_Path_With_Multiple_Dots) {
            EXPECT_TRUE(Path::HasExtension("C:\\file.name.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True_For_Absolute_Path) {
            EXPECT_TRUE(Path::IsPathFullyQualified("C:\\folder\\file.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True_For_UNC_Path) {
            EXPECT_TRUE(Path::IsPathFullyQualified("\\\\server\\share\\file.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Relative_Path) {
            EXPECT_FALSE(Path::IsPathFullyQualified("folder\\file.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_FileName_Only) {
            EXPECT_FALSE(Path::IsPathFullyQualified("file.txt"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::IsPathFullyQualified(""));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_False_For_Drive_Letter_Only) {
            EXPECT_FALSE(Path::IsPathFullyQualified("C:"));
        }

        TEST(PathTest, IsPathFullyQualified_Should_Return_True__For_Root_Directory) {
            EXPECT_TRUE(Path::IsPathFullyQualified("C:\\"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Absolute_Path) {
            EXPECT_TRUE(Path::IsPathRooted("C:\\folder\\file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_UNC_Path) {
            EXPECT_TRUE(Path::IsPathRooted("\\\\server\\share\\file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Path_Starting_With_Separator) {
            EXPECT_TRUE(Path::IsPathRooted("\\folder\\file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Relative_Path) {
            EXPECT_FALSE(Path::IsPathRooted("folder\\file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_FileName_Only) {
            EXPECT_FALSE(Path::IsPathRooted("file.txt"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Empty_String) {
            EXPECT_FALSE(Path::IsPathRooted(""));
        }

        TEST(PathTest, IsPathRooted_Should_Return_False_For_Drive_Letter_Only) {
            EXPECT_FALSE(Path::IsPathRooted("C:"));
        }

        TEST(PathTest, IsPathRooted_Should_Return_True_For_Root_Directory) {
            EXPECT_TRUE(Path::IsPathRooted("C:\\"));
        }

        TEST(PathTest, Join_Should_Return_Joined_Path) {
            auto joined = Path::Join({ "C:\\Test", "file.txt" });
            EXPECT_TRUE(joined == "C:\\Test\\file.txt");
        }

        TEST(PathTest, Join_Should_Return_Joined_Path_For_Multiple_Components) {
            auto joined = Path::Join({ "C:\\Test", "folder", "file.txt" });
            EXPECT_TRUE(joined == "C:\\Test\\folder\\file.txt");
        }

        TEST(PathTest, Join_Should_Throw_Exception_For_Invalid_Chars) {
            ASSERT_THROW(Path::Join({ "C:\\Test", "file|name.txt" }), ArgumentException);
        }

        TEST(PathTest, TryJoin_Should_Return_False_For_Invalid_Chars) {
            String result("");
            EXPECT_FALSE(Path::TryJoin({ "C:\\Test", "file|name.txt" }, result));
        }

        TEST(PathTest, TryJoin_Should_Return_True_For_Valid_Path) {
            String result("");
            EXPECT_TRUE(Path::TryJoin({ "C:\\Test", "file.txt" }, result));
            EXPECT_TRUE(result == "C:\\Test\\file.txt");
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Remove_Trailing_Separator) {
            String path = "C:\\Test\\";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "C:\\Test");
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Do_Nothing_If_No_Trailing_Separator) {
            String path = "C:\\Test";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "C:\\Test");
        }

        TEST(PathTest, TrimEndingDirectorySeparator_Should_Not_Trim_Root_Path) {
            String path = "C:\\";
            path = Path::TrimEndingDirectorySeparator(path);
            EXPECT_TRUE(path == "C:\\");
        }
    }
}
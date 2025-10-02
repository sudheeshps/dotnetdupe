#include "pch.h"
#include "gtest/gtest.h"
#include "../DotNetDupe/String.h"
#include "../DotNetDupe/Path.h"
#include <filesystem>
using namespace DotNetDupe::System::IO;
namespace SystemTests {
    namespace PathTests {
        TEST(PathTest, ChangeExtension_Should_Return_Path_With_Changed_Extension) {
            String path = _T("test.txt");
            path = Path::ChangeExtension(path, _T("csv"));
            EXPECT_TRUE(path == _T("test.csv"));
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_FileName_Passed) {
            ASSERT_THROW(Path::ChangeExtension(_T(""), _T("txt")), ArgumentException);
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_Extension_Passed) {
            ASSERT_THROW(Path::ChangeExtension(_T("file"), _T("")), ArgumentException);
        }
        TEST(PathTest, ChangeExtension_Should_Throw_Exception_When_Invalid_FileNameOrExtension_Passed) {
            ASSERT_THROW(Path::ChangeExtension(_T("fil"), _T("txt")), ArgumentException);
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path) {
            auto combined = Path::Combine({ _T("C:\\Test"),_T("file.txt") });
            EXPECT_TRUE(combined == _T("C:\\Test\\file.txt"));
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components) {
            auto combined = Path::Combine({ _T("C:\\Test"),_T("folder"), _T("file.txt")});
            std::wcout << L"Combined: " << combined.GetString();
            EXPECT_TRUE(combined == _T("C:\\Test\\folder\\file.txt"));
        }
        TEST(PathTest, Combine_Should_Return_Combined_Path_For_Multiple_Components2) {
            auto combined = Path::Combine({ _T("C:\\Test"),_T("folder"), _T("folder2") });
            EXPECT_TRUE(combined == _T("C:\\Test\\folder\\folder2"));
        }
        TEST(PathTest, Combine_Should_Return_First_Path_When_First_Path_Is_Not_Rooted) {
            auto combined = Path::Combine({ _T("Test"),_T("file.txt") });
            EXPECT_TRUE(combined == _T("Test"));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_True) {
            EXPECT_TRUE(true == Path::EndsInDirectorySeparator(_T("C:\\Test\\")));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator(_T("Test")));
        }
        TEST(PathTest, EndsInDirectorySeparator_Should_Return_False_When_Path_Invalid) {
            EXPECT_TRUE(false == Path::EndsInDirectorySeparator(_T("Test")));
        }
        TEST(PathTest, Exists_Should_Return_True_When_Folder_Path_Exists) {
            EXPECT_TRUE(true == Path::Exists(_T("C:\\Windows")));
        }
        TEST(PathTest, Exists_Should_Return_True_When_File_Path_Exists) {
            EXPECT_TRUE(true == Path::Exists(_T("C:\\Windows\\System32\\notepad.exe")));
        }
        TEST(PathTest, Exists_Should_Return_False_When_Path_Not_Exists) {
            EXPECT_TRUE(false == Path::Exists(_T("C:\\dsfdfd")));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_File_Path) {
            EXPECT_TRUE(Path::GetDirectoryName(_T("C:\\Test\\file.txt")) == _T("C:\\Test"));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Parent_Directory_For_A_Sub_Directory) {
            EXPECT_TRUE(Path::GetDirectoryName(_T("C:\\Test\\TestSub")) == _T("C:\\Test"));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Drive_Letter_For_A_Root_Folder) {
            EXPECT_TRUE(Path::GetDirectoryName(_T("C:\\Test")) == _T("C:\\"));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetDirectoryName(_T("")) == _T(""));
        }
        TEST(PathTest, GetDirectoryName_Should_Return_Root_Drive_When_No_Parent) {
            EXPECT_TRUE(Path::GetDirectoryName(_T("C:\\")) == _T("C:\\"));
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_File_Path_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T("C:\\Test\\file.txt")) == _T("file.txt"));
        }
        TEST(PathTest, GetFileName_Should_Return_File_Name_When_Valid_Path_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T("C:\\Test")) == _T("Test"));
        }
        TEST(PathTest, GetFileName_Should_Return_FileName_Part_When_File_Name_With_Only_Extension) {
            EXPECT_TRUE(Path::GetFileName(_T("C:\\Test\\.txt")) == _T(".txt"));
        }
        TEST(PathTest, GetFileName_Should_Return_Single_Dot_When_Single_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T(".")) == _T("."));
        }
        TEST(PathTest, GetFileName_Should_Return_Double_Dot_When_Double_Dot_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T("..")) == _T(".."));
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Root_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T("C:\\")) == _T(""));
        }
        TEST(PathTest, GetFileName_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileName(_T("")) == _T(""));
        }
        TEST(PathTest, GetExtension_Should_Return_Extension_When_File_Path_Passed) {
            EXPECT_TRUE(Path::GetExtension(_T("C:\\Test\\file.txt")) == _T("txt"));
        }
        TEST(PathTest, GetExtension_Should_Return_Same_String_When_Folder_Path_Passed) {
            EXPECT_TRUE(Path::GetExtension(_T("C:\\Test")) == _T(""));
        }
        TEST(PathTest, GetExtension_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetExtension(_T("")) == _T(""));
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension(_T("C:\\file.txt")) == _T("file"));
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_File_Name_With_No_Extension2) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension(_T("C:\\file")) == _T("file"));
        }
        TEST(PathTest, GetFileNameWithoutExtension_Should_Return_Empty_String_When_Empty_String_Passed) {
            EXPECT_TRUE(Path::GetFileNameWithoutExtension(_T("")) == _T(""));
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_File_Name_Passed) {
            std::filesystem::current_path(_T("C:\\Windows\\System32"));
            EXPECT_TRUE(Path::GetFullPath(_T("notepad.exe")) == _T("C:\\Windows\\System32\\notepad.exe"));
        }
        TEST(PathTest, GetFullPath_Should_Return_Absoulte_Path_When_Folder_Name_Passed) {
            std::filesystem::current_path(_T("C:\\Windows"));
            EXPECT_TRUE(Path::GetFullPath(_T("System32")) == _T("C:\\Windows\\System32"));
        }
        TEST(PathTest, GetInvalidFileNameChars_Should_Return_Vector_Of_Correct_Size) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            EXPECT_EQ(41, invalidChars.size());
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Contain_Invalid_Special_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            std::vector<TCHAR> specialChars = {'<', '>', ':', '"', '/', '\\', '|', '?', '*'};
            for (auto specialChar : specialChars) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), specialChar), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Contain_Invalid_Control_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            for (int i = 0; i <= 31; ++i) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), static_cast<TCHAR>(i)), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidFileNameChars_Should_Not_Contain_Valid_Chars) {
            auto invalidChars = Path::GetInvalidFileNameChars();
            std::vector<TCHAR> validChars = {'a', 'b', 'c', '1', '2', '3', '.'};
            for (auto validChar : validChars) {
                EXPECT_EQ(std::find(invalidChars.begin(), invalidChars.end(), validChar), invalidChars.end());
            }
        }
        TEST(PathTest, GetInvalidPathChars_Should_Return_Vector_Of_Correct_Size) {
            auto invalidChars = Path::GetInvalidPathChars();
            EXPECT_EQ(33, invalidChars.size());
        }

        TEST(PathTest, GetInvalidPathChars_Should_Contain_Pipe_Char) {
            auto invalidChars = Path::GetInvalidPathChars();
            EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), '|'), invalidChars.end());
        }

        TEST(PathTest, GetInvalidPathChars_Should_Contain_Invalid_Control_Chars) {
            auto invalidChars = Path::GetInvalidPathChars();
            for (int i = 0; i <= 31; ++i) {
                EXPECT_NE(std::find(invalidChars.begin(), invalidChars.end(), static_cast<TCHAR>(i)), invalidChars.end());
            }
        }

        TEST(PathTest, GetInvalidPathChars_Should_Not_Contain_Valid_Chars) {
            auto invalidChars = Path::GetInvalidPathChars();
            std::vector<TCHAR> validChars = {'a', 'b', 'c', '1', '2', '3', '.', '\\', '/'};
            for (auto validChar : validChars) {
                EXPECT_EQ(std::find(invalidChars.begin(), invalidChars.end(), validChar), invalidChars.end());
            }
        }

        TEST(PathTest, GetPathRoot_Should_Return_Drive_Root) {
            String path = _T("C:\\folder\\file.txt");
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = _T("C:\\");
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot.GetRawString();
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root) {
            String path = _T("\\Server\\Share\\folder\\file.txt");
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = _T("\\Server\\Share");
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot.GetRawString();
        }

        TEST(PathTest, GetPathRoot_Should_Return_Volume_Separator_Root) {
            EXPECT_TRUE(Path::GetPathRoot(_T("C:\\")) == _T("C:\\"));
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Relative_Path) {
            EXPECT_TRUE(Path::GetPathRoot(_T("folder\\file.txt")) == _T(""));
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_Empty_Path) {
            EXPECT_TRUE(Path::GetPathRoot(_T("")) == _T(""));
        }

        TEST(PathTest, GetPathRoot_Should_Return_Drive_Root_With_Trailing_Slash) {
            String path = _T("D:\\");
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = _T("D:\\");
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot.GetRawString();
        }

        TEST(PathTest, GetPathRoot_Should_Return_UNC_Root_With_Trailing_Slash) {
            String path = _T("\\Server\\Share\\");
            String actualRoot = Path::GetPathRoot(path);
            String expectedRoot = _T("\\Server\\Share");
            EXPECT_EQ(actualRoot, expectedRoot) << "Actual root: " << actualRoot.GetRawString();
        }

        TEST(PathTest, GetPathRoot_Should_Return_Empty_String_For_File_Name_Only) {
            EXPECT_TRUE(Path::GetPathRoot(_T("file.txt")) == _T(""));
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
            EXPECT_EQ(_T('.'), randomFileName[8]);
        }

        // Given: A request for a random file name
        // When: GetRandomFileName is called
        // Then: The returned string should contain only valid characters
        TEST(PathTest, GetRandomFileName_WhenCalled_ReturnsStringWithValidChars) {
            // Arrange
            const String validChars = _T("abcdefghijklmnopqrstuvwxyz0123456789.");

            // Act
            String randomFileName = Path::GetRandomFileName();

            // Assert
            for (int i = 0; i < randomFileName.GetLength(); ++i) {
                EXPECT_TRUE(validChars.Contains(randomFileName[i]));
            }
        }

        // Negative tests for GetRandomFileName are not applicable as the method does not take any input
        // and is designed to always return a valid 8.3 format file name.
    }
}

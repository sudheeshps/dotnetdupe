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
    }
}

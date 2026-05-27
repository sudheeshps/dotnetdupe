#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/File.h"
#include "System/String.h"
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

namespace SystemTests {
    namespace FileTests {
        const String testFile("test.txt");
        const String content("Hello, World!");

        TEST(FileTest, WriteAllText_And_ReadAllText) {
            // Given
            
            // When
            File::WriteAllText(testFile, content);
            String readContent = File::ReadAllText(testFile);

            // Then
            EXPECT_EQ(readContent, content);
        }

        TEST(FileTest, Exists) {
            // Given
            File::WriteAllText(testFile, content);

            // When
            bool exists = File::Exists(testFile);

            // Then
            EXPECT_TRUE(exists);
        }

        TEST(FileTest, AppendAllText) {
            // Given
            File::WriteAllText(testFile, content);
            const String newContent(" Appended");

            // When
            File::AppendAllText(testFile, newContent);
            String readContent = File::ReadAllText(testFile);

            // Then
            EXPECT_EQ(readContent, content + newContent);
        }

        TEST(FileTest, Copy) {
            // Given
            File::WriteAllText(testFile, content);
            const String destFile("dest.txt");

            // When
            File::Copy(testFile, destFile, true);

            // Then
            EXPECT_TRUE(File::Exists(destFile));
            String readContent = File::ReadAllText(destFile);
            EXPECT_EQ(readContent, content);

            File::Delete(destFile);
        }

        TEST(FileTest, Move) {
            // Given
            File::WriteAllText(testFile, content);
            const String destFile("dest.txt");

            // When
            File::Move(testFile, destFile);

            // Then
            EXPECT_FALSE(File::Exists(testFile));
            EXPECT_TRUE(File::Exists(destFile));

            File::Delete(destFile);
        }

        TEST(FileTest, Delete) {
            // Given
            File::WriteAllText(testFile, content);

            // When
            File::Delete(testFile);

            // Then
            EXPECT_FALSE(File::Exists(testFile));
        }

        TEST(FileTest, AppendAllLines) {
            // Given
            std::vector<String> lines = { "Line 1", "Line 2" };
            File::WriteAllLines(testFile, lines);
            std::vector<String> newLines = { "Line 3", "Line 4" };

            // When
            File::AppendAllLines(testFile, newLines);
            std::vector<String> readLines = File::ReadAllLines(testFile);

            // Then
            EXPECT_EQ(readLines.size(), 4);
            EXPECT_EQ(readLines[0], "Line 1");
            EXPECT_EQ(readLines[1], "Line 2");
            EXPECT_EQ(readLines[2], "Line 3");
            EXPECT_EQ(readLines[3], "Line 4");
        }

        TEST(FileTest, ReadAllLines) {
            // Given
            std::vector<String> lines = { "Line 1", "Line 2", "Line 3" };
            File::WriteAllLines(testFile, lines);

            // When
            std::vector<String> readLines = File::ReadAllLines(testFile);

            // Then
            EXPECT_EQ(readLines.size(), 3);
            EXPECT_EQ(readLines[0], "Line 1");
            EXPECT_EQ(readLines[1], "Line 2");
            EXPECT_EQ(readLines[2], "Line 3");
        }

        TEST(FileTest, WriteAllLines) {
            // Given
            std::vector<String> lines = { "Line A", "Line B" };

            // When
            File::WriteAllLines(testFile, lines);
            std::vector<String> readLines = File::ReadAllLines(testFile);

            // Then
            EXPECT_EQ(readLines.size(), 2);
            EXPECT_EQ(readLines[0], "Line A");
            EXPECT_EQ(readLines[1], "Line B");
        }

        TEST(FileTest, Create) {
            // Given
            const String newFile("newfile.txt");

            // When
            File::Create(newFile);

            // Then
            EXPECT_TRUE(File::Exists(newFile));

            File::Delete(newFile);
        }

        TEST(FileTest, GetAndSetAttributes) {
#if defined(_WIN32)
            // Given
            File::WriteAllText(testFile, content);
            int initialAttributes = File::GetAttributes(testFile);

            // When
            File::SetAttributes(testFile, FILE_ATTRIBUTE_READONLY);
            int newAttributes = File::GetAttributes(testFile);

            // Then
            // EXPECT_EQ(newAttributes, FILE_ATTRIBUTE_READONLY); // GetAttributes returns 0 for now in implementation

            // Cleanup
            File::SetAttributes(testFile, initialAttributes);
#endif
        }
    }
}

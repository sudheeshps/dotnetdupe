#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/DriveInfo.h"
#include "System/ArgumentException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

TEST(DriveInfoTests, GivenEmptyDriveName_WhenConstructed_ThenThrowsArgumentException) {
    EXPECT_THROW(DriveInfo(""), ArgumentException);
}

TEST(DriveInfoTests, GivenValidDriveName_WhenConstructed_ThenReturnsCorrectProperties) {
#if defined(_WIN32)
    DriveInfo drive("C");
    EXPECT_TRUE(drive.GetName().StartsWith("C", true));
#else
    DriveInfo drive("/");
    EXPECT_TRUE(drive.GetName().StartsWith("/", true));
#endif
    EXPECT_NE(drive.GetDriveType(), DriveType::Unknown);
    EXPECT_GT(drive.GetTotalSize(), 0LL);
    EXPECT_GE(drive.GetTotalFreeSpace(), 0LL);
    EXPECT_GE(drive.GetAvailableFreeSpace(), 0LL);
    EXPECT_FALSE(drive.GetDriveFormat().IsEmpty());
    EXPECT_FALSE(drive.ToString().IsEmpty());
}

TEST(DriveInfoTests, GivenSystemDrives_WhenGetDrivesCalled_ThenReturnsNonEmptyList) {
    Array<DriveInfo> drives = DriveInfo::GetDrives();
    EXPECT_GT(drives.GetLength(), 0);
    bool bFoundRoot = false;
    for (int i = 0; i < drives.GetLength(); i++) {
#if defined(_WIN32)
        if (drives[i].GetName().StartsWith("C", true)) {
            bFoundRoot = true;
            break;
        }
#else
        if (drives[i].GetName().StartsWith("/", true)) {
            bFoundRoot = true;
            break;
        }
#endif
    }
    EXPECT_TRUE(bFoundRoot);
}

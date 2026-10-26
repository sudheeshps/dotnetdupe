#include "pch.h"
#include "gtest/gtest.h"
#include "System/IO/File.h"
#include "System/IO/FileInfo.h"
#include "System/IO/FileSystemEntry.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/IO/MemoryStream.h"
#include "System/Security/Cryptography/SHA256.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/IOException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;
using namespace DotNetDupe::System::Security::Cryptography;

namespace {
    static String CreateUniqueTempDir(const char* szPrefix) {
        const String sBase = Path::Combine(Path::GetTempPath(), String(szPrefix));
        if (Directory::Exists(sBase)) {
            Directory::Delete(sBase, true);
        }
        Directory::CreateDirectory(sBase);
        return sBase;
    }
}

TEST(FileSystemEntryTests, GivenDefaultAndParameterizedEntry_WhenQueried_ThenReturnsExpectedProperties) {
    FileSystemEntry defaultEntry;
    EXPECT_FALSE(defaultEntry.GetIsDirectory());
    EXPECT_FALSE(defaultEntry.GetIsReparsePoint());
    EXPECT_EQ(defaultEntry.GetLength64(), 0);

    const DateTimeOffset dtNow = DateTimeOffset::UtcNow();
    const FileAttributes eAttrs = static_cast<FileAttributes>(
        static_cast<int>(FileAttributes::Normal) | static_cast<int>(FileAttributes::ReparsePoint));
    FileSystemEntry fileEntry(
        "/data/work/report.txt",
        "report.txt",
        4096,
        dtNow,
        dtNow,
        eAttrs);
    EXPECT_STREQ(fileEntry.GetFullPath().GetRawString(), "/data/work/report.txt");
    EXPECT_STREQ(fileEntry.GetName().GetRawString(), "report.txt");
    EXPECT_EQ(fileEntry.GetLength64(), 4096);
    EXPECT_EQ(fileEntry.GetAttributes(), eAttrs);
    EXPECT_EQ(fileEntry.GetCreationTime().GetTicks(), dtNow.GetTicks());
    EXPECT_EQ(fileEntry.GetLastWriteTime().GetTicks(), dtNow.GetTicks());
    EXPECT_FALSE(fileEntry.GetIsDirectory());
    EXPECT_TRUE(fileEntry.GetIsReparsePoint());
}

TEST(FileInfoTests, GivenExistingAndMissingFile_WhenQueried_ThenReturnsMetadataOrThrows) {
    const String sDir = CreateUniqueTempDir("DotNetDupe_FileInfoTests");
    const String sFile = Path::Combine(sDir, "sample.bin");

    EXPECT_THROW(FileInfo(""), ArgumentException);

    FileInfo missingInfo(sFile);
    EXPECT_FALSE(missingInfo.GetExists());
    EXPECT_STREQ(missingInfo.GetName().GetRawString(), "sample.bin");
    EXPECT_STREQ(missingInfo.GetExtension().GetRawString(), ".bin");
    EXPECT_STREQ(missingInfo.GetDirectoryName().GetRawString(), Path::GetDirectoryName(missingInfo.GetFullName()).GetRawString());
    EXPECT_THROW(missingInfo.GetLength64(), IOException);

    Array<byte> arrData(64);
    for (int i = 0; i < 64; ++i) {
        arrData[i] = static_cast<byte>(i + 1);
    }
    File::WriteAllBytes(sFile, arrData);

    missingInfo.Refresh();
    EXPECT_TRUE(missingInfo.GetExists());
    EXPECT_EQ(missingInfo.GetLength64(), 64);
    EXPECT_FALSE(missingInfo.GetIsReparsePoint());
    EXPECT_GT(missingInfo.GetLastWriteTime().GetTicks(), 0);
    EXPECT_GT(missingInfo.GetCreationTime().GetTicks(), 0);

    Directory::Delete(sDir, true);
}

TEST(FileOperationsTests, GivenFiles_WhenVerifiedCopyAndByteCompareCalled_ThenPreservesDataAndTimestamps) {
    const String sDir = CreateUniqueTempDir("DotNetDupe_FileOpsTests");
    const String sSrc = Path::Combine(sDir, "source.dat");
    const String sDst1 = Path::Combine(sDir, "copy1.dat");
    const String sDst2 = Path::Combine(sDir, "copy2.dat");
    const String sDiff = Path::Combine(sDir, "diff.dat");

    Array<byte> arrPayload(256);
    for (int i = 0; i < 256; ++i) {
        arrPayload[i] = static_cast<byte>((i * 7) & 0xFF);
    }
    File::WriteAllBytes(sSrc, arrPayload);

    const Array<byte> arrReadBack = File::ReadAllBytes(sSrc);
    EXPECT_EQ(arrReadBack.GetLength(), 256);
    EXPECT_EQ(File::GetLength(sSrc), 256);

    const DateTimeOffset dtCustom(638540000000000000LL);
    File::SetLastWriteTime(sSrc, dtCustom);
    const DateTimeOffset dtSrcWrite = File::GetLastWriteTime(sSrc);
    EXPECT_EQ(dtSrcWrite.GetTicks(), dtCustom.GetTicks());

    File::Copy(sSrc, sDst1, true, true);
    EXPECT_TRUE(File::AreFilesByteIdentical(sSrc, sDst1));

    EXPECT_TRUE(File::CopyVerified(sSrc, sDst2, true));
    EXPECT_TRUE(File::AreFilesByteIdentical(sSrc, sDst2));
    EXPECT_EQ(File::GetLastWriteTime(sDst2).GetTicks(), dtCustom.GetTicks());

    arrPayload[100] ^= 0xFF;
    File::WriteAllBytes(sDiff, arrPayload);
    EXPECT_FALSE(File::AreFilesByteIdentical(sSrc, sDiff));

    Directory::Delete(sDir, true);
}

TEST(DirectoryMetadataTests, GivenNestedDirectory_WhenGetFileSystemEntriesWithMetadataCalled_ThenReturnsEntries) {
    const String sDir = CreateUniqueTempDir("DotNetDupe_DirMetaTests");
    const String sSubDir = Path::Combine(sDir, "sub");
    Directory::CreateDirectory(sSubDir);

    const String sFile1 = Path::Combine(sDir, "root.txt");
    const String sFile2 = Path::Combine(sSubDir, "nested.txt");
    File::WriteAllText(sFile1, "root-data");
    File::WriteAllText(sFile2, "nested-data-12345");

    const Array<FileSystemEntry> arrTop = Directory::GetFileSystemEntriesWithMetadata(
        sDir,
        "*",
        SearchOption::TopDirectoryOnly);
    EXPECT_EQ(arrTop.GetLength(), 2);

    const Array<FileSystemEntry> arrAll = Directory::GetFileSystemEntriesWithMetadata(
        sDir,
        "*",
        SearchOption::AllDirectories);
    EXPECT_EQ(arrAll.GetLength(), 3);

    EXPECT_THROW(Directory::GetFileSystemEntriesWithMetadata(Path::Combine(sDir, "missing")), IOException);

    Directory::Delete(sDir, true);
}

TEST(PartialAndEdgeSha256Tests, GivenStreamsAndFiles_WhenPartialAndEdgeHashCalled_ThenProducesDeterministicDigests) {
    const String sDir = CreateUniqueTempDir("DotNetDupe_EdgeHashTests");
    const String sSmallFile = Path::Combine(sDir, "small.bin");
    const String sLargeFile = Path::Combine(sDir, "large.bin");
    const String sLargeFileDiffMiddle = Path::Combine(sDir, "large_diff_mid.bin");

    Array<byte> arrSmall(128);
    for (int i = 0; i < 128; ++i) {
        arrSmall[i] = static_cast<byte>(i);
    }
    File::WriteAllBytes(sSmallFile, arrSmall);

    Array<byte> arrLarge(50000);
    for (int i = 0; i < 50000; ++i) {
        arrLarge[i] = static_cast<byte>((i * 31) & 0xFF);
    }
    File::WriteAllBytes(sLargeFile, arrLarge);

    arrLarge[25000] ^= 0xFF;
    File::WriteAllBytes(sLargeFileDiffMiddle, arrLarge);

    const String sEdgeSmall = SHA256::ComputeFastEdgeHash(sSmallFile, 4096);
    EXPECT_EQ(sEdgeSmall.GetLength(), 64);

    const String sEdgeLarge1 = SHA256::ComputeFastEdgeHash(sLargeFile, 4096);
    const String sEdgeLarge2 = SHA256::ComputeFastEdgeHash(sLargeFileDiffMiddle, 4096);
    EXPECT_STREQ(sEdgeLarge1.GetRawString(), sEdgeLarge2.GetRawString());

    const String sFullHex1 = SHA256::HashFileToHex(sLargeFile);
    const String sFullHex2 = SHA256::HashFileToHex(sLargeFileDiffMiddle);
    EXPECT_STRNE(sFullHex1.GetRawString(), sFullHex2.GetRawString());

    SmartPointer<MemoryStream> pMem = SmartPointer<MemoryStream>::NewShared();
    pMem->Write(reinterpret_cast<const char*>(arrSmall.GetData()), 0, arrSmall.GetLength());
    pMem->SetPosition(0);
    const Array<byte> arrSliceHash = SHA256::HashData(pMem, 10, 32);
    EXPECT_EQ(arrSliceHash.GetLength(), 32);

    EXPECT_THROW(SHA256::HashData(SmartPointer<Stream>(), 0, 10), ArgumentNullException);
    EXPECT_THROW(SHA256::HashData(pMem, -1, 10), ArgumentOutOfRangeException);
    EXPECT_THROW(SHA256::ComputeFastEdgeHash(sSmallFile, 0), ArgumentOutOfRangeException);

    Directory::Delete(sDir, true);
}

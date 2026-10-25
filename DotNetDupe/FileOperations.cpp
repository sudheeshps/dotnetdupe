#include "pch.h"
#include "System/IO/File.h"
#include "System/IO/FileStream.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include "System/Security/Cryptography/SHA256.h"
#include <filesystem>
#include <fstream>
#include <cstring>

#if defined(_WIN32)
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#endif

namespace fs = std::filesystem;

namespace {
    fs::path ToFsPathOps(const DotNetDupe::System::String& sPath) {
        /// Convert UTF-8 library string to std::filesystem::path.
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(sPath.GetRawString()));
#else
        return fs::path(sPath.GetRawString());
#endif
    }

    DotNetDupe::System::DateTimeOffset FsTimeToDateTimeOffset(fs::file_time_type ftime) {
        /// Convert std::filesystem::file_time_type to DateTimeOffset ticks.
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        auto llSec = std::chrono::round<std::chrono::seconds>(sctp.time_since_epoch()).count();
        int64_t llTicks = (llSec + 62135596800LL) * 10000000LL;
        return DotNetDupe::System::DateTimeOffset(llTicks);
    }

    fs::file_time_type DateTimeOffsetToFsTime(const DotNetDupe::System::DateTimeOffset& dtTime) {
        /// Convert DateTimeOffset ticks to std::filesystem::file_time_type.
        int64_t llUnixSec = (dtTime.GetTicks() / 10000000LL) - 62135596800LL;
        auto sysTp = std::chrono::system_clock::time_point(std::chrono::seconds(llUnixSec));
        return fs::file_time_type::clock::now() + (sysTp - std::chrono::system_clock::now());
    }

    static bool CompareStreamsByteByByte(std::ifstream& stmA, std::ifstream& stmB) {
        /// Read 8KB chunks from both streams and verify exact byte equality.
        char szBufA[8192];
        char szBufB[8192];
        while (stmA.good() && stmB.good()) {
            stmA.read(szBufA, sizeof(szBufA));
            stmB.read(szBufB, sizeof(szBufB));
            if (stmA.gcount() != stmB.gcount()) return false;
            if (stmA.gcount() > 0 && std::memcmp(szBufA, szBufB, static_cast<size_t>(stmA.gcount())) != 0) return false;
        }
        return stmA.eof() && stmB.eof();
    }

    static bool StreamToStagingFile(
        const DotNetDupe::System::String& sSrc,
        const DotNetDupe::System::String& sPartPath,
        long long llTotalBytes,
        DotNetDupe::System::Action<long long, long long>& fnProgress) {
        /// Stream bytes from source to staging .part file and invoke progress callback.
        std::ifstream stmIn(ToFsPathOps(sSrc), std::ios::binary);
        std::ofstream stmOut(ToFsPathOps(sPartPath), std::ios::binary | std::ios::trunc);
        if (!stmIn.is_open() || !stmOut.is_open()) return false;
        char szBuf[65536];
        long long llCopied = 0;
        while (stmIn.good()) {
            stmIn.read(szBuf, sizeof(szBuf));
            std::streamsize nRead = stmIn.gcount();
            if (nRead <= 0) break;
            stmOut.write(szBuf, nRead);
            llCopied += static_cast<long long>(nRead);
            if (fnProgress) fnProgress(llCopied, llTotalBytes);
        }
        stmOut.flush();
        return stmOut.good();
    }

    static bool VerifyAndCommitStagingFile(
        const DotNetDupe::System::String& sSrc,
        const DotNetDupe::System::String& sPartPath,
        const DotNetDupe::System::String& sDst,
        long long llExpectedSize,
        bool bPreserveTimestamps) {
        /// Verify staging file size and SHA-256 match source before atomic replacement.
        if (DotNetDupe::System::IO::File::GetLength(sPartPath) != llExpectedSize) return false;
        auto sSrcHash = DotNetDupe::System::Security::Cryptography::SHA256::HashFileToHex(sSrc);
        auto sPartHash = DotNetDupe::System::Security::Cryptography::SHA256::HashFileToHex(sPartPath);
        if (sSrcHash != sPartHash) return false;
        if (DotNetDupe::System::IO::File::Exists(sDst)) DotNetDupe::System::IO::File::Delete(sDst);
        DotNetDupe::System::IO::File::Move(sPartPath, sDst);
        if (bPreserveTimestamps) {
            DotNetDupe::System::IO::File::SetLastWriteTime(sDst, DotNetDupe::System::IO::File::GetLastWriteTime(sSrc));
        }
        return true;
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {

            long long File::GetLength(const String& sPath) {
                /// Guard: Reject empty path.
                if (sPath.IsEmpty()) {
                    throw ArgumentException("Path cannot be empty.");
                }

                /// Query 64-bit file size from filesystem.
                std::error_code ec;
                auto uSize = fs::file_size(ToFsPathOps(sPath), ec);
                if (ec) {
                    throw IO::IOException("Failed to retrieve file length.");
                }
                return static_cast<long long>(uSize);
            }

            DateTimeOffset File::GetCreationTime(const String& sPath) {
#if defined(_WIN32)
                /// Query Win32 file creation time via GetFileAttributesExW.
                WIN32_FILE_ATTRIBUTE_DATA data{};
                std::wstring wsPath = StringConvertInternal::Utf8ToWChar(sPath.GetRawString());
                if (!GetFileAttributesExW(wsPath.c_str(), GetFileExInfoStandard, &data)) {
                    throw IO::IOException("Failed to retrieve file creation time.");
                }
                ULARGE_INTEGER uli{};
                uli.LowPart = data.ftCreationTime.dwLowDateTime;
                uli.HighPart = data.ftCreationTime.dwHighDateTime;
                int64_t llTicks = static_cast<int64_t>(uli.QuadPart) + 504911232000000000LL;
                return DateTimeOffset(llTicks);
#else
                /// Fallback to last write time on POSIX platforms.
                return GetLastWriteTime(sPath);
#endif
            }

            DateTimeOffset File::GetLastWriteTime(const String& sPath) {
                /// Query last write timestamp from filesystem.
                std::error_code ec;
                auto ftime = fs::last_write_time(ToFsPathOps(sPath), ec);
                if (ec) {
                    throw IO::IOException("Failed to retrieve file last write time.");
                }
                return FsTimeToDateTimeOffset(ftime);
            }

            void File::SetLastWriteTime(const String& sPath, const DateTimeOffset& dtLastWriteTime) {
                /// Update last write timestamp on filesystem entry.
                std::error_code ec;
                fs::last_write_time(ToFsPathOps(sPath), DateTimeOffsetToFsTime(dtLastWriteTime), ec);
                if (ec) {
                    throw IO::IOException("Failed to set file last write time.");
                }
            }

            Array<byte> File::ReadAllBytes(const String& sPath) {
                /// Guard: Open binary file stream and query byte length.
                long long llLen = GetLength(sPath);
                std::ifstream stmIn(ToFsPathOps(sPath), std::ios::binary);
                if (!stmIn.is_open()) {
                    throw IO::IOException("Failed to open file for reading bytes.");
                }

                /// Allocate byte array and read complete content.
                Array<byte> arrResult(static_cast<int>(llLen));
                if (llLen > 0) {
                    stmIn.read(reinterpret_cast<char*>(arrResult.GetData()), static_cast<std::streamsize>(llLen));
                }
                return arrResult;
            }

            void File::WriteAllBytes(const String& sPath, const Array<byte>& arrBytes) {
                /// Open binary output stream and write all bytes.
                std::ofstream stmOut(ToFsPathOps(sPath), std::ios::binary | std::ios::trunc);
                if (!stmOut.is_open()) {
                    throw IO::IOException("Failed to open file for writing bytes.");
                }
                if (arrBytes.GetLength() > 0) {
                    stmOut.write(reinterpret_cast<const char*>(arrBytes.GetData()), arrBytes.GetLength());
                }
            }

            bool File::AreFilesByteIdentical(const String& sPathA, const String& sPathB) {
                /// Guard: Verify both files exist and have identical 64-bit length.
                if (!Exists(sPathA) || !Exists(sPathB)) return false;
                long long llLenA = GetLength(sPathA);
                if (llLenA != GetLength(sPathB)) return false;
                if (llLenA == 0) return true;

                /// Open both streams and compare byte-for-byte.
                std::ifstream stmA(ToFsPathOps(sPathA), std::ios::binary);
                std::ifstream stmB(ToFsPathOps(sPathB), std::ios::binary);
                if (!stmA.is_open() || !stmB.is_open()) return false;
                return CompareStreamsByteByByte(stmA, stmB);
            }

            void File::Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite, bool bPreserveTimestamps) {
                /// Perform standard file copy and optionally preserve last write timestamp.
                Copy(sSourceFileName, sDestFileName, bOverwrite);
                if (bPreserveTimestamps) {
                    SetLastWriteTime(sDestFileName, GetLastWriteTime(sSourceFileName));
                }
            }

            bool File::CopyVerified(
                const String& sSourceFileName,
                const String& sDestFileName,
                bool bOverwrite,
                bool bPreserveTimestamps,
                Action<long long, long long> fnProgress) {
                /// Guard: Validate source existence and destination overwrite policy.
                if (!Exists(sSourceFileName)) throw IO::IOException("Source file does not exist.");
                if (!bOverwrite && Exists(sDestFileName)) throw IO::IOException("Destination file already exists.");

                /// Ensure parent directory exists and stream to .part staging file.
                String sParentDir = Path::GetDirectoryName(sDestFileName);
                if (!sParentDir.IsEmpty() && !Directory::Exists(sParentDir)) Directory::CreateDirectory(sParentDir, true);
                String sPartPath = sDestFileName + ".backman.part";
                long long llTotalBytes = GetLength(sSourceFileName);
                if (!StreamToStagingFile(sSourceFileName, sPartPath, llTotalBytes, fnProgress)) return false;

                /// Read back staging file, verify SHA-256 and size, and atomically commit.
                return VerifyAndCommitStagingFile(sSourceFileName, sPartPath, sDestFileName, llTotalBytes, bPreserveTimestamps);
            }

        }
    }
}

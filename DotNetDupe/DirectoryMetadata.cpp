#include "pch.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/IOException.h"
#include <filesystem>
#include <vector>
#include <regex>

#if defined(_WIN32)
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#endif

namespace fs = std::filesystem;

namespace {
    std::regex BuildWildcardRegex(const DotNetDupe::System::String& sPattern) {
        /// Convert wildcard search pattern (* and ?) into case-insensitive regular expression.
        std::string sPat = sPattern.GetRawString();
        std::string sReg;
        for (char c : sPat) {
            if (c == '*') sReg += ".*";
            else if (c == '?') sReg += ".";
            else if (std::strchr(".\\/()[]^$+|{}", c) != nullptr) { sReg += "\\"; sReg += c; }
            else sReg += c;
        }
        return std::regex("^" + sReg + "$", std::regex_constants::icase);
    }

#if defined(_WIN32)
    DotNetDupe::System::DateTimeOffset FileTimeToDto(const FILETIME& ft) noexcept {
        /// Convert Win32 FILETIME (100-ns intervals since 1601) to DateTimeOffset ticks.
        ULARGE_INTEGER uli{};
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        int64_t llTicks = static_cast<int64_t>(uli.QuadPart) + 504911232000000000LL;
        return DotNetDupe::System::DateTimeOffset(llTicks);
    }

    bool IsDotOrDotDotW(const wchar_t* wzName) noexcept {
        /// Return true if directory entry name is "." or "..".
        if (wzName[0] != L'.') return false;
        return (wzName[1] == L'\0') || (wzName[1] == L'.' && wzName[2] == L'\0');
    }

    DotNetDupe::System::IO::FileSystemEntry CreateEntryFromFindData(
        const DotNetDupe::System::String& sDirPath,
        const WIN32_FIND_DATAW& fd) {
        /// Construct FileSystemEntry from Win32 WIN32_FIND_DATAW structure.
        DotNetDupe::System::String sName(StringConvertInternal::WCharToUtf8(fd.cFileName).c_str());
        DotNetDupe::System::String sFullPath = DotNetDupe::System::IO::Path::Combine(sDirPath, sName);
        ULARGE_INTEGER uliSize{};
        uliSize.LowPart = fd.nFileSizeLow;
        uliSize.HighPart = fd.nFileSizeHigh;
        bool bIsDir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        long long llSize = bIsDir ? 0LL : static_cast<long long>(uliSize.QuadPart);
        auto eAttrs = static_cast<DotNetDupe::System::IO::FileAttributes>(fd.dwFileAttributes);
        return DotNetDupe::System::IO::FileSystemEntry(
            sFullPath, sName, llSize, FileTimeToDto(fd.ftCreationTime), FileTimeToDto(fd.ftLastWriteTime), eAttrs);
    }

    void EnumerateDirectorySinglePassWin32(
        const DotNetDupe::System::String& sDirPath,
        const std::regex& regPattern,
        bool bRecursive,
        std::vector<DotNetDupe::System::IO::FileSystemEntry>& vecResults) {
        /// Enumerate directory via FindFirstFileExW with FindExInfoBasic and LARGE_FETCH.
        DotNetDupe::System::String sSearchSpec = DotNetDupe::System::IO::Path::Combine(sDirPath, "*");
        std::wstring wsSpec = StringConvertInternal::Utf8ToWChar(sSearchSpec.GetRawString());
        WIN32_FIND_DATAW fd{};
        HANDLE hFind = FindFirstFileExW(wsSpec.c_str(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
        if (hFind == INVALID_HANDLE_VALUE) return;
        std::vector<DotNetDupe::System::String> vecSubDirs;
        do {
            if (IsDotOrDotDotW(fd.cFileName)) continue;
            auto entry = CreateEntryFromFindData(sDirPath, fd);
            if (std::regex_match(entry.GetName().GetRawString(), regPattern)) vecResults.push_back(entry);
            if (bRecursive && entry.GetIsDirectory() && !entry.GetIsReparsePoint()) vecSubDirs.push_back(entry.GetFullPath());
        } while (FindNextFileW(hFind, &fd) != FALSE);
        FindClose(hFind);
        for (const auto& sSubDir : vecSubDirs) EnumerateDirectorySinglePassWin32(sSubDir, regPattern, true, vecResults);
    }
#else
    DotNetDupe::System::DateTimeOffset PosixTimeToDto(fs::file_time_type ftime) {
        /// Convert POSIX std::filesystem::file_time_type to DateTimeOffset ticks.
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        auto llSec = std::chrono::duration_cast<std::chrono::seconds>(sctp.time_since_epoch()).count();
        return DotNetDupe::System::DateTimeOffset((llSec + 62135596800LL) * 10000000LL);
    }

    DotNetDupe::System::IO::FileSystemEntry CreateEntryFromDirEntryPosix(const fs::directory_entry& de) {
        /// Construct FileSystemEntry from POSIX std::filesystem::directory_entry without following symlink loops.
        std::error_code ec;
        auto symStatus = de.symlink_status(ec);
        auto regStatus = de.status(ec);
        bool bIsSymlink = fs::is_symlink(symStatus);
        bool bIsDir = fs::is_directory(regStatus);
        int iAttrs = static_cast<int>(DotNetDupe::System::IO::FileAttributes::Normal);
        if (bIsDir) iAttrs |= static_cast<int>(DotNetDupe::System::IO::FileAttributes::Directory);
        if (bIsSymlink) iAttrs |= static_cast<int>(DotNetDupe::System::IO::FileAttributes::ReparsePoint);
        std::string sNameStd = de.path().filename().string();
        if (!sNameStd.empty() && sNameStd[0] == '.') iAttrs |= static_cast<int>(DotNetDupe::System::IO::FileAttributes::Hidden);
        long long llSize = (!bIsDir && !bIsSymlink) ? static_cast<long long>(de.file_size(ec)) : 0LL;
        if (ec) llSize = 0LL;
        auto dtWrite = PosixTimeToDto(de.last_write_time(ec));
        return DotNetDupe::System::IO::FileSystemEntry(
            DotNetDupe::System::String(de.path().string().c_str()),
            DotNetDupe::System::String(sNameStd.c_str()),
            llSize, dtWrite, dtWrite, static_cast<DotNetDupe::System::IO::FileAttributes>(iAttrs));
    }

    void EnumerateDirectorySinglePassPosix(
        const DotNetDupe::System::String& sDirPath,
        const std::regex& regPattern,
        bool bRecursive,
        std::vector<DotNetDupe::System::IO::FileSystemEntry>& vecResults) {
        /// Enumerate POSIX directory and recurse into non-symlink subdirectories when requested.
        std::error_code ec;
        auto opts = fs::directory_options::skip_permission_denied;
        std::vector<DotNetDupe::System::String> vecSubDirs;
        for (auto it = fs::directory_iterator(fs::path(sDirPath.GetRawString()), opts, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
            if (ec) break;
            auto entry = CreateEntryFromDirEntryPosix(*it);
            if (std::regex_match(entry.GetName().GetRawString(), regPattern)) vecResults.push_back(entry);
            if (bRecursive && entry.GetIsDirectory() && !entry.GetIsReparsePoint()) vecSubDirs.push_back(entry.GetFullPath());
        }
        for (const auto& sSubDir : vecSubDirs) EnumerateDirectorySinglePassPosix(sSubDir, regPattern, true, vecResults);
    }
#endif
    DotNetDupe::System::Array<DotNetDupe::System::IO::FileSystemEntry> VectorToEntryArray(
        const std::vector<DotNetDupe::System::IO::FileSystemEntry>& vecEntries) {
        /// Convert collected vector into ABI-safe Array<FileSystemEntry>.
        DotNetDupe::System::Array<DotNetDupe::System::IO::FileSystemEntry> arrResult(static_cast<int>(vecEntries.size()));
        for (int iIdx = 0; iIdx < static_cast<int>(vecEntries.size()); ++iIdx) arrResult[iIdx] = vecEntries[iIdx];
        return arrResult;
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {

            Array<FileSystemEntry> Directory::GetFileSystemEntriesWithMetadata(const String& sPath) {
                /// Delegate to wildcard overload with TopDirectoryOnly option.
                return GetFileSystemEntriesWithMetadata(sPath, "*", SearchOption::TopDirectoryOnly);
            }

            Array<FileSystemEntry> Directory::GetFileSystemEntriesWithMetadata(
                const String& sPath,
                const String& sSearchPattern,
                SearchOption eSearchOption) {
                /// Guard: Verify target directory exists before enumeration.
                if (!Exists(sPath)) throw IO::IOException("Directory does not exist.");
                std::vector<FileSystemEntry> vecEntries;
                std::regex regPattern = BuildWildcardRegex(sSearchPattern);
                bool bRecursive = (eSearchOption == SearchOption::AllDirectories);
#if defined(_WIN32)
                EnumerateDirectorySinglePassWin32(sPath, regPattern, bRecursive, vecEntries);
#else
                EnumerateDirectorySinglePassPosix(sPath, regPattern, bRecursive, vecEntries);
#endif
                return VectorToEntryArray(vecEntries);
            }

        }
    }
}

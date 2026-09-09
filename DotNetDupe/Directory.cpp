#include "pch.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/IOException.h"
#include "System/ArgumentException.h"
#include <filesystem>
#include <vector>
#include <regex>

#if defined(_WIN32)
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#endif

namespace fs = std::filesystem;

namespace {
    fs::path ToFsPath(const DotNetDupe::System::String& sPath) {
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(sPath.GetRawString()));
#else
        return fs::path(sPath.GetRawString());
#endif
    }

    std::regex PatternToRegex(const DotNetDupe::System::String& sPattern) {
        std::string sPat = sPattern.GetRawString();
        std::string sRegexStr;
        for (char c : sPat) {
            if (c == '*') sRegexStr += ".*";
            else if (c == '?') sRegexStr += ".";
            else if (c == '.' || c == '\\' || c == '/' || c == '(' || c == ')' || c == '[' || c == ']' || c == '^' || c == '$' || c == '+' || c == '|' || c == '{' || c == '}') {
                sRegexStr += "\\";
                sRegexStr += c;
            } else {
                sRegexStr += c;
            }
        }
        return std::regex("^" + sRegexStr + "$", std::regex_constants::icase);
    }

    DotNetDupe::System::DateTimeOffset FileTimeToDateTimeOffset(fs::file_time_type ftime) {
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        auto durationSec = std::chrono::duration_cast<std::chrono::seconds>(sctp.time_since_epoch()).count();
        // 62135596800ULL is seconds between 0001-01-01 and 1970-01-01
        int64_t ticks = (durationSec + 62135596800LL) * 10000000LL;
        return DotNetDupe::System::DateTimeOffset(ticks);
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            bool Directory::Exists(const String& sPath) {
                /// Guard: Check for empty path.
                if (sPath.IsEmpty()) return false;
                std::error_code ec;
                /// Return: Filesystem directory existence check.
                return fs::is_directory(ToFsPath(sPath), ec);
            }

            void Directory::CreateDirectory(const String& sPath) {
                /// Forward: Delegate to recursive directory creation overload.
                CreateDirectory(sPath, true);
            }

            void Directory::CreateDirectory(const String& sPath, bool bRecursive) {
                /// Guard: Validate path parameter.
                if (sPath.IsEmpty()) {
                    throw ArgumentException("Path cannot be empty.");
                }
                std::error_code ec;
                /// Step: Create single or recursive directories.
                if (bRecursive) {
                    fs::create_directories(ToFsPath(sPath), ec);
                } else {
                    fs::create_directory(ToFsPath(sPath), ec);
                }
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
            }

            void Directory::Delete(const String& sPath) {
                /// Forward: Delegate to non-recursive delete.
                Delete(sPath, false);
            }

            void Directory::Delete(const String& sPath, bool bRecursive) {
                /// Guard: Validate directory existence.
                if (sPath.IsEmpty()) {
                    throw ArgumentException("Path cannot be empty.");
                }
                fs::path p = ToFsPath(sPath);
                std::error_code ec;
                if (!fs::exists(p, ec)) {
                    throw IO::IOException("Directory does not exist.");
                }

                /// Step: Remove directory or subtree.
                if (bRecursive) {
                    fs::remove_all(p, ec);
                } else {
                    fs::remove(p, ec);
                }
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
            }

            void Directory::Move(const String& sSourceDirName, const String& sDestDirName) {
                /// Guard: Validate directory names.
                if (sSourceDirName.IsEmpty() || sDestDirName.IsEmpty()) {
                    throw ArgumentException("Source and destination paths cannot be empty.");
                }
                std::error_code ec;
                /// Step: Rename directory path.
                fs::rename(ToFsPath(sSourceDirName), ToFsPath(sDestDirName), ec);
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
            }

            Array<String> Directory::GetFiles(const String& sPath) {
                /// Forward: Delegate with wildcard pattern "*".
                return GetFiles(sPath, "*");
            }

            Array<String> Directory::GetFiles(const String& sPath, const String& sSearchPattern) {
                /// Guard: Validate directory existence.
                if (!Exists(sPath)) {
                    throw IO::IOException("Directory does not exist.");
                }
                std::vector<String> results;
                std::regex reg = PatternToRegex(sSearchPattern);
                std::error_code ec;

                /// Step: Iterate entries and collect matching files.
                for (const auto& entry : fs::directory_iterator(ToFsPath(sPath), ec)) {
                    if (entry.is_regular_file(ec)) {
                        std::string filename = entry.path().filename().string();
                        if (std::regex_match(filename, reg)) {
                            results.push_back(String(entry.path().string().c_str()));
                        }
                    }
                }
                /// Return: Array of matching file paths.
                Array<String> arr((int)results.size());
                for (int iIdx = 0; iIdx < (int)results.size(); iIdx++) arr[iIdx] = results[iIdx];
                return arr;
            }

            Array<String> Directory::GetDirectories(const String& sPath) {
                /// Forward: Delegate with wildcard pattern "*".
                return GetDirectories(sPath, "*");
            }

            Array<String> Directory::GetDirectories(const String& sPath, const String& sSearchPattern) {
                /// Guard: Validate directory existence.
                if (!Exists(sPath)) {
                    throw IO::IOException("Directory does not exist.");
                }
                std::vector<String> results;
                std::regex reg = PatternToRegex(sSearchPattern);
                std::error_code ec;

                /// Step: Iterate entries and collect matching directories.
                for (const auto& entry : fs::directory_iterator(ToFsPath(sPath), ec)) {
                    if (entry.is_directory(ec)) {
                        std::string filename = entry.path().filename().string();
                        if (std::regex_match(filename, reg)) {
                            results.push_back(String(entry.path().string().c_str()));
                        }
                    }
                }
                /// Return: Array of matching directory paths.
                Array<String> arr((int)results.size());
                for (int iIdx = 0; iIdx < (int)results.size(); iIdx++) arr[iIdx] = results[iIdx];
                return arr;
            }

            Array<String> Directory::GetFileSystemEntries(const String& sPath) {
                /// Forward: Delegate with wildcard pattern "*".
                return GetFileSystemEntries(sPath, "*");
            }

            Array<String> Directory::GetFileSystemEntries(const String& sPath, const String& sSearchPattern) {
                /// Guard: Validate directory existence.
                if (!Exists(sPath)) {
                    throw IO::IOException("Directory does not exist.");
                }
                std::vector<String> results;
                std::regex reg = PatternToRegex(sSearchPattern);
                std::error_code ec;

                /// Step: Iterate entries and collect matching items.
                for (const auto& entry : fs::directory_iterator(ToFsPath(sPath), ec)) {
                    std::string filename = entry.path().filename().string();
                    if (std::regex_match(filename, reg)) {
                        results.push_back(String(entry.path().string().c_str()));
                    }
                }
                /// Return: Array of matching entry paths.
                Array<String> arr((int)results.size());
                for (int iIdx = 0; iIdx < (int)results.size(); iIdx++) arr[iIdx] = results[iIdx];
                return arr;
            }

            String Directory::GetCurrentDirectory() {
                /// Step: Query current working directory from filesystem.
                std::error_code ec;
                fs::path p = fs::current_path(ec);
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
                return String(p.string().c_str());
            }

            void Directory::SetCurrentDirectory(const String& sPath) {
                /// Guard: Validate target path.
                if (sPath.IsEmpty()) {
                    throw ArgumentException("Path cannot be empty.");
                }
                std::error_code ec;
                /// Step: Set current working directory.
                fs::current_path(ToFsPath(sPath), ec);
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
            }

            String Directory::GetDirectoryRoot(const String& sPath) {
                /// Forward: Delegate to Path::GetPathRoot.
                return Path::GetPathRoot(sPath);
            }

            DateTimeOffset Directory::GetCreationTime(const String& sPath) {
                /// Forward: Delegate to GetLastWriteTime.
                return GetLastWriteTime(sPath);
            }

            DateTimeOffset Directory::GetLastWriteTime(const String& sPath) {
                /// Step: Query filesystem last write time.
                std::error_code ec;
                auto ftime = fs::last_write_time(ToFsPath(sPath), ec);
                if (ec) {
                    throw IO::IOException(ec.message().c_str());
                }
                return FileTimeToDateTimeOffset(ftime);
            }

            DateTimeOffset Directory::GetLastAccessTime(const String& sPath) {
                /// Forward: Delegate to GetLastWriteTime.
                return GetLastWriteTime(sPath);
            }
        }
    }
}

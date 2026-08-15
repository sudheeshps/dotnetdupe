#include "pch.h"
#include "System/IO/File.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "System/Text/TextEncoding.h"
#include "System/IOException.h"

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
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            bool File::Exists(const String& sPath) {
                return fs::exists(ToFsPath(sPath));
            }

            String File::ReadAllText(const String& sPath) {
                std::ifstream fileStream(ToFsPath(sPath), std::ios::binary);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for reading.");
                }
                std::stringstream buffer;
                buffer << fileStream.rdbuf();
                std::string charContent = buffer.str();
                
                Array<char> bytes((int)charContent.size());
                for (int iIdx = 0; iIdx < (int)charContent.size(); iIdx++) bytes[iIdx] = charContent[iIdx];
                return Text::TextEncoding::UTF8()->GetString(bytes);
            }

            void File::WriteAllText(const String& sPath, const String& sContents) {
                Array<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(sContents);

                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for writing.");
                }
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite) {
                fs::copy_options options = fs::copy_options::none;
                if (bOverwrite) options |= fs::copy_options::overwrite_existing;
                
                try {
                    fs::copy(ToFsPath(sSourceFileName), ToFsPath(sDestFileName), options);
                } catch (const std::exception& ex) {
                    throw IO::IOException(ex.what());
                }
            }

            void File::Move(const String& sSourceFileName, const String& sDestFileName) {
                try {
                    fs::rename(ToFsPath(sSourceFileName), ToFsPath(sDestFileName));
                } catch (const std::exception& ex) {
                    throw IO::IOException(ex.what());
                }
            }

            void File::Delete(const String& sPath) {
                try {
                    fs::remove(ToFsPath(sPath));
                } catch (const std::exception& ex) {
                    throw IO::IOException(ex.what());
                }
            }

            void File::AppendAllText(const String& sPath, const String& sContents) {
                Array<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(sContents);
                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary | std::ios_base::app);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for writing.");
                }
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::AppendAllLines(const String& sPath, const Array<String>& sContents) {
                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary | std::ios_base::app);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for writing.");
                }
                for (int iIdx = 0; iIdx < sContents.GetLength(); iIdx++) {
                    const auto& line = sContents[iIdx];
                    Array<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    fileStream.write(lineBytes.GetData(), lineBytes.GetLength());
                    fileStream << "\n";
                }
            }

            Array<String> File::ReadAllLines(const String& sPath) {
                std::vector<String> tempLines;
                std::ifstream fileStream(ToFsPath(sPath), std::ios::binary);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for reading.");
                }
                std::string line;
                while (std::getline(fileStream, line)) {
                    Array<char> lineBytes((int)line.size());
                    for (int iIdx = 0; iIdx < (int)line.size(); iIdx++) lineBytes[iIdx] = line[iIdx];
                    tempLines.push_back(Text::TextEncoding::UTF8()->GetString(lineBytes));
                }
                
                Array<String> lines((int)tempLines.size());
                for (int iIdx = 0; iIdx < (int)tempLines.size(); iIdx++) lines[iIdx] = tempLines[iIdx];
                return lines;
            }

            void File::WriteAllLines(const String& sPath, const Array<String>& sContents) {
                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary);
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for writing.");
                }
                for (int iIdx = 0; iIdx < sContents.GetLength(); iIdx++) {
                    const auto& line = sContents[iIdx];
                    Array<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    fileStream.write(lineBytes.GetData(), lineBytes.GetLength());
                    fileStream << "\n";
                }
            }

            void File::Create(const String& sPath) {
                std::ofstream fileStream(ToFsPath(sPath));
                if (!fileStream.is_open()) {
                    throw IO::IOException("Failed to open file for writing.");
                }
            }

#if !defined(_WIN32)
            static int ComputeLinuxFileAttributes(const fs::path& pPath, const fs::file_status& statusInfo) {
                int iAttrs = static_cast<int>(FileAttributes::Normal);
                if ((statusInfo.permissions() & fs::perms::owner_write) == fs::perms::none) iAttrs |= static_cast<int>(FileAttributes::ReadOnly);
                if (fs::is_directory(statusInfo)) iAttrs |= static_cast<int>(FileAttributes::Directory);
                if (!pPath.filename().empty() && pPath.filename().string()[0] == '.') iAttrs |= static_cast<int>(FileAttributes::Hidden);
                return iAttrs;
            }

            static fs::perms ComputeLinuxFilePermissions(FileAttributes eAttributes, fs::perms permsCurrent) {
                if ((static_cast<int>(eAttributes) & static_cast<int>(FileAttributes::ReadOnly)) != 0) {
                    return permsCurrent & ~(fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write);
                }
                return permsCurrent | fs::perms::owner_write;
            }
#endif

            bool File::GetAttributes(const String& sPath, FileAttributes& eAttributes) {
            #if defined(_WIN32)
                std::wstring sWPath = Utf8ToWChar(sPath.GetRawString());
                DWORD iAttrs = GetFileAttributesW(sWPath.c_str());
                if (iAttrs == INVALID_FILE_ATTRIBUTES) return false;
                eAttributes = static_cast<FileAttributes>(iAttrs);
                return true;
            #else
                fs::path pPath = ToFsPath(sPath);
                std::error_code ec;
                auto statusInfo = fs::status(pPath, ec);
                if (ec) return false;
                eAttributes = static_cast<FileAttributes>(ComputeLinuxFileAttributes(pPath, statusInfo));
                return true;
            #endif
            }

            bool File::SetAttributes(const String& sPath, FileAttributes eAttributes) {
            #if defined(_WIN32)
                std::wstring sWPath = Utf8ToWChar(sPath.GetRawString());
                return ::SetFileAttributesW(sWPath.c_str(), static_cast<DWORD>(eAttributes)) != FALSE;
            #else
                fs::path pPath = ToFsPath(sPath);
                std::error_code ec;
                auto statusInfo = fs::status(pPath, ec);
                if (ec) return false;
                fs::permissions(pPath, ComputeLinuxFilePermissions(eAttributes, statusInfo.permissions()), fs::perm_options::replace, ec);
                return !ec;
            #endif
            }
        }
    }
}

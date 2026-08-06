#include "pch.h"
#include "System/IO/File.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "System/Text/TextEncoding.h"

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
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::Copy(const String& sSourceFileName, const String& sDestFileName, bool bOverwrite) {
                fs::copy_options options = fs::copy_options::none;
                if (bOverwrite) options |= fs::copy_options::overwrite_existing;
                
                try {
                    fs::copy(ToFsPath(sSourceFileName), ToFsPath(sDestFileName), options);
                } catch (...) {
                    // Handle error
                }
            }

            void File::Move(const String& sSourceFileName, const String& sDestFileName) {
                try {
                    fs::rename(ToFsPath(sSourceFileName), ToFsPath(sDestFileName));
                } catch (...) {
                    // Handle error
                }
            }

            void File::Delete(const String& sPath) {
                try {
                    fs::remove(ToFsPath(sPath));
                } catch (...) {
                    // Handle error
                }
            }

            void File::AppendAllText(const String& sPath, const String& sContents) {
                Array<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(sContents);
                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary | std::ios_base::app);
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::AppendAllLines(const String& sPath, const Array<String>& sContents) {
                std::ofstream fileStream(ToFsPath(sPath), std::ios::binary | std::ios_base::app);
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
                for (int iIdx = 0; iIdx < sContents.GetLength(); iIdx++) {
                    const auto& line = sContents[iIdx];
                    Array<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    fileStream.write(lineBytes.GetData(), lineBytes.GetLength());
                    fileStream << "\n";
                }
            }

            void File::Create(const String& sPath) {
                std::ofstream fileStream(ToFsPath(sPath));
            }

            bool File::GetAttributes(const String& sPath, FileAttributes& eAttributes) {
            #if defined(_WIN32)
                std::wstring sWPath = Utf8ToWChar(sPath.GetRawString());
                DWORD iAttrs = GetFileAttributesW(sWPath.c_str());
                if (iAttrs == INVALID_FILE_ATTRIBUTES) {
                    return false;
                }
                eAttributes = static_cast<FileAttributes>(iAttrs);
                return true;
            #else
                fs::path pPath = ToFsPath(sPath);
                std::error_code ecError;
                auto statusInfo = fs::status(pPath, ecError);
                if (ecError) {
                    return false;
                }

                int iAttrs = static_cast<int>(FileAttributes::Normal);
                if ((statusInfo.permissions() & fs::perms::owner_write) == fs::perms::none) {
                    iAttrs |= static_cast<int>(FileAttributes::ReadOnly);
                }
                if (fs::is_directory(statusInfo)) {
                    iAttrs |= static_cast<int>(FileAttributes::Directory);
                }

                // On Linux, we consider files starting with '.' as hidden
                if (!pPath.filename().empty() && pPath.filename().string()[0] == '.') {
                    iAttrs |= static_cast<int>(FileAttributes::Hidden);
                }

                eAttributes = static_cast<FileAttributes>(iAttrs);
                return true;
            #endif
            }

            bool File::SetAttributes(const String& sPath, FileAttributes eAttributes) {
            #if defined(_WIN32)
                std::wstring sWPath = Utf8ToWChar(sPath.GetRawString());
                if (::SetFileAttributesW(sWPath.c_str(), static_cast<DWORD>(eAttributes))) {
                    return true;
                }
                return false;
            #else
                fs::path pPath = ToFsPath(sPath);
                std::error_code ecError;
                auto statusInfo = fs::status(pPath, ecError);
                if (ecError) {
                    return false;
                }

                auto permsCurrent = statusInfo.permissions();
                if ((static_cast<int>(eAttributes) & static_cast<int>(FileAttributes::ReadOnly)) != 0) {
                    permsCurrent &= ~(fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write);
                } else {
                    permsCurrent |= fs::perms::owner_write;
                }

                fs::permissions(pPath, permsCurrent, fs::perm_options::replace, ecError);
                return !ecError;
            #endif
            }
        }
    }
}

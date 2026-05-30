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
    fs::path ToFsPath(const DotNetDupe::System::String& path) {
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::Utf8ToWChar(path.GetRawString()));
#else
        return fs::path(path.GetRawString());
#endif
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            bool File::Exists(const String& path) {
                return fs::exists(ToFsPath(path));
            }

            String File::ReadAllText(const String& path) {
                std::ifstream fileStream(ToFsPath(path), std::ios::binary);
                std::stringstream buffer;
                buffer << fileStream.rdbuf();
                std::string charContent = buffer.str();
                
                Array<char> bytes((int)charContent.size());
                for (int i = 0; i < (int)charContent.size(); i++) bytes[i] = charContent[i];
                return Text::TextEncoding::UTF8()->GetString(bytes);
            }

            void File::WriteAllText(const String& path, const String& contents) {
                Array<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);

                std::ofstream fileStream(ToFsPath(path), std::ios::binary);
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::Copy(const String& sourceFileName, const String& destFileName, bool overwrite) {
                fs::copy_options options = fs::copy_options::none;
                if (overwrite) options |= fs::copy_options::overwrite_existing;
                
                try {
                    fs::copy(ToFsPath(sourceFileName), ToFsPath(destFileName), options);
                } catch (...) {
                    // Handle error
                }
            }

            void File::Move(const String& sourceFileName, const String& destFileName) {
                try {
                    fs::rename(ToFsPath(sourceFileName), ToFsPath(destFileName));
                } catch (...) {
                    // Handle error
                }
            }

            void File::Delete(const String& path) {
                try {
                    fs::remove(ToFsPath(path));
                } catch (...) {
                    // Handle error
                }
            }

            void File::AppendAllText(const String& path, const String& contents) {
                Array<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);
                std::ofstream fileStream(ToFsPath(path), std::ios::binary | std::ios_base::app);
                fileStream.write(contentBytes.GetData(), contentBytes.GetLength());
            }

            void File::AppendAllLines(const String& path, const Array<String>& contents) {
                std::ofstream fileStream(ToFsPath(path), std::ios::binary | std::ios_base::app);
                for (int i = 0; i < contents.GetLength(); i++) {
                    const auto& line = contents[i];
                    Array<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    fileStream.write(lineBytes.GetData(), lineBytes.GetLength());
                    fileStream << "\n";
                }
            }

            Array<String> File::ReadAllLines(const String& path) {
                std::vector<String> tempLines;
                std::ifstream fileStream(ToFsPath(path), std::ios::binary);
                std::string line;
                while (std::getline(fileStream, line)) {
                    Array<char> lineBytes((int)line.size());
                    for (int i = 0; i < (int)line.size(); i++) lineBytes[i] = line[i];
                    tempLines.push_back(Text::TextEncoding::UTF8()->GetString(lineBytes));
                }
                
                Array<String> lines((int)tempLines.size());
                for (int i = 0; i < (int)tempLines.size(); i++) lines[i] = tempLines[i];
                return lines;
            }

            void File::WriteAllLines(const String& path, const Array<String>& contents) {
                std::ofstream fileStream(ToFsPath(path), std::ios::binary);
                for (int i = 0; i < contents.GetLength(); i++) {
                    const auto& line = contents[i];
                    Array<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    fileStream.write(lineBytes.GetData(), lineBytes.GetLength());
                    fileStream << "\n";
                }
            }

            void File::Create(const String& path) {
                std::ofstream fileStream(ToFsPath(path));
            }

            bool File::GetAttributes(const String& path, FileAttributes& fileAttributes) {
            #if defined(_WIN32)
                std::wstring wpath = Utf8ToWChar(path.GetRawString());
                DWORD dwAttrs = GetFileAttributesW(wpath.c_str());
                if (dwAttrs == INVALID_FILE_ATTRIBUTES) {
                    return false;
                }
                fileAttributes = static_cast<FileAttributes>(dwAttrs);
                return true;
            #else
                fs::path p = ToFsPath(path);
                std::error_code ec;
                auto status = fs::status(p, ec);
                if (ec) {
                    return false;
                }

                int attrs = static_cast<int>(FileAttributes::Normal);
                if ((status.permissions() & fs::perms::owner_write) == fs::perms::none) {
                    attrs |= static_cast<int>(FileAttributes::ReadOnly);
                }
                if (fs::is_directory(status)) {
                    attrs |= static_cast<int>(FileAttributes::Directory);
                }

                // On Linux, we consider files starting with '.' as hidden
                if (!p.filename().empty() && p.filename().string()[0] == '.') {
                    attrs |= static_cast<int>(FileAttributes::Hidden);
                }

                fileAttributes = static_cast<FileAttributes>(attrs);
                return true;
            #endif
            }

            bool File::SetAttributes(const String& path, FileAttributes fileAttributes) {
            #if defined(_WIN32)
                std::wstring wpath = Utf8ToWChar(path.GetRawString());
                if (::SetFileAttributesW(wpath.c_str(), static_cast<DWORD>(fileAttributes))) {
                    return true;
                }
                return false;
            #else
                fs::path p = ToFsPath(path);
                std::error_code ec;
                auto status = fs::status(p, ec);
                if (ec) {
                    return false;
                }

                auto perms = status.permissions();
                if ((static_cast<int>(fileAttributes) & static_cast<int>(FileAttributes::ReadOnly)) != 0) {
                    perms &= ~(fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write);
                } else {
                    perms |= fs::perms::owner_write;
                }

                fs::permissions(p, perms, fs::perm_options::replace, ec);
                return !ec;
            #endif
            }
        }
    }
}

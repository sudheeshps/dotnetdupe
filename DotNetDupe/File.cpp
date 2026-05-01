#include "pch.h"
#include "System/IO/File.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <vector>
#include "System/Text/TextEncoding.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            std::string File::ToNarrowPath(const String& path) {
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);
                return narrowPath;
            }

            bool File::Exists(const String& path) {
                std::ifstream f(ToNarrowPath(path).c_str());
                return f.good();
            }

            String File::ReadAllText(const String& path) {
                std::ifstream f(ToNarrowPath(path).c_str(), std::ios::binary);
                std::stringstream buffer;
                buffer << f.rdbuf();
                std::string charContent = buffer.str();
                
                // Convert char content to String (wchar_t) using UTF8 encoding
                std::vector<char> bytes(charContent.begin(), charContent.end());
                return Text::TextEncoding::UTF8()->GetString(bytes);
            }

            void File::WriteAllText(const String& path, const String& contents) {
                // Convert String (wchar_t) contents to char bytes using UTF8 encoding
                std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);

                std::ofstream f(ToNarrowPath(path).c_str(), std::ios::binary);
                f.write(contentBytes.data(), contentBytes.size());
            }

            void File::Copy(const String& sourceFileName, const String& destFileName, bool overwrite) {
                if (!CopyFileA(ToNarrowPath(sourceFileName).c_str(), ToNarrowPath(destFileName).c_str(), !overwrite)) {
                    // Handle error, for now just returning
                }
            }

            void File::Move(const String& sourceFileName, const String& destFileName) {
                if (!MoveFileA(ToNarrowPath(sourceFileName).c_str(), ToNarrowPath(destFileName).c_str())) {
                    // Handle error, for now just returning
                }
            }

            void File::Delete(const String& path) {
                if (!DeleteFileA(ToNarrowPath(path).c_str())) {
                    // Handle error, for now just returning
                }
            }

            void File::AppendAllText(const String& path, const String& contents) {
                String existingContent = ReadAllText(path);
                String newContent = existingContent + contents;
                WriteAllText(path, newContent);
            }

            void File::AppendAllLines(const String& path, const std::vector<String>& contents) {
                std::ofstream f(ToNarrowPath(path).c_str(), std::ios::binary | std::ios_base::app);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n"; // Add newline character
                }
            }

            std::vector<String> File::ReadAllLines(const String& path) {
                std::vector<String> lines;
                std::ifstream f(ToNarrowPath(path).c_str(), std::ios::binary);
                std::string line;
                while (std::getline(f, line)) {
                    std::vector<char> lineBytes(line.begin(), line.end());
                    lines.push_back(Text::TextEncoding::UTF8()->GetString(lineBytes));
                }
                return lines;
            }

            void File::WriteAllLines(const String& path, const std::vector<String>& contents) {
                std::ofstream f(ToNarrowPath(path).c_str(), std::ios::binary);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n"; // Add newline character
                }
            }

            void File::Create(const String& path) {
                HANDLE hFile = CreateFileA(ToNarrowPath(path).c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile != INVALID_HANDLE_VALUE) {
                    CloseHandle(hFile);
                }
            }

            int File::GetAttributes(const String& path) {
                return GetFileAttributesA(ToNarrowPath(path).c_str());
            }

            void File::SetAttributes(const String& path, int fileAttributes) {
                SetFileAttributesA(ToNarrowPath(path).c_str(), fileAttributes);
            }
        }
    }
}
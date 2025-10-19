#include "../pch.h"
#include "File.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <vector>
#include "../Text/TextEncoding.h"

namespace DotNetDupe {
    namespace System {
        namespace IO {
            bool File::Exists(const String& path) {
                // Convert wchar_t path to char path for std::ifstream
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                std::ifstream f(narrowPath.c_str());
                return f.good();
            }

            String File::ReadAllText(const String& path) {
                // Convert wchar_t path to char path for std::ifstream
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                std::ifstream f(narrowPath.c_str(), std::ios::binary);
                std::stringstream buffer;
                buffer << f.rdbuf();
                std::string charContent = buffer.str();
                
                // Convert char content to String (wchar_t) using UTF8 encoding
                std::vector<char> bytes(charContent.begin(), charContent.end());
                return Text::TextEncoding::UTF8()->GetString(bytes);
            }

            void File::WriteAllText(const String& path, const String& contents) {
                // Convert wchar_t path to char path for std::ofstream
                int pathBufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(pathBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], pathBufferSize, NULL, NULL);

                // Convert String (wchar_t) contents to char bytes using UTF8 encoding
                std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);

                std::ofstream f(narrowPath.c_str(), std::ios::binary);
                f.write(contentBytes.data(), contentBytes.size());
            }

            void File::Copy(const String& sourceFileName, const String& destFileName, bool overwrite) {
                // Convert wchar_t paths to char paths
                int sourceBufferSize = WideCharToMultiByte(CP_UTF8, 0, sourceFileName.GetRawString(), sourceFileName.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowSourcePath(sourceBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, sourceFileName.GetRawString(), sourceFileName.GetLength(), &narrowSourcePath[0], sourceBufferSize, NULL, NULL);

                int destBufferSize = WideCharToMultiByte(CP_UTF8, 0, destFileName.GetRawString(), destFileName.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowDestPath(destBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, destFileName.GetRawString(), destFileName.GetLength(), &narrowDestPath[0], destBufferSize, NULL, NULL);

                if (!CopyFileA(narrowSourcePath.c_str(), narrowDestPath.c_str(), !overwrite)) {
                    // Handle error, for now just returning
                }
            }

            void File::Move(const String& sourceFileName, const String& destFileName) {
                // Convert wchar_t paths to char paths
                int sourceBufferSize = WideCharToMultiByte(CP_UTF8, 0, sourceFileName.GetRawString(), sourceFileName.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowSourcePath(sourceBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, sourceFileName.GetRawString(), sourceFileName.GetLength(), &narrowSourcePath[0], sourceBufferSize, NULL, NULL);

                int destBufferSize = WideCharToMultiByte(CP_UTF8, 0, destFileName.GetRawString(), destFileName.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowDestPath(destBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, destFileName.GetRawString(), destFileName.GetLength(), &narrowDestPath[0], destBufferSize, NULL, NULL);

                if (!MoveFileA(narrowSourcePath.c_str(), narrowDestPath.c_str())) {
                    // Handle error, for now just returning
                }
            }

            void File::Delete(const String& path) {
                // Convert wchar_t path to char path
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                if (!DeleteFileA(narrowPath.c_str())) {
                    // Handle error, for now just returning
                }
            }

            void File::AppendAllText(const String& path, const String& contents) {
                // Convert wchar_t path to char path
                int pathBufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(pathBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], pathBufferSize, NULL, NULL);

                // Convert String (wchar_t) contents to char bytes using UTF8 encoding
                std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);

                std::ofstream f(narrowPath.c_str(), std::ios::binary | std::ios_base::app);
                f.write(contentBytes.data(), contentBytes.size());
            }

            void File::AppendAllLines(const String& path, const std::vector<String>& contents) {
                // Convert wchar_t path to char path
                int pathBufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(pathBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], pathBufferSize, NULL, NULL);

                std::ofstream f(narrowPath.c_str(), std::ios::binary | std::ios_base::app);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n"; // Add newline character
                }
            }

            std::vector<String> File::ReadAllLines(const String& path) {
                std::vector<String> lines;
                // Convert wchar_t path to char path
                int pathBufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(pathBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], pathBufferSize, NULL, NULL);

                std::ifstream f(narrowPath.c_str(), std::ios::binary);
                std::string line;
                while (std::getline(f, line)) {
                    std::vector<char> lineBytes(line.begin(), line.end());
                    lines.push_back(Text::TextEncoding::UTF8()->GetString(lineBytes));
                }
                return lines;
            }

            void File::WriteAllLines(const String& path, const std::vector<String>& contents) {
                // Convert wchar_t path to char path
                int pathBufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(pathBufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], pathBufferSize, NULL, NULL);

                std::ofstream f(narrowPath.c_str(), std::ios::binary);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n"; // Add newline character
                }
            }

            void File::Create(const String& path) {
                // Convert wchar_t path to char path
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                HANDLE hFile = CreateFileA(narrowPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile != INVALID_HANDLE_VALUE) {
                    CloseHandle(hFile);
                }
            }

            int File::GetAttributes(const String& path) {
                // Convert wchar_t path to char path
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                return GetFileAttributesA(narrowPath.c_str());
            }

            void File::SetAttributes(const String& path, int fileAttributes) {
                // Convert wchar_t path to char path
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), NULL, 0, NULL, NULL);
                std::string narrowPath(bufferSize, '\0');
                WideCharToMultiByte(CP_UTF8, 0, path.GetRawString(), path.GetLength(), &narrowPath[0], bufferSize, NULL, NULL);

                SetFileAttributesA(narrowPath.c_str(), fileAttributes);
            }
        }
    }
}
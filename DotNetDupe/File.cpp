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
                std::ifstream f(ToFsPath(path), std::ios::binary);
                std::stringstream buffer;
                buffer << f.rdbuf();
                std::string charContent = buffer.str();
                
                std::vector<char> bytes(charContent.begin(), charContent.end());
                return Text::TextEncoding::UTF8()->GetString(bytes);
            }

            void File::WriteAllText(const String& path, const String& contents) {
                std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);

                std::ofstream f(ToFsPath(path), std::ios::binary);
                f.write(contentBytes.data(), contentBytes.size());
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
                std::vector<char> contentBytes = Text::TextEncoding::UTF8()->GetBytes(contents);
                std::ofstream f(ToFsPath(path), std::ios::binary | std::ios_base::app);
                f.write(contentBytes.data(), contentBytes.size());
            }

            void File::AppendAllLines(const String& path, const std::vector<String>& contents) {
                std::ofstream f(ToFsPath(path), std::ios::binary | std::ios_base::app);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n";
                }
            }

            std::vector<String> File::ReadAllLines(const String& path) {
                std::vector<String> lines;
                std::ifstream f(ToFsPath(path), std::ios::binary);
                std::string line;
                while (std::getline(f, line)) {
                    std::vector<char> lineBytes(line.begin(), line.end());
                    lines.push_back(Text::TextEncoding::UTF8()->GetString(lineBytes));
                }
                return lines;
            }

            void File::WriteAllLines(const String& path, const std::vector<String>& contents) {
                std::ofstream f(ToFsPath(path), std::ios::binary);
                for (const auto& line : contents) {
                    std::vector<char> lineBytes = Text::TextEncoding::UTF8()->GetBytes(line);
                    f.write(lineBytes.data(), lineBytes.size());
                    f << "\n";
                }
            }

            void File::Create(const String& path) {
                std::ofstream f(ToFsPath(path));
            }

            int File::GetAttributes(const String& path) {
                // Not fully portable in a simple 'int', but we can return something basic or throw
                return 0; 
            }

            void File::SetAttributes(const String& path, int fileAttributes) {
                // Not fully portable
            }
        }
    }
}

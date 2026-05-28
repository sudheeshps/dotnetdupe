#include "pch.h"
#include "System/Path.h"
#include "System/IOException.h"
#include <filesystem>
#include <random>
#include <string>
#include <fstream>
#include <algorithm>

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
#undef GetTempFileName
#undef GetTempPath
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
            String Path::ChangeExtension(const String& filePath, const String& extension) {
                if (filePath.GetLength() <= 0 || extension.GetLength() <= 0)
                    throw ArgumentException("Invalid argument");
                if (filePath.GetLength() <= extension.GetLength())
                    throw ArgumentException("Invalid argument");
                auto strTempFilePath = filePath;
                if (filePath [0] == '.')
                    strTempFilePath = filePath.Remove(0);

                fs::path path = ToFsPath(strTempFilePath);
                path.replace_extension(Utf8ToWChar(extension.GetRawString()));
                return String(WCharToUtf8(path.wstring().c_str()).c_str());
            }

            String Path::Combine(const std::initializer_list<String> paths) {
                auto indexedList = _init_list_with_indexer(paths);
                fs::path rootPath = ToFsPath(indexedList [0]);
                if (!rootPath.has_root_directory())
                    return indexedList [0];
                String strCombinedPath("");
                for (auto path : paths) {
                    strCombinedPath.Append(path);
                    strCombinedPath.Append(static_cast<char>(fs::path::preferred_separator));
                }
                return strCombinedPath.Remove(strCombinedPath.GetLength() - 1);
            }

            bool Path::EndsInDirectorySeparator(const String& filePath) {
                if (filePath.IsEmpty()) return false;
                char lastChar = filePath[filePath.GetLength() - 1];
                return lastChar == '\\' || lastChar == '/';
            }

            bool Path::Exists(const String& filePath) {
                return fs::exists(ToFsPath(filePath));
            }

            String Path::GetDirectoryName(const String& filePath) {
                if (filePath.IsEmpty())
                    return String("");

                fs::path path = ToFsPath(filePath);
                return String(WCharToUtf8(path.parent_path().wstring().c_str()).c_str());
            }

            String Path::GetFileName(const String& filePath) {
                if (filePath.IsEmpty())
                    return String("");
                fs::path path = ToFsPath(filePath);
                return String(WCharToUtf8(path.filename().wstring().c_str()).c_str());
            }

            String Path::GetExtension(const String& filePath) {
                if (filePath.IsEmpty())
                    return String("");

                fs::path path = ToFsPath(filePath);
                std::wstring ext = path.extension().wstring();
                if (ext.empty()) return String("");
                return String(WCharToUtf8(ext.c_str() + 1).c_str());
            }

            String Path::GetFileNameWithoutExtension(const String& filePath) {
                if (filePath.IsEmpty())
                    return String("");

                fs::path path = ToFsPath(filePath);
                return String(WCharToUtf8(path.stem().wstring().c_str()).c_str());
            }

            String Path::GetFullPath(const String& path) {
                if (path.IsEmpty())
                    return String("");

                return String(WCharToUtf8(fs::absolute(ToFsPath(path)).wstring().c_str()).c_str());
            }

            Array<char> Path::GetInvalidFileNameChars() {
                static const std::vector<char> tempInvalidChars = {
                    '"', '<', '>', '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
                    ':', '*', '?', '\\', '/' };
                Array<char> invalidChars((int)tempInvalidChars.size());
                for (int i = 0; i < (int)tempInvalidChars.size(); i++) invalidChars[i] = tempInvalidChars[i];
                return invalidChars;
            }

            Array<char> Path::GetInvalidPathChars() {
                static const std::vector<char> tempInvalidChars = {
                    '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F' };
                Array<char> invalidChars((int)tempInvalidChars.size());
                for (int i = 0; i < (int)tempInvalidChars.size(); i++) invalidChars[i] = tempInvalidChars[i];
                return invalidChars;
            }

            String Path::GetRandomFileName()
            {
                const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
                const int chars_len = sizeof(chars) / sizeof(char) - 1;

                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<> distribution(0, chars_len - 1);

                char random_string[13];

                for (int i = 0; i < 8; ++i) {
                    random_string[i] = chars[distribution(generator)];
                }
                random_string[8] = '.';
                for (int i = 0; i < 3; ++i) {
                    random_string[9 + i] = chars[distribution(generator)];
                }
                random_string[12] = '\0';

                return String(random_string);
            }

            String Path::GetPathRoot(const String& path)
            {
                if (path.IsEmpty())
                    return String("");

                if (path.StartsWith(String("\\"), false))
                {
                    // UNC path like \\server\share
                    int len = path.GetLength();
                    if (len > 2)
                    {
                        int idx = path.IndexOf(String("\\"), 2, false);
                        if (idx != -1)
                        {
                            idx = path.IndexOf(String("\\"), idx + 1, false);
                            if (idx != -1)
                                return path.Substring(0, idx);
                        }
                    }
                    return path;
                }

                fs::path p = ToFsPath(path);
                return String(WCharToUtf8(p.root_path().wstring().c_str()).c_str());
            }

            String Path::GetRelativePath(const String& relativeTo, const String& path) {
                const fs::path fsRelativeTo = ToFsPath(GetFullPath(relativeTo));
                const fs::path fsPath = ToFsPath(GetFullPath(path));

                if (fsRelativeTo.root_name() != fsPath.root_name()) {
                    return path;
                }

                return String(WCharToUtf8(fs::relative(fsPath, fsRelativeTo).wstring().c_str()).c_str());
            }

            String Path::GetTempFileName() {
                String temp_dir_path = GetTempPath();

                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> distribution(0, 35);
                const char alphabet [] = "abcdefghijklmnopqrstuvwxyz0123456789";

                for (int i = 0; i < 100; ++i) {
                    char random_name [9];
                    for (int j = 0; j < 8; ++j) {
                        random_name [j] = alphabet [distribution(generator)];
                    }
                    random_name [8] = '\0';

                    fs::path file_path = ToFsPath(temp_dir_path) / Utf8ToWChar((String(random_name) + ".tmp").GetRawString());

                    if (!fs::exists(file_path)) {
                        std::ofstream ofs(file_path);
                        if (ofs) {
                            return String(WCharToUtf8(file_path.wstring().c_str()).c_str());
                        }
                    }
                }

                throw IOException("Could not create a unique temporary file.");
            }

            String Path::GetTempPath() {
#if defined(_WIN32)
                wchar_t buffer [MAX_PATH];
                ::GetTempPathW(MAX_PATH, buffer);
                String path(WCharToUtf8(buffer).c_str());
                if (!path.EndsWith('\\', false)) path.Append('\\');
                return path;
#else
                const char* tmpdir = getenv("TMPDIR");
                String path = tmpdir ? String(tmpdir) : String("/tmp/");
                if (!path.EndsWith('/', false)) path.Append('/');
                return path;
#endif
            }

            bool Path::HasExtension(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }

                String filenameStr = GetFileName(path);

                if (filenameStr.IsEmpty() || filenameStr == "." || filenameStr == "..") {
                    return false;
                }

                std::string filename((const char*)filenameStr);

                auto dot_pos = filename.rfind('.');

                if (dot_pos == std::string::npos || dot_pos == filename.length() - 1) {
                    return false;
                }

                return true;
            }

            bool Path::IsPathFullyQualified(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }
                return ToFsPath(path).is_absolute();
            }

            bool Path::IsPathRooted(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }

                int len = path.GetLength();

                if (len >= 1) {
                    char first = path[0];
                    if (first == '\\' || first == '/') {
                        return true;
                    }
                }

                if (len >= 2) {
                    char first = path[0];
                    char second = path[1];
                    if (second == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z'))) {
                        if (len >= 3) {
                            char third = path[2];
                            if (third == '\\' || third == '/') {
                                return true;
                            }
                        }
                        return false;
                    }
                }

                return false;
            }

            String Path::Join(const std::initializer_list<String> paths) {
                String result("");
                if (TryJoin(paths, result)) {
                    return result;
                }
                throw ArgumentException("Invalid character in path.");
            }

            bool Path::TryJoin(const std::initializer_list<String> paths, String& result) {
                auto invalidPathChars = GetInvalidPathChars();
                for (const auto& path : paths) {
                    for (auto invalidChar : invalidPathChars) {
                        if (path.Contains(invalidChar)) {
                            result = String("");
                            return false;
                        }
                    }
                }

                String joinedPath("");
                for (const auto& path : paths) {
                    if (path.IsEmpty()) {
                        continue;
                    }

                    if (joinedPath.IsEmpty()) {
                        joinedPath = path;
                    }
                    else {
                        char lastChar = joinedPath [joinedPath.GetLength() - 1];
                        if (lastChar != '\\' && lastChar != '/') {
                            joinedPath.Append(static_cast<char>(fs::path::preferred_separator));
                        }

                        if (path [0] == '\\' || path [0] == '/') {
                            joinedPath.Append(path.Substring(1, path.GetLength() - 1));
                        }
                        else {
                            joinedPath.Append(path);
                        }
                    }
                }

                result = joinedPath;
                return true;
            }

            String Path::TrimEndingDirectorySeparator(const String& path) {
                if (path.IsEmpty()) return path;
                if (Path::IsPathRooted(path) && path.GetLength() <= 3) return path;

                char lastChar = path[path.GetLength() - 1];
                if (lastChar == '\\' || lastChar == '/') {
                    return path.Substring(0, path.GetLength() - 1);
                }
                return path;
            }

            char Path::GetDirectorySeparatorChar() {
                return static_cast<char>(fs::path::preferred_separator);
            }

            char Path::GetAltDirectorySeparatorChar() {
                return (fs::path::preferred_separator == '/') ? '\\' : '/';
            }

            char Path::GetVolumeSeparatorChar() {
                return ':';
            }

            char Path::GetPathSeparator() {
#if defined(_WIN32)
                return ';';
#else
                return ':';
#endif
            }
        }
    }
}

#include "pch.h"
#include "Path.h"
#include "IOException.h"
#include <filesystem>
#include <random>
#include <string>
#include <fstream>
namespace fs = std::filesystem;
namespace DotNetDupe {
    namespace System {
        namespace IO {
            String Path::ChangeExtension(const String& filePath, const String& extension) {
                if (filePath.GetLength() <= 0 || extension.GetLength() <= 0)
                    throw ArgumentException(_T("Invalid argument"));
                if (filePath.GetLength() <= extension.GetLength())
                    throw ArgumentException(_T("Invalid argument"));
                auto strTempFilePath = filePath;
                if (filePath [0] == _T('.'))
                    strTempFilePath = filePath.Remove(0);

                fs::path path = strTempFilePath.GetRawString();
                path.replace_extension(extension.GetRawString());
                return path.c_str();
            }

            String Path::Combine(const std::initializer_list<String> paths) {
                auto indexedList = _init_list_with_indexer(paths);
                fs::path rootPath = indexedList [0].GetRawString();
                if (!rootPath.has_root_directory())
                    return indexedList [0];
                String strCombinedPath(_T(""));
                for (auto path : paths) {
                    strCombinedPath.Append(path);
                    strCombinedPath.Append(fs::path::preferred_separator);
                }
                return strCombinedPath.Remove(strCombinedPath.GetLength() - 1);
            }
            bool Path::EndsInDirectorySeparator(const String& filePath) {
                fs::path path = filePath.GetRawString();
                if (!path.has_root_path())
                    return false;
                return filePath.EndsWith(fs::path::preferred_separator, false);
            }
            bool Path::Exists(const String& filePath) {
                return fs::exists(filePath.GetRawString());
            }
            String Path::GetDirectoryName(const String& filePath) {
                if (filePath.IsEmpty())
                    return _T("");

                fs::path path = filePath.GetRawString();
                return path.parent_path().c_str();
            }
            String Path::GetFileName(const String& filePath) {
                if (filePath.IsEmpty())
                    return _T("");
                fs::path path = filePath.GetRawString();
                return path.filename().c_str();
            }
            String Path::GetExtension(const String& filePath) {
                if (filePath.IsEmpty())
                    return _T("");

                fs::path path = filePath.GetRawString();
                return path.extension().c_str() + 1;
            }
            String Path::GetFileNameWithoutExtension(const String& filePath) {
                if (filePath.IsEmpty())
                    return _T("");

                fs::path path = filePath.GetRawString();
                return path.stem().c_str();
            }
            String Path::GetFullPath(const String& path) {
                if (path.IsEmpty())
                    return _T("");

                return fs::absolute(path.GetRawString()).c_str();
            }
            std::vector<TCHAR> Path::GetInvalidFileNameChars() {
                static const std::vector<TCHAR> invalidChars = {
                    '"', '<', '>', '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
                    ':', '*', '?', '\\', '/' };
                return invalidChars;
            }
            std::vector<TCHAR> Path::GetInvalidPathChars() {
                static const std::vector<TCHAR> invalidChars = {
                    '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F' };
                return invalidChars;
            }

            String Path::GetRandomFileName()
            {
                const TCHAR chars[] = _T("abcdefghijklmnopqrstuvwxyz0123456789");
                const int chars_len = sizeof(chars) / sizeof(TCHAR) - 1;

                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<> distribution(0, chars_len - 1);

                TCHAR random_string[13];

                for (int i = 0; i < 8; ++i) {
                    random_string[i] = chars[distribution(generator)];
                }
                random_string[8] = _T('.');
                for (int i = 0; i < 3; ++i) {
                    random_string[9 + i] = chars[distribution(generator)];
                }
                random_string[12] = _T('\0');

                return String(random_string);
            }

            String Path::GetPathRoot(const String& path)
            {
                if (path.IsEmpty())
                    return _T("");

                if (path.StartsWith(String(_T("\\")), false))
                {
                    // UNC path like \\server\share
                    int len = path.GetLength();
                    if (len > 2)
                    {
                        int idx = path.IndexOf(String(_T("\\")), 2, false);
                        if (idx != -1)
                        {
                            idx = path.IndexOf(String(_T("\\")), idx + 1, false);
                            if (idx != -1)
                                return path.Substring(0, idx);
                        }
                    }
                    return path;
                }

                fs::path p(path.GetRawString());
                return p.root_path().c_str();
            }
            String Path::GetRelativePath(const String& relativeTo, const String& path) {
                const fs::path fsRelativeTo = GetFullPath(relativeTo).GetRawString();
                const fs::path fsPath = GetFullPath(path).GetRawString();

                if (fsRelativeTo.root_name() != fsPath.root_name()) {
                    return path;
                }

                return fs::relative(fsPath, fsRelativeTo).c_str();
            }

            String Path::GetTempFileName() {
                auto temp_dir_path = GetTempPath();

                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> distribution(0, 35);
                const TCHAR alphabet [] = _T("abcdefghijklmnopqrstuvwxyz0123456789");

                for (int i = 0; i < 100; ++i) {
                    TCHAR random_name [9];
                    for (int j = 0; j < 8; ++j) {
                        random_name [j] = alphabet [distribution(generator)];
                    }
                    random_name [8] = _T('\0');

                    fs::path file_path = fs::path(temp_dir_path.GetRawString()) / (String(random_name) + _T(".tmp")).GetRawString();

                    if (!fs::exists(file_path)) {
                        std::ofstream ofs(file_path);
                        if (ofs.is_open()) {
                            ofs.close();
                            return String(file_path.c_str());
                        }
                    }
                }

                throw IOException(_T("Could not create a unique temporary file."));
            }

            String Path::GetTempPath() {
                return fs::temp_directory_path().c_str();
            }

            bool Path::HasExtension(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }

                String filenameStr = GetFileName(path);

                if (filenameStr.IsEmpty() || filenameStr == _T(".") || filenameStr == _T("..")) {
                    return false;
                }
                
                std::basic_string<TCHAR> filename(filenameStr.GetRawString());

                auto dot_pos = filename.rfind(_T('.'));

                if (dot_pos == std::string::npos || dot_pos == filename.length() - 1) {
                    return false;
                }

                return true;
            }

            bool Path::IsPathFullyQualified(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }
                return fs::path(path.GetRawString()).is_absolute();
            }

            bool Path::IsPathRooted(const String& path) {
                if (path.IsEmpty()) {
                    return false;
                }

                int len = path.GetLength();

                if (len >= 1) {
                    TCHAR first = path[0];
                    if (first == _T('\\') || first == _T('/')) {
                        return true;
                    }
                }

                if (len >= 2) {
                    TCHAR first = path[0];
                    TCHAR second = path[1];
                    if (second == _T(':') && ((first >= _T('a') && first <= _T('z')) || (first >= _T('A') && first <= _T('Z')))) {
                        if (len >= 3) {
                            TCHAR third = path[2];
                            if (third == _T('\\') || third == _T('/')) {
                                return true;
                            }
                        }
                        return false;
                    }
                }

                return false;
            }

            String Path::Join(const std::initializer_list<String> paths) {
                String result(_T(""));
                if (TryJoin(paths, result)) {
                    return result;
                }
                throw ArgumentException(_T("Invalid character in path."));
            }

            bool Path::TryJoin(const std::initializer_list<String> paths, String& result) {
                auto invalidPathChars = GetInvalidPathChars();

                for (const auto& path : paths) {
                    for (auto invalidChar : invalidPathChars) {
                        if (path.Contains(invalidChar)) {
                            result = String(_T(""));
                            return false;
                        }
                    }
                }

                String joinedPath(_T(""));
                for (const auto& path : paths) {
                    if (path.IsEmpty()) {
                        continue;
                    }
                    if (joinedPath.IsEmpty()) {
                        joinedPath = path;
                    }
                    else {
                        joinedPath.Append(fs::path::preferred_separator);
                        joinedPath.Append(path);
                    }
                }
                result = joinedPath;
                return true;
            }

            String Path::TrimEndingDirectorySeparator(const String& path) {
                if (path.IsEmpty()) {
                    return path;
                }

                if (EndsInDirectorySeparator(path)) {
                    String root = GetPathRoot(path);
                    if (path.Equals(root)) {
                        return path;
                    }
                    return path.Substring(0, path.GetLength() - 1);
                }

                return path;
            }
        }
    }
}
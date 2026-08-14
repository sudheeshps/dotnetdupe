#include "pch.h"
#include "System/IO/Path.h"
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
    fs::path ToFsPath(const DotNetDupe::System::String& sPath) {
#if defined(_WIN32)
        return fs::path(DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(sPath.GetRawString()));
#else
        return fs::path(sPath.GetRawString());
#endif
    }

    DotNetDupe::System::String FromFsPath(const fs::path& path) {
#if defined(_WIN32)
        return DotNetDupe::System::String(DotNetDupe::System::Internal::StringConvertInternal::WCharToUtf8(path.wstring().c_str()).c_str());
#else
        return DotNetDupe::System::String(path.string().c_str());
#endif
    }
}

namespace DotNetDupe {
    namespace System {
        namespace IO {
            String Path::ChangeExtension(const String& sFilePath, const String& sExtension) {
                if (sFilePath.GetLength() <= 0 || sExtension.GetLength() <= 0)
                    throw ArgumentException("Invalid argument");
                if (sFilePath.GetLength() <= sExtension.GetLength())
                    throw ArgumentException("Invalid argument");
                auto sTempFilePath = sFilePath;
                if (sFilePath [0] == '.')
                    sTempFilePath = sFilePath.Remove(0);

                fs::path path = ToFsPath(sTempFilePath);
#if defined(_WIN32)
                path.replace_extension(DotNetDupe::System::Internal::StringConvertInternal::Utf8ToWChar(sExtension.GetRawString()));
#else
                path.replace_extension(sExtension.GetRawString());
#endif
                return FromFsPath(path);
            }

            String Path::Combine(const std::initializer_list<String> sPaths) {
                auto indexedList = _init_list_with_indexer(sPaths);
                fs::path rootPath = ToFsPath(indexedList [0]);
                if (!rootPath.has_root_directory())
                    return indexedList [0];
                String sCombinedPath("");
                for (auto sPath : sPaths) {
                    sCombinedPath.Append(sPath);
                    sCombinedPath.Append(static_cast<char>(fs::path::preferred_separator));
                }
                return sCombinedPath.Remove(sCombinedPath.GetLength() - 1);
            }

            bool Path::EndsInDirectorySeparator(const String& sFilePath) {
                if (sFilePath.IsEmpty()) return false;
                char chLastChar = sFilePath[sFilePath.GetLength() - 1];
                return chLastChar == '\\' || chLastChar == '/';
            }

            bool Path::Exists(const String& sFilePath) {
                return fs::exists(ToFsPath(sFilePath));
            }

            String Path::GetDirectoryName(const String& sFilePath) {
                if (sFilePath.IsEmpty())
                    return "";

                fs::path path = ToFsPath(sFilePath);
                return FromFsPath(path.parent_path());
            }

            String Path::GetFileName(const String& sFilePath) {
                if (sFilePath.IsEmpty())
                    return "";
                fs::path path = ToFsPath(sFilePath);
                return FromFsPath(path.filename());
            }

            String Path::GetExtension(const String& sFilePath) {
                if (sFilePath.IsEmpty())
                    return "";

                fs::path path = ToFsPath(sFilePath);
                return FromFsPath(path.extension());
            }

            String Path::GetFileNameWithoutExtension(const String& sFilePath) {
                if (sFilePath.IsEmpty())
                    return "";

                fs::path path = ToFsPath(sFilePath);
                return FromFsPath(path.stem());
            }

            String Path::GetFullPath(const String& sPath) {
                if (sPath.IsEmpty())
                    return "";

                return FromFsPath(fs::absolute(ToFsPath(sPath)));
            }

            Array<char> Path::GetInvalidFileNameChars() {
                static const std::vector<char> tempInvalidChars = {
                    '"', '<', '>', '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
                    ':', '*', '?', '\\', '/' };
                Array<char> invalidChars((int)tempInvalidChars.size());
                for (int iIndex = 0; iIndex < (int)tempInvalidChars.size(); iIndex++) invalidChars[iIndex] = tempInvalidChars[iIndex];
                return invalidChars;
            }

            Array<char> Path::GetInvalidPathChars() {
                static const std::vector<char> tempInvalidChars = {
                    '|', '\0',
                    '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
                    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F' };
                Array<char> invalidChars((int)tempInvalidChars.size());
                for (int iIndex = 0; iIndex < (int)tempInvalidChars.size(); iIndex++) invalidChars[iIndex] = tempInvalidChars[iIndex];
                return invalidChars;
            }

            String Path::GetRandomFileName()
            {
                const char chChars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
                const int nCharsLen = sizeof(chChars) / sizeof(char) - 1;

                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<> distribution(0, nCharsLen - 1);

                char chRandomString[13];

                for (int iIndex = 0; iIndex < 8; ++iIndex) {
                    chRandomString[iIndex] = chChars[distribution(generator)];
                }
                chRandomString[8] = '.';
                for (int iIndex = 0; iIndex < 3; ++iIndex) {
                    chRandomString[9 + iIndex] = chChars[distribution(generator)];
                }
                chRandomString[12] = '\0';

                return String(chRandomString);
            }

            String Path::GetPathRoot(const String& sPath)
            {
                if (sPath.IsEmpty())
                    return "";

                if (sPath.StartsWith("\\", false))
                {
                    // UNC path like \\server\share
                    int nLen = sPath.GetLength();
                    if (nLen > 2)
                    {
                        int iIdx = sPath.IndexOf("\\", 2, false);
                        if (iIdx != -1)
                        {
                            iIdx = sPath.IndexOf("\\", iIdx + 1, false);
                            if (iIdx != -1)
                                return sPath.Substring(0, iIdx);
                        }
                    }
                    return sPath;
                }

                fs::path p = ToFsPath(sPath);
                return FromFsPath(p.root_path());
            }

            String Path::GetRelativePath(const String& sRelativeTo, const String& sPath) {
                const fs::path fsRelativeTo = ToFsPath(GetFullPath(sRelativeTo));
                const fs::path fsPath = ToFsPath(GetFullPath(sPath));

#if defined(_WIN32)
                if (fsRelativeTo.root_name() != fsPath.root_name()) {
                    return sPath;
                }
#endif

                return FromFsPath(fs::relative(fsPath, fsRelativeTo));
            }

            static String GenerateRandomTempFileName(const String& sTempDir) {
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> dist(0, 35);
                const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
                for (int i = 0; i < 100; ++i) {
                    char name[9];
                    for (int j = 0; j < 8; ++j) name[j] = alphabet[dist(generator)];
                    name[8] = '\0';
                    fs::path fp = ToFsPath(sTempDir) / (String(name) + ".tmp").GetRawString();
                    if (!fs::exists(fp)) {
                        std::ofstream ofs(fp);
                        if (ofs) return FromFsPath(fp);
                    }
                }
                throw IOException("Could not create a unique temporary file.");
            }

            String Path::GetTempFileName() {
                return GenerateRandomTempFileName(GetTempPath());
            }

            String Path::GetTempPath() {
#if defined(_WIN32)
                wchar_t buffer [MAX_PATH];
                ::GetTempPathW(MAX_PATH, buffer);
                String sPath(WCharToUtf8(buffer).c_str());
                if (!sPath.EndsWith('\\', false)) sPath.Append('\\');
                return sPath;
#else
                const char* pTmpDir = getenv("TMPDIR");
                String sPath = pTmpDir ? String(pTmpDir) : String("/tmp/");
                if (!sPath.EndsWith('/', false)) sPath.Append('/');
                return sPath;
#endif
            }

            bool Path::HasExtension(const String& sPath) {
                if (sPath.IsEmpty()) return false;
                String sFilenameStr = GetFileName(sPath);
                if (sFilenameStr.IsEmpty() || sFilenameStr == "." || sFilenameStr == "..") return false;
                std::string sFilename((const char*)sFilenameStr);
                auto nDotPos = sFilename.rfind('.');
                return (nDotPos != std::string::npos && nDotPos != sFilename.length() - 1);
            }

            bool Path::IsPathFullyQualified(const String& sPath) {
                if (sPath.IsEmpty()) return false;
                return ToFsPath(sPath).is_absolute();
            }

            bool Path::IsPathRooted(const String& sPath) {
                if (sPath.IsEmpty()) return false;
                int nLen = sPath.GetLength();
                if (sPath[0] == '\\' || sPath[0] == '/') return true;
                if (nLen >= 2 && sPath[1] == ':' && std::isalpha(static_cast<unsigned char>(sPath[0]))) {
                    return (nLen >= 3 && (sPath[2] == '\\' || sPath[2] == '/'));
                }
                return false;
            }

            String Path::Join(const std::initializer_list<String> sPaths) {
                String sResult("");
                if (TryJoin(sPaths, sResult)) return sResult;
                throw ArgumentException("Invalid character in path.");
            }

            static bool ValidatePathChars(const std::initializer_list<String>& sPaths) {
                auto invalidPathChars = Path::GetInvalidPathChars();
                for (const auto& sPath : sPaths) {
                    for (auto ch : invalidPathChars) {
                        if (sPath.Contains(ch)) return false;
                    }
                }
                return true;
            }

            static void AppendPathSegment(String& sJoined, const String& sPath) {
                if (sPath.IsEmpty()) return;
                if (sJoined.IsEmpty()) {
                    sJoined = sPath;
                } else {
                    char chLast = sJoined[sJoined.GetLength() - 1];
                    if (chLast != '\\' && chLast != '/') sJoined.Append(static_cast<char>(fs::path::preferred_separator));
                    if (sPath[0] == '\\' || sPath[0] == '/') {
                        sJoined.Append(sPath.Substring(1, sPath.GetLength() - 1));
                    } else {
                        sJoined.Append(sPath);
                    }
                }
            }

            bool Path::TryJoin(const std::initializer_list<String> sPaths, String& sResult) {
                if (!ValidatePathChars(sPaths)) {
                    sResult = String("");
                    return false;
                }
                String sJoinedPath("");
                for (const auto& sPath : sPaths) AppendPathSegment(sJoinedPath, sPath);
                sResult = sJoinedPath;
                return true;
            }

            String Path::TrimEndingDirectorySeparator(const String& sPath) {
                if (sPath.IsEmpty()) return sPath;
                if (Path::IsPathRooted(sPath) && sPath.GetLength() <= 3) return sPath;

                char chLastChar = sPath[sPath.GetLength() - 1];
                if (chLastChar == '\\' || chLastChar == '/') {
                    return sPath.Substring(0, sPath.GetLength() - 1);
                }
                return sPath;
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
